pkzip -e0 zzipself.zip zzipself.exe
pkzip -a zzipself.zip zzipself.txt
zzipsetstub.exe zzipself.zip zzipself.exe
del zzipself.exe
ren zzipself.zip zzipself.exe

REM testrun:
echo now testing zzipself.exe
zzipself.exe zzipself.txt

