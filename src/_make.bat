

SET PATH_BACK=%PATH%

rem --- nsd.lib ---
cd nsd
call _make
copy *.lib ..\..\lib\
cd..

rem --- nsc.exe ---
rem call "C:\Program Files (x86)\Microsoft Visual Studio .NET 2003\Common7\Tools\vsvars32.bat"
cd nsc
rem devenv nsc.sln /build Release
cd release
copy *.exe ..\..\..\bin\
cd..
cd..
SET PATH=%PATH_BACK%

rem --- nsc64.exe ---
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
cd nsc64\
MSBuild -t:Build -p:Configuration=Release;Platform="x64"
cd x64\Release
copy *.exe ..\..\..\..\bin\
cd..
cd..
cd..
SET PATH=%PATH_BACK%

rem --- rom.bin ---
cd rom
call _make
copy *.bin ..\..\bin\
cd..

rem --- nsdl.chm ---
cd help
copy *.chm ..\..\doc\
cd..
