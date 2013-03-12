@echo off
..\..\bin\nsc -n %~n1
if %errorlevel% neq 0	pause&goto end
if not exist %~n1.mml	pause&goto end
start %~n1.nsf
:end
