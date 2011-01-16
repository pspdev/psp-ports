#include <stdlib.h>
#include "pspgl_hash.h"


#define HASH(k)  ((k) % HASH_SIZE)

struct hashentry {
	unsigned long key;
	void *data;
	struct hashentry *next;
};


void* __pspgl_hash_lookup (const struct hashtable *h, unsigned long key)
{
	struct hashentry *ent;

	if (key > h->maxkey)
		return NULL;

	for (ent=h->buckets[HASH(key)]; ent!=NULL; ent=ent->next) {
		if (ent->key == key)
			return ent->data;
	}

	return NULL;
}


void __pspgl_hash_insert (struct hashtable *h, unsigned long key, void *value)
{
	unsigned long b = HASH(key);
	struct hashentry *ent;

	if (key <= h->maxkey) {
		for (ent=h->buckets[b]; ent!=NULL; ent=ent->next) {
			if (ent->key == key) {
				ent->data = value;
				return;
			}
		}
	}

	if ((ent = malloc(sizeof(*ent)))) {
		ent->key = key;
		ent->data = value;
		ent->next = h->buckets[b];
		h->buckets[b] = ent;
		if (key > h->maxkey)
			h->maxkey = key;
	}
}


void* __pspgl_hash_remove (struct hashtable *h, unsigned long key)
{
	struct hashentry **entp;

	for (entp=&h->buckets[HASH(key)]; *entp!=NULL; entp=&(*entp)->next) {
		struct hashentry *ent = *entp;

		if (ent->key == key) {
			void *ret = ent->data;
			*entp = ent->next;
			free(ent);
			return ret;
		}
	}

	return NULL;
}


unsigned long __pspgl_hash_uniquekey (const struct hashtable *h)
{
	unsigned long ret;

	if (h->maxkey < HASH_NO_KEY-1) {
		ret = h->maxkey + 1;
	} else {
		for(ret=1; ret<HASH_NO_KEY; ret++) {
			if (__pspgl_hash_lookup(h, ret) == NULL)
				break;
		}
	}

	return ret;
}


void __pspgl_hash_foreach_free (struct hashtable *h, void (*func) (void *data))
{
	unsigned long i;

	for (i=0; i<HASH_SIZE; i++) {
		struct hashentry *ent, *next;
		for(ent=h->buckets[i]; ent!=NULL; ent=next) {
			next = ent->next;
			func(ent->data);
			free(ent);
		}
		h->buckets[i] = NULL;
	}
}

