#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include "../pspgl_codegen.h"


/* set all VFPU regs zero */
void vfpu_init (void)
{
        __asm__ volatile (
		cgen_asm(vmzero_q(Q_M000))	/* access register array as matrices for speed */
		cgen_asm(vmzero_q(Q_M100))
		cgen_asm(vmzero_q(Q_M200))
		cgen_asm(vmzero_q(Q_M300))
		cgen_asm(vmzero_q(Q_M400))
		cgen_asm(vmzero_q(Q_M500))
		cgen_asm(vmzero_q(Q_M600))
		cgen_asm(vmzero_q(Q_M700))
	);
}

/* initialize VFPU test pattern */
void vfpu_init1 (void)
{
        __asm__ volatile (
		cgen_asm(vone_s(S_S000))
		cgen_asm(vadd_s(S_S010, S_S000, S_S000))
		cgen_asm(vadd_s(S_S020, S_S010, S_S000))
		cgen_asm(vadd_s(S_S030, S_S020, S_S000))     /* R000 = ( 1  2  3  4) */
		cgen_asm(vone_q(Q_R703))                     /* R703 = ( 1  1  1  1) */
		cgen_asm(vscl_q(Q_R703, Q_R703, S_S030))     /* R703 = ( 4  4  4  4) */
		cgen_asm(vadd_q(Q_R001, Q_R000, Q_R703))     /* R001 = ( 5  6  7  8) */
		cgen_asm(vadd_q(Q_R002, Q_R001, Q_R703))     /* R002 = ( 9 10 11 12) */
		cgen_asm(vadd_q(Q_R003, Q_R002, Q_R703))     /* R003 = (13 14 15 16) */
		cgen_asm(vadd_q(Q_R100, Q_R003, Q_R703))     /* R100 = (17 18 19 20) */
		cgen_asm(vadd_q(Q_R101, Q_R100, Q_R703))     /* R101 = (21 22 23 24) */
		cgen_asm(vadd_q(Q_R102, Q_R101, Q_R703))     /* ... */
		cgen_asm(vadd_q(Q_R103, Q_R102, Q_R703))
		cgen_asm(vmmov_q(Q_M200, Q_M000))            /* M2 = M0 */
		cgen_asm(vmmov_q(Q_M300, Q_M100))            /* M3 = M1 */
		cgen_asm(vmzero_q(Q_M400))                   /* M4 = M5 = M6 = M7 = 0.0f */
		cgen_asm(vmzero_q(Q_M500))
		cgen_asm(vmzero_q(Q_M600))
		cgen_asm(vmzero_q(Q_M700))
	);
}


void vfpu_save_regs (float vfpu_regs [32][4])
{
        register void *ptr __asm__ ("a0") = vfpu_regs;
        __asm__ volatile (
		cgen_asm(sv_q(Q_R000, 0 * 16, R_a0, 0))
		cgen_asm(sv_q(Q_R001, 1 * 16, R_a0, 0))
		cgen_asm(sv_q(Q_R002, 2 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R003, 3 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R100, 4 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R101, 5 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R102, 6 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R103, 7 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R200, 8 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R201, 9 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R202, 10 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R203, 11 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R300, 12 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R301, 13 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R302, 14 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R303, 15 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R400, 16 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R401, 17 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R402, 18 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R403, 19 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R500, 20 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R501, 21 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R502, 22 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R503, 23 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R600, 24 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R601, 25 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R602, 26 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R603, 27 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R700, 28 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R701, 29 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R702, 30 * 16, R_a0, 0))
                cgen_asm(sv_q(Q_R703, 31 * 16, R_a0, 0))
		: "=r"(ptr) : "r"(ptr) : "memory");
}


static inline unsigned int f2x (float f)
{
	union { float f; unsigned int i; } u = { .f = f };
	return u.i;
}

void vfpu_diff (float r1 [32][4], float r2 [32][4])
{
        int i, j;

        for (i=0; i<32; i++) {
                for (j=0; j<4; j++) {
			if (r1[i][j] != r2[i][j])
				break;
		}
		if (j<4)
			pspDebugScreenPrintf("-%2i% 7.3f% 7.3f% 7.3f% 7.3f %08x%08x%08x%08x\n", 
						i,
						r1[i][0], r1[i][1], r1[i][2], r1[i][3],
						f2x(r1[i][0]), f2x(r1[i][1]), f2x(r1[i][2]), f2x(r1[i][3]));
	}

        for (i=0; i<32; i++) {
                for (j=0; j<4; j++) {
			if (r1[i][j] != r2[i][j])
				break;
		}
		if (j<4)
			pspDebugScreenPrintf("+%2i% 7.3f% 7.3f% 7.3f% 7.3f %08x%08x%08x%08x\n", 
						i,
						r2[i][0], r2[i][1], r2[i][2], r2[i][3],
						f2x(r2[i][0]), f2x(r2[i][1]), f2x(r2[i][2]), f2x(r2[i][3]));
	}
	pspDebugScreenPrintf("\n");
}


static float vfpu_regs0 [32][4] __attribute__((aligned(16)));
static float vfpu_regs1 [32][4] __attribute__((aligned(16)));

//static unsigned long test_vector [4] __attribute__((aligned(16))) = { 0xffff0000, 0x00ff0000, 0x0000ff00, 0x0000ffff };
static float test_vector [4] __attribute__((aligned(16))) = { -1.1, 2.2, -0.3, 0.4 };
//static float test_vector [4] __attribute__((aligned(16))) = { 1.0, 2.0, 3.0, 4.0 };

/**
 *  ok... this function is the place to actually try the behaviour of some yet-unknown instructions.
 */
void vfpu_testcase (void *arg)
{
	register void *ptr __asm__ ("a0") = arg;
	__asm__ volatile (
		cgen_asm(lv_q(Q_R100, 0, R_a0))
		cgen_asm(vsat0_q(Q_R200, Q_R100))	
		cgen_asm(vsat1_q(Q_R300, Q_R100))	
		: "=r"(ptr) : "r"(ptr) : "memory");
}


int main (int argc, char **argv)
{
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

	pspDebugScreenInit();
	pspDebugScreenPrintf("VFPU test  --  vfpu_regs0 = %p, vfpu_regs1 = %p\n\n", vfpu_regs0, vfpu_regs1);
	pspDebugScreenPrintf("press O (run VFPU test), Square (trap into breakpoint), X to exit\n\n");

	//vfpu_init();
	vfpu_init1();

	while (1) {
		SceCtrlData pad;

		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_CIRCLE) {
			vfpu_save_regs(vfpu_regs0);
			vfpu_testcase(test_vector);
			vfpu_save_regs(vfpu_regs1);
			vfpu_diff(vfpu_regs0, vfpu_regs1);
		}

		if (pad.Buttons & PSP_CTRL_SQUARE)
			asm("break\n"); /* Cause a break exception, to check that the exception handler works... */

		if (pad.Buttons & PSP_CTRL_CROSS)
			break;

		sceDisplayWaitVblankStart();
	}

	return 0;
}
