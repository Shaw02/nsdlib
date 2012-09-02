
rem --- nsd.lib ---
cd nsd
nmake
copy *.lib ..\..\lib\
cd..

rem --- nsc.exe ---
cd nsc
cd release
copy *.exe ..\..\..\bin\
cd..
cd..

rem --- rom.bin ---
cd rom
nmake
copy *.bin ..\..\bin\
cd..

rem --- nsdl.chm ---
cd help
copy *.chm ..\..\doc\
cd..
