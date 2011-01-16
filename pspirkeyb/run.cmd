rem
rem This is a simple hack to autostart the elf from Visual Studio
rem 
rem you need windows nt NETCAT (http://www.vulnwatch.org/netcat/)
rem

set SLEEP=C:\devkitPro\msys\bin\sleep.exe
set NETCAT=C:\devkitPro\devkitPSP\bin\nc.exe
set HOST=localhost
set PORT=10000
echo "reset" | %NETCAT% %HOST% %PORT%
%SLEEP% 8
echo "host0:/pspirkeyb/irkeybtest/irkeybtest.elf" | %NETCAT% %HOST% %PORT%
%SLEEP% 0
