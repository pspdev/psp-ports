#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <pspge.h>
#include <psputils.h>

#include "pspgl_internal.h"
#include "pspgl_buffers.h"
#include "pspgl_dlist.h"

#define NUM_CMDLISTS	16u

#define DLIST_SIZE	512	/* command words (32bit) */
#define DLIST_EXTRA	4	/* 4 commands of overhead at the end */

#define DLIST_CACHED	0	/* put command list in cached memory */

struct pspgl_dlist {
	unsigned long len;	/* amount used */
	int qid;		/* queue ID; -1 if not queued */

	struct pspgl_buffer *pins; /* list of buffers pinned by this dlist */

#if DLIST_CACHED
 	unsigned long __attribute__((aligned(16))) cmd_buf[DLIST_SIZE];
#else
	unsigned long *cmd_buf;
	/* need to align to cache lines (64bytes) to avoid cached/uncached conflicts */
	unsigned long __attribute__((aligned(CACHELINE_SIZE))) _cmdbuf[DLIST_SIZE];
#endif
};


static int dlist_needinit = 1;
static struct pspgl_dlist dlists[NUM_CMDLISTS]
	__attribute__((aligned(CACHELINE_SIZE)));
static unsigned dlist_current;

static void dlist_reset (struct pspgl_dlist *d)
{
	d->qid = -1;
	d->len = 0;
	d->pins = NULL;
}

void __pspgl_dlist_init(void)
{
	if (likely(!dlist_needinit))
		return;
	dlist_needinit = 0;

	dlist_current = 0;
	for(int i = 0; i < NUM_CMDLISTS; i++) {
		struct pspgl_dlist *d = &dlists[i];

		if (!DLIST_CACHED)
			d->cmd_buf = __pspgl_uncached(d->_cmdbuf, 
						      sizeof(d->_cmdbuf));
		dlist_reset(d);
	}
}

void __pspgl_dlist_enqueue_cmd (unsigned long cmd)
{
	struct pspgl_dlist *d = &dlists[dlist_current];

	if (unlikely((d->len + 1) >= (DLIST_SIZE - DLIST_EXTRA))) {
		__pspgl_dlist_submit();
		d = &dlists[dlist_current];
	}
	d->cmd_buf[d->len++] = cmd;
}


/* Wait for a dlist to stop being used by the hardware, and do the
   appropriate updates on any buffers it pins. */
static void sync_list(struct pspgl_dlist *list)
{
	unsigned long long start=0;
	struct pspgl_buffer *data, *next;

	if (list->qid == -1)
		return;

	if (pspgl_curctx->stats.enabled)
		start = now();

	sceGeListSync(list->qid, PSP_GE_LIST_DONE);

	if (pspgl_curctx->stats.enabled)
		pspgl_curctx->stats.queuewait += now() - start;

	pspgl_ge_register_dump();
	pspgl_ge_matrix_dump();

	for(data = list->pins; data != NULL; data = next) {
		next = data->pin_next;
		data->pin_prevp = NULL;
		data->pin_next = NULL;

		data->flags &= ~BF_PINNED;

		__pspgl_buffer_free(data);
	}

	dlist_reset(list);
}

static
void dlist_finish (struct pspgl_dlist *d)
{
	if (unlikely((d->len + DLIST_EXTRA) > DLIST_SIZE))
		__pspgl_log("d->len=%d DLIST_SIZE=%d\n", d->len, DLIST_SIZE);
	assert((d->len + DLIST_EXTRA) <= DLIST_SIZE);

	d->cmd_buf[d->len++] = 0x0f000000;	/* FINISH */
	d->cmd_buf[d->len++] = 0x0c000000;	/* END */
	d->cmd_buf[d->len++] = 0x00000000;	/* NOP */
	d->cmd_buf[d->len++] = 0x00000000;	/* NOP */
}

/* Submit all pending command lists to hardware, and move to the next
   free command list.  If all lists are busy, wait until the next one
   is available (ie, hardware has finished). */
void __pspgl_dlist_submit(void)
{
	struct pspgl_dlist *d = &dlists[dlist_current];

	dlist_finish(d);
	pspgl_dlist_dump(d->cmd_buf, d->len);

	assert(d->qid == -1);
	if (DLIST_CACHED)
		sceKernelDcacheWritebackInvalidateRange(d->cmd_buf, sizeof(d->cmd_buf));

	if (pspgl_curctx->stats.enabled)
		pspgl_curctx->stats.buffer_issues++;

	psp_log("queueing %d commands", d->len);
	d->qid = sceGeListEnQueue(d->cmd_buf, &d->cmd_buf[d->len-1], 0, NULL);

	/* move to next command queue */
	dlist_current = (dlist_current + 1) % NUM_CMDLISTS;
	d = &dlists[dlist_current];
	if (d->qid != -1)
		sync_list(d);
}


/* Pin a buffer to a particular dlist.  If it is already attached
   to a dlist, move it to this dlist, so it remains pinned a little
   longer. */
void __pspgl_dlist_pin_buffer(struct pspgl_buffer *data, unsigned flags)
{
	struct pspgl_dlist *d = &dlists[dlist_current];

	if (data->pin_prevp != NULL) {
		/* pinned by someone; snip from whatever list its
		   currently on */
		if (data->pin_next)
			data->pin_next->pin_prevp = data->pin_prevp;
		*(data->pin_prevp) = data->pin_next;
	} else {
		/* newly pinned */
		data->refcount++;
	}
	data->flags |= flags;

	/* insert into current dlist pin list */
	data->pin_prevp = &d->pins;
	data->pin_next = d->pins;
	if (data->pin_next)
		data->pin_next->pin_prevp = &data->pin_next;
	d->pins = data;

	/* tell buffer management this buffer wants to be in vidmem */
	__pspgl_buffer_want_vidmem(data);
}

/* Block until the hardware has finished with all outstanding lists */
void __pspgl_dlist_await_completion (int (*pred)(void *), void *p)
{
	unsigned i;

	i = dlist_current;

	do {
		sync_list(&dlists[i]);
		i = (i + 1) % NUM_CMDLISTS;
		if (pred && (*pred)(p))
			break;
	} while(i != dlist_current);
}

/* Cancel all pending lists */
void __pspgl_dlist_cancel (void)
{
	for(int i = 0; i < NUM_CMDLISTS; i++) {
		struct pspgl_dlist *d = &dlists[i];

		if (d->qid != -1) {
			sceGeListDeQueue(d->qid);
			sync_list(d);
		}
	}
}

void * __pspgl_dlist_insert_space (unsigned long size)
{
	struct pspgl_dlist *d = &dlists[dlist_current];
	unsigned long len;
	unsigned long adr;

	size = ROUNDUP(size, sizeof(d->cmd_buf[0])) / sizeof(d->cmd_buf[0]);

	if ((d->len + 2 + size) >= (DLIST_SIZE - 4)) {
		__pspgl_dlist_submit();
		d = &dlists[dlist_current];

		assert(d->len == 0);
		assert(size <= (DLIST_SIZE - DLIST_EXTRA));
	}

	len = d->len;
	d->len += size + 2;
	adr = (unsigned long) &d->cmd_buf[d->len];
	d->cmd_buf[len  ] = (CMD_BASE << 24) | ((adr >> 8) & 0xf0000);
	d->cmd_buf[len+1] = (CMD_JUMP << 24) | (adr & 0xffffff);

	return (void *)&d->cmd_buf[len+2];
}

