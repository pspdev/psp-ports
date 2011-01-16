
SRC_FILES += \
	$(CODEDIR)/botlib/be_aas_bspq3.c \
	$(CODEDIR)/botlib/be_aas_cluster.c \
	$(CODEDIR)/botlib/be_aas_debug.c \
	$(CODEDIR)/botlib/be_aas_entity.c \
	$(CODEDIR)/botlib/be_aas_file.c \
	$(CODEDIR)/botlib/be_aas_main.c \
	$(CODEDIR)/botlib/be_aas_move.c \
	$(CODEDIR)/botlib/be_aas_optimize.c \
	$(CODEDIR)/botlib/be_aas_reach.c \
	$(CODEDIR)/botlib/be_aas_route.c \
	$(CODEDIR)/botlib/be_aas_routealt.c \
	$(CODEDIR)/botlib/be_aas_sample.c \
	$(CODEDIR)/botlib/be_ai_char.c \
	$(CODEDIR)/botlib/be_ai_chat.c \
	$(CODEDIR)/botlib/be_ai_gen.c \
	$(CODEDIR)/botlib/be_ai_goal.c \
	$(CODEDIR)/botlib/be_ai_move.c \
	$(CODEDIR)/botlib/be_ai_weap.c \
	$(CODEDIR)/botlib/be_ai_weight.c \
	$(CODEDIR)/botlib/be_ea.c \
	$(CODEDIR)/botlib/be_interface.c \
	$(CODEDIR)/botlib/l_crc.c \
	$(CODEDIR)/botlib/l_libvar.c \
	$(CODEDIR)/botlib/l_log.c \
	$(CODEDIR)/botlib/l_memory.c \
	$(CODEDIR)/botlib/l_precomp.c \
	$(CODEDIR)/botlib/l_script.c \
	$(CODEDIR)/botlib/l_struct.c


SRC_FILES += \
	$(CODEDIR)/client/cl_cgame.c \
	$(CODEDIR)/client/cl_cin.c \
	$(CODEDIR)/client/cl_console.c \
	$(CODEDIR)/client/cl_input.c \
	$(CODEDIR)/client/cl_keys.c \
	$(CODEDIR)/client/cl_main.c \
	$(CODEDIR)/client/cl_net_chan.c \
	$(CODEDIR)/client/cl_parse.c \
	$(CODEDIR)/client/cl_scrn.c \
	$(CODEDIR)/client/cl_ui.c \
	$(CODEDIR)/client/snd_adpcm.c \
	$(CODEDIR)/client/snd_dma.c \
	$(CODEDIR)/client/snd_mem.c \
	$(CODEDIR)/client/snd_mix.c \
	$(CODEDIR)/client/snd_wavelet.c


SRC_FILES += \
	$(CODEDIR)/game/ai_chat.c \
	$(CODEDIR)/game/ai_cmd.c \
	$(CODEDIR)/game/ai_dmnet.c \
	$(CODEDIR)/game/ai_dmq3.c \
	$(CODEDIR)/game/ai_main.c \
	$(CODEDIR)/game/ai_team.c \
	$(CODEDIR)/game/ai_vcmd.c \
	$(CODEDIR)/game/bg_misc.c \
	$(CODEDIR)/game/bg_pmove.c \
	$(CODEDIR)/game/bg_slidemove.c \
	$(CODEDIR)/game/g_active.c \
	$(CODEDIR)/game/g_arenas.c \
	$(CODEDIR)/game/g_bot.c \
	$(CODEDIR)/game/g_client.c \
	$(CODEDIR)/game/g_cmds.c \
	$(CODEDIR)/game/g_combat.c \
	$(CODEDIR)/game/g_items.c \
	$(CODEDIR)/game/g_main.c \
	$(CODEDIR)/game/g_mem.c \
	$(CODEDIR)/game/g_misc.c \
	$(CODEDIR)/game/g_missile.c \
	$(CODEDIR)/game/g_mover.c \
	$(CODEDIR)/game/g_session.c \
	$(CODEDIR)/game/g_spawn.c \
	$(CODEDIR)/game/g_svcmds.c \
	$(CODEDIR)/game/g_syscalls.c \
	$(CODEDIR)/game/g_target.c \
	$(CODEDIR)/game/g_team.c \
	$(CODEDIR)/game/g_trigger.c \
	$(CODEDIR)/game/g_utils.c \
	$(CODEDIR)/game/g_weapon.c \
	$(CODEDIR)/game/q_math.c \
	$(CODEDIR)/game/q_shared.c

# doesn't seems to be used anywhere... see also ./code/unix/ChangeLog
#	$(CODEDIR)/game/g_rankings.c \
#	$(CODEDIR)/game/bg_lib.c \


SRC_FILES += \
	$(CODEDIR)/jpeg-6/jcapimin.c \
	$(CODEDIR)/jpeg-6/jccoefct.c \
	$(CODEDIR)/jpeg-6/jccolor.c \
	$(CODEDIR)/jpeg-6/jcdctmgr.c \
	$(CODEDIR)/jpeg-6/jchuff.c \
	$(CODEDIR)/jpeg-6/jcinit.c \
	$(CODEDIR)/jpeg-6/jcmainct.c \
	$(CODEDIR)/jpeg-6/jcmarker.c \
	$(CODEDIR)/jpeg-6/jcmaster.c \
	$(CODEDIR)/jpeg-6/jcomapi.c \
	$(CODEDIR)/jpeg-6/jcparam.c \
	$(CODEDIR)/jpeg-6/jcphuff.c \
	$(CODEDIR)/jpeg-6/jcprepct.c \
	$(CODEDIR)/jpeg-6/jcsample.c \
	$(CODEDIR)/jpeg-6/jctrans.c \
	$(CODEDIR)/jpeg-6/jdapimin.c \
	$(CODEDIR)/jpeg-6/jdapistd.c \
	$(CODEDIR)/jpeg-6/jdatadst.c \
	$(CODEDIR)/jpeg-6/jdatasrc.c \
	$(CODEDIR)/jpeg-6/jdcoefct.c \
	$(CODEDIR)/jpeg-6/jdcolor.c \
	$(CODEDIR)/jpeg-6/jddctmgr.c \
	$(CODEDIR)/jpeg-6/jdhuff.c \
	$(CODEDIR)/jpeg-6/jdinput.c \
	$(CODEDIR)/jpeg-6/jdmainct.c \
	$(CODEDIR)/jpeg-6/jdmarker.c \
	$(CODEDIR)/jpeg-6/jdmaster.c \
	$(CODEDIR)/jpeg-6/jdmerge.c \
	$(CODEDIR)/jpeg-6/jdphuff.c \
	$(CODEDIR)/jpeg-6/jdpostct.c \
	$(CODEDIR)/jpeg-6/jdsample.c \
	$(CODEDIR)/jpeg-6/jdtrans.c \
	$(CODEDIR)/jpeg-6/jerror.c \
	$(CODEDIR)/jpeg-6/jfdctflt.c \
	$(CODEDIR)/jpeg-6/jfdctfst.c \
	$(CODEDIR)/jpeg-6/jfdctint.c \
	$(CODEDIR)/jpeg-6/jidctflt.c \
	$(CODEDIR)/jpeg-6/jidctfst.c \
	$(CODEDIR)/jpeg-6/jidctint.c \
	$(CODEDIR)/jpeg-6/jidctred.c \
	$(CODEDIR)/jpeg-6/jload.c \
	$(CODEDIR)/jpeg-6/jmemmgr.c \
	$(CODEDIR)/jpeg-6/jmemnobs.c \
	$(CODEDIR)/jpeg-6/jquant1.c \
	$(CODEDIR)/jpeg-6/jquant2.c \
	$(CODEDIR)/jpeg-6/jutils.c

# unused:
#	$(CODEDIR)/jpeg-6/jcapistd.c \
#	$(CODEDIR)/jpeg-6/jmemansi.c \
#	$(CODEDIR)/jpeg-6/jmemdos.c \
#	$(CODEDIR)/jpeg-6/jmemname.c \
# unused: the JPEG transcoding command line utility
#	$(CODEDIR)/jpeg-6/jpegtran.c \


SRC_FILES += \
	$(CODEDIR)/qcommon/cm_load.c \
	$(CODEDIR)/qcommon/cm_patch.c \
	$(CODEDIR)/qcommon/cm_polylib.c \
	$(CODEDIR)/qcommon/cm_test.c \
	$(CODEDIR)/qcommon/cm_trace.c \
	$(CODEDIR)/qcommon/cmd.c \
	$(CODEDIR)/qcommon/common.c \
	$(CODEDIR)/qcommon/cvar.c \
	$(CODEDIR)/qcommon/files.c \
	$(CODEDIR)/qcommon/huffman.c \
	$(CODEDIR)/qcommon/md4.c \
	$(CODEDIR)/qcommon/msg.c \
	$(CODEDIR)/qcommon/net_chan.c \
	$(CODEDIR)/qcommon/unzip.c \
	$(CODEDIR)/qcommon/vm.c \
	$(CODEDIR)/qcommon/vm_interpreted.c \

## we're using the interpreted for now... 
#	$(CODEDIR)/qcommon/vm_ppc.c \
#	$(CODEDIR)/qcommon/vm_ppc_new.c \
#	$(CODEDIR)/qcommon/vm_x86.c


SRC_FILES += \
	$(CODEDIR)/renderer/tr_animation.c  \
	$(CODEDIR)/renderer/tr_backend.c  \
	$(CODEDIR)/renderer/tr_bsp.c  \
	$(CODEDIR)/renderer/tr_cmds.c  \
	$(CODEDIR)/renderer/tr_curve.c  \
	$(CODEDIR)/renderer/tr_flares.c  \
	$(CODEDIR)/renderer/tr_font.c  \
	$(CODEDIR)/renderer/tr_image.c  \
	$(CODEDIR)/renderer/tr_init.c  \
	$(CODEDIR)/renderer/tr_light.c  \
	$(CODEDIR)/renderer/tr_main.c  \
	$(CODEDIR)/renderer/tr_marks.c  \
	$(CODEDIR)/renderer/tr_mesh.c  \
	$(CODEDIR)/renderer/tr_model.c  \
	$(CODEDIR)/renderer/tr_noise.c  \
	$(CODEDIR)/renderer/tr_scene.c  \
	$(CODEDIR)/renderer/tr_shade.c  \
	$(CODEDIR)/renderer/tr_shade_calc.c  \
	$(CODEDIR)/renderer/tr_shader.c  \
	$(CODEDIR)/renderer/tr_shadows.c  \
	$(CODEDIR)/renderer/tr_sky.c  \
	$(CODEDIR)/renderer/tr_surface.c  \
	$(CODEDIR)/renderer/tr_world.c


SRC_FILES += \
	$(CODEDIR)/server/sv_bot.c \
	$(CODEDIR)/server/sv_ccmds.c \
	$(CODEDIR)/server/sv_client.c \
	$(CODEDIR)/server/sv_game.c \
	$(CODEDIR)/server/sv_init.c \
	$(CODEDIR)/server/sv_main.c \
	$(CODEDIR)/server/sv_net_chan.c \
	$(CODEDIR)/server/sv_snapshot.c \
	$(CODEDIR)/server/sv_world.c

# XXX ?!?!
#	$(CODEDIR)/server/sv_rankings.c \


SRC_FILES += \
	generic/input.c \
	generic/main.c \
	generic/glimp.c \
	generic/net.c \
	generic/snddma.c

