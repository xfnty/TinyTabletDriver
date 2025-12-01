@echo off
setlocal enableextensions enabledelayedexpansion
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do set esc=%%b

set project_name=TinyWacomDriver

set root_dir=%~dp0
set output_dir=%~dp0out\
set dist_dir=%~dp0dist\
set temp_dir=%TEMP%\%project_name%\

for %%a in (%*) do set %%a=1
if "%1"=="" (call :PrintUsage && exit /b 0)
if "%clean%"=="1" (call :Clean  || exit /b 1)
if "%build%"=="1" (call :Build || exit /b 1)
if "%run%"=="1"   (call :Run   || exit /b 1)
if "%dist%"=="1"  (call :Dist  || exit /b 1)
exit /b 0

:PrintUsage
echo Usage: project [commands]
echo.
echo Available commands:
echo   clean   Delete intermediate and output files.
echo   build   Build the project.
echo   run     Run the executable.
echo   dist    Package project for distribution.
echo.
echo Temporary  directory: %temp_dir%
exit /b 0

:Build
mkdir "%temp_dir%." "%output_dir%." > nul 2>&1
set "compile_start=%time%"
set unity=%temp_dir%unity
set cflags=/nologo /std:c11 /Wall /wd4820 /wd5045 /wd4711 /Z7 /O2
set lflags=/debug /subsystem:windows /INCREMENTAL:NO /entry:_start
set oflags=/Fo:"%unity%.obj" /Fe:"%output_dir%%project_name%.exe"
set links=kernel32.lib winusb.lib hid.lib setupapi.lib ucrt.lib vcruntime.lib
set command=cl %cflags% "%unity%.c" %links% %oflags% /link %lflags%
echo // %command%> "%unity%.c"
for /r "%root_dir%src" %%a in (*.c) do echo #include "%%a">> "%unity%.c"
%command% || exit /b 1
set "compile_end=%time%"
for /f "tokens=1-4 delims=:.," %%a in ("%compile_start%") do set start_h=%%a&set /a start_m=100%%b %% 100&set /a start_s=100%%c %% 100&set /a start_ms=100%%d %% 100
for /f "tokens=1-4 delims=:.," %%a in ("%compile_end%") do set end_h=%%a&set /a end_m=100%%b %% 100&set /a end_s=100%%c %% 100&set /a end_ms=100%%d %% 100
set /a hours=%end_h%-%start_h%
set /a mins=%end_m%-%start_m%
set /a secs=%end_s%-%start_s%
set /a ms=%end_ms%-%start_ms%
if %ms% lss 0 set /a secs = %secs% - 1 & set /a ms = 100%ms%
if %secs% lss 0 set /a mins = %mins% - 1 & set /a secs = 60%secs%
if %mins% lss 0 set /a hours = %hours% - 1 & set /a mins = 60%mins%
if %hours% lss 0 set /a hours = 24%hours%
if 1%ms% lss 100 set ms=0%ms%
set /a totalsecs = %hours%*3600 + %mins%*60 + %secs%
for %%g in (%output_dir%%project_name%.exe) do ^
for /f "tokens=1,2" %%s in ('robocopy %%~dpg. %%~dpg. %%~nxg /l /nocopy /is /njh /njs /ndl /nc') do (
    echo Compiled down to !esc![1;96m%%s!esc![0m bytes in !esc![1;96m%totalsecs%.%ms%!esc![0m seconds.
)
exit /b 0

:Run
start "" /D "%output_dir%." /wait /b "%output_dir%%project_name%.exe"
if not !errorlevel! == 0 (
    echo Exited with code !esc![1;91m!errorlevel!!esc![0m
    exit /b 1
)
exit /b 0

:Dist
mkdir "%dist_dir%." 2> nul
copy /Y "C:\Windows\System32\vcruntime140.dll" "%output_dir%." 1> nul
tar -acf "%dist_dir%%project_name%.zip" -C "%output_dir%." * || exit /b 1
for %%g in (%dist_dir%%project_name%.zip) do ^
for /f "tokens=1,2" %%s in ('robocopy %%~dpg. %%~dpg. %%~nxg /l /nocopy /is /njh /njs /ndl /nc') do (
    echo Package size is !esc![1;96m%%s!esc![0m bytes.
)
exit /b 0

:Clean
rmdir /s /q "%output_dir%." "%temp_dir%." "%dist_dir%." 2> nul
exit /b 0
