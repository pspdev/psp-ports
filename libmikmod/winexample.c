#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mikmod.h"


extern int _mm_errno;
extern BOOL _mm_critical;
extern char *_mm_errmsg[];


void my_error_handler(void)
{
	printf("_mm_critical %d\n", MikMod_critical);
	printf("_mm_errno %d\n", MikMod_errno);
	printf("%s\n", MikMod_strerror(MikMod_errno));
	return;
}

extern UWORD md_mode;
extern UBYTE md_reverb;
extern UBYTE md_pansep;
// ]]

int main(int argc, char** argv)
{

// [[ Mikmod-specific
	int maxchan = 128;
	
	MODULE *mf = NULL; // for mod
	SAMPLE *sf = NULL; // for wav
	int voice = 0; 	   // for wav
// ]]
	int pan = 127;
	int vol = 127;
	int freq = 22000;

	MODULE *module;

    /* register all the drivers */
    MikMod_RegisterAllDrivers();

    /* register all the module loaders */
    MikMod_RegisterAllLoaders();

    /* initialize the library */
    md_mode |= DMODE_SOFT_MUSIC;
    if (MikMod_Init("")) {
        fprintf(stderr, "Could not initialize sound, reason: %s\n",
                MikMod_strerror(MikMod_errno));
        return;
    }

    /* load module */
    module = Player_Load("./music.xm", 64, 0);
    if (module) {
        /* start module */
        Player_Start(module);

        while (Player_Active()) {
            /* we're playing */
            usleep(10000);
            MikMod_Update();
        }

        Player_Stop();
        Player_Free(module);
    } else
        fprintf(stderr, "Could not load module, reason: %s\n",
                MikMod_strerror(MikMod_errno));

    /* give up */
    MikMod_Exit();
}
