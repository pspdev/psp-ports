#include <time.h>
#include <psptypes.h>
#include <psprtc.h>

#define GL_GLEXT_PROTOTYPES
#include "pspgl_internal.h"
#include <GL/glext.h>

unsigned __pspgl_ticks_to_us(unsigned long long ticks)
{
	static unsigned tickres;

	if (tickres == 0) {
		tickres = sceRtcGetTickResolution();
		psp_log("tick resolution %d\n", tickres);
	}

	return (ticks * 1000000ull) / tickres;
}

void glEnableStatsPSP(GLenum stats)
{
	switch(stats) {
	case GL_STATS_TIMING_PSP:
		pspgl_curctx->stats.enabled = GL_TRUE;
		break;
	default:
		GLERROR(GL_INVALID_ENUM);
	}
}

void glDisableStatsPSP(GLenum stats)
{
	switch(stats) {
	case GL_STATS_TIMING_PSP:
		pspgl_curctx->stats.enabled = GL_FALSE;
		break;
	default:
		GLERROR(GL_INVALID_ENUM);
	}
}

void glResetStatsPSP(GLenum stats)
{
	switch(stats) {
	case GL_STATS_CMDISSUES_PSP:
		pspgl_curctx->stats.buffer_issues = 0;

	case GL_STATS_QUEUEWAITTIME_PSP:
		pspgl_curctx->stats.queuewait = 0;
		break;
	default:
		GLERROR(GL_INVALID_ENUM);
	}

}

void glGetStatisticsuivPSP(GLenum stats, GLuint *ret)
{
	switch(stats) {
	case GL_STATS_FRAMETIME_PSP:
		ret[0] = __pspgl_ticks_to_us(pspgl_curctx->draw->flip_end - 
					     pspgl_curctx->draw->prev_end);
		return;
	case GL_STATS_APPTIME_PSP:
		ret[0] = __pspgl_ticks_to_us(pspgl_curctx->draw->flip_start - 
					     pspgl_curctx->draw->prev_end);
		break;
	case GL_STATS_SWAPTIME_PSP:
		ret[0] = __pspgl_ticks_to_us(pspgl_curctx->draw->flip_end - 
					     pspgl_curctx->draw->flip_start);
		break;
	case GL_STATS_CMDISSUES_PSP:
		ret[0] = pspgl_curctx->stats.buffer_issues;
		break;
	case GL_STATS_QUEUEWAITTIME_PSP:
		ret[0] = __pspgl_ticks_to_us(pspgl_curctx->stats.queuewait);
		break;

	default:
		GLERROR(GL_INVALID_ENUM);
	}
}
