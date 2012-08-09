
rem --- nsd.lib ---
cd nsd
nmake
copy *.lib ..\..\lib\
cd..

rem --- nsc.exe ---
cd nsc
cd..

rem --- nsdl.chm ---
cd help
copy *.chm ..\..\doc\
cd..

rem œœœ for Debug œœœ
cd _Debug
nmake
cd..

cd _nsf
nmake
cd..

