path %path%;..\..\bin
del buildlog.txt
del errlog.txt
del comlog.txt
make -k clean all >buildlog.txt 2>&1

if %errorlevel% equ 0	goto end
start errlog.txt
:end
