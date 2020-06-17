
SET PATH_BACK=%PATH%

rem --- nsd.lib ---
cd nsd
call _clean
cd..

rem --- nsc64.exe ---
cd nsc64\
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
MSBuild -t:Clean -p:Configuration=Release;Platform="x64"
cd..
SET PATH=%PATH_BACK%

rem --- rom.bin ---
cd rom
call _clean
cd..
