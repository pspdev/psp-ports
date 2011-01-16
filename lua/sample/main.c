// Lua test program by Frank Buss (aka Shine)

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

/* Define the module info section */
PSP_MODULE_INFO("LUATEST", 0, 1, 1);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

/* Define printf, just to make typing easier */
#define printf	pspDebugScreenPrintf

/* Exit callback */
int exit_callback(void)
{
	sceKernelExitGame();

	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", (void *) exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();

	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

int lua_ctrlRead(lua_State *L)
{
	// number of passed arguments
	int argc = lua_gettop(L);
	if (argc != 0) return luaL_error(L, "wrong number of arguments");
	
	SceCtrlData pad;
	sceCtrlReadBufferPositive(&pad, 1); 
	
	// push return value (multiple return values are possible)
	lua_pushnumber(L, (int)pad.Buttons);

	// number of returned arguments
	return 1;
}

#define CHECK_CTRL(name, bit) \
int name(lua_State *L) \
{ \
	int argc = lua_gettop(L); \
	if (argc != 1) return luaL_error(L, "wrong number of arguments"); \
\
	/* get first argument as int */ \
	int argument = luaL_checkint(L, 1); \
\
	/* push result */ \
	lua_pushboolean(L, (argument & bit) == bit); \
	return 1; \
}

CHECK_CTRL(lua_isCtrlSelect, PSP_CTRL_SELECT)
CHECK_CTRL(lua_isCtrlStart, PSP_CTRL_START)
CHECK_CTRL(lua_isCtrlUp, PSP_CTRL_UP)
CHECK_CTRL(lua_isCtrlRight, PSP_CTRL_RIGHT)
CHECK_CTRL(lua_isCtrlDown, PSP_CTRL_DOWN)
CHECK_CTRL(lua_isCtrlLeft, PSP_CTRL_LEFT)
CHECK_CTRL(lua_isCtrlLTrigger, PSP_CTRL_LTRIGGER)
CHECK_CTRL(lua_isCtrlRTrigger, PSP_CTRL_RTRIGGER)
CHECK_CTRL(lua_isCtrlTriangle, PSP_CTRL_TRIANGLE)
CHECK_CTRL(lua_isCtrlCircle, PSP_CTRL_CIRCLE)
CHECK_CTRL(lua_isCtrlCross, PSP_CTRL_CROSS)
CHECK_CTRL(lua_isCtrlSquare, PSP_CTRL_SQUARE)
CHECK_CTRL(lua_isCtrlHome, PSP_CTRL_HOME)
CHECK_CTRL(lua_isCtrlHold, PSP_CTRL_HOLD)

int lua_waitVblankStart(lua_State *L)
{
	if (lua_gettop(L) != 0) return luaL_error(L, "wrong number of arguments");
	sceDisplayWaitVblankStart();
	return 0;
}

// print all arguments, converted to strings
int lua_print(lua_State *L)
{
	int argc = lua_gettop(L);
	int n;
	for (n=1; n <= argc; n++) printf("%s\n", lua_tostring(L, n));
	return 0;
}

int main(int argc, char** argv)
{
	pspDebugScreenInit();
	SetupCallbacks();

	// script file must be in the same directory where the program started
	const char* scriptName = "script.lua";
	char* scriptFilename = (char*) malloc(strlen(argv[0]) + strlen(scriptName));
	strcpy(scriptFilename, argv[0]);
	char* end = strrchr(scriptFilename, '/');
	end++;
	*end = 0;
	strcat(scriptFilename, scriptName);

	// init Lua and load all libraries
	lua_State *L = lua_open();
	luaL_openlibs(L);
	
	// register our own functions
	lua_register(L, "ctrlRead", lua_ctrlRead);
	lua_register(L, "isCtrlSelect", lua_isCtrlSelect);
	lua_register(L, "isCtrlStart", lua_isCtrlStart);
	lua_register(L, "isCtrlUp", lua_isCtrlUp);
	lua_register(L, "isCtrlRight", lua_isCtrlRight);
	lua_register(L, "isCtrlDown", lua_isCtrlDown);
	lua_register(L, "isCtrlLeft", lua_isCtrlLeft);
	lua_register(L, "isCtrlUp", lua_isCtrlUp);
	lua_register(L, "isCtrlLTrigger", lua_isCtrlLTrigger);
	lua_register(L, "isCtrlRTrigger", lua_isCtrlRTrigger);
	lua_register(L, "isCtrlTriangle", lua_isCtrlTriangle);
	lua_register(L, "isCtrlCircle", lua_isCtrlCircle);
	lua_register(L, "isCtrlCross", lua_isCtrlCross);
	lua_register(L, "isCtrlSquare", lua_isCtrlSquare);
	lua_register(L, "isCtrlHome", lua_isCtrlHome);
	lua_register(L, "isCtrlHold", lua_isCtrlHold);
	lua_register(L, "print", lua_print);
	lua_register(L, "waitVblankStart", lua_waitVblankStart);

	// load script
	int status = luaL_loadfile(L, scriptFilename);
	
	// call script
	if (status == 0) status = lua_pcall(L, 0, LUA_MULTRET, 0);
	
	// show error, if any
	if (status != 0) {
		printf("error: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1);  // remove error message
	}
	
	// cleanup
	lua_close(L);
	free(scriptFilename);

	sceKernelExitGame();

	return 0;
}
