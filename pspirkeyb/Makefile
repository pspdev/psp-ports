all:
	$(MAKE) -C libpspirkeyb all
	$(MAKE) -C irkeybtest all
	$(MAKE) -C asciidemo all
	$(MAKE) -C pspirkeybprx exp_symbols
	$(MAKE) -C libpspirkeybprx all
	$(MAKE) -C asciidemoprx all
	$(MAKE) -C IRDAMon all

clean:
	$(MAKE) -C libpspirkeyb clean
	$(MAKE) -C irkeybtest clean
	$(MAKE) -C asciidemo clean
	$(MAKE) -C pspirkeybprx clean
	$(MAKE) -C libpspirkeybprx clean
	$(MAKE) -C asciidemoprx clean
	$(MAKE) -C IRDAMon clean
	rm -rf release

release: all
	mkdir -p release
	mkdir -p release/seplugins
	mkdir -p release/psp/game/asciidemo
	mkdir -p release/psp/game/asciidemoprx
	cp pspirkeybprx/irkeyb.prx release/seplugins
	cp -ar libpspirkeyb/keymap release/seplugins
	cp libpspirkeyb/config/pspirkeyb.ini release/seplugins
	cp asciidemo/EBOOT.PBP release/psp/game/asciidemo
	cp asciidemoprx/EBOOT.PBP release/psp/game/asciidemoprx

PSPSDK=$(shell psp-config --pspsdk-path)

install: all
	$(MAKE) -C libpspirkeyb install
	install -d $(DESTDIR)$(PSPSDK)/samples/irkeyb
	install -d $(DESTDIR)$(PSPSDK)/samples/irkeyb/keymap
	cp libpspirkeyb/keymap/*.ini $(DESTDIR)$(PSPSDK)/samples/irkeyb/keymap
	cp libpspirkeyb/keymap/README.txt $(DESTDIR)$(PSPSDK)/samples/irkeyb/keymap
	cp libpspirkeyb/config/pspirkeyb.ini $(DESTDIR)$(PSPSDK)/samples/irkeyb
	cp asciidemo/main.c $(DESTDIR)$(PSPSDK)/samples/irkeyb
	cp asciidemo/Makefile.sdk $(DESTDIR)$(PSPSDK)/samples/irkeyb/Makefile

uninstall:
	$(MAKE) -C libpspirkeyb install
	rm -rf $(PSPSDK)/samples/irkeyb
