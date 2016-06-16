@echo off
setlocal EnableDelayedExpansion

rem Changes to the location of make.bat so it can be called from elsewhere
cd /d %~dp0 

SET LIST=
SET PLATFORM=%1
SET HWV_DIR=..\HWV\POP
SET NOLOADER="FALSE"
SET HWV_FILE=
SET HWV_POP=

rem Redo this check later so we can actually check for product.def OR product_loader.def
if NOT EXIST config\%PLATFORM%\product.def goto errorPlatform

if "%2" == "clean" goto CLEAN
if "%2" == "noloader" (
    SET NOLOADER="TRUE"
)

:BUILD
:RUNMAKE

if "%PLATFORM%a"=="a" goto errorPlatform

if NOT EXIST obj\nul mkdir obj

if NOT EXIST obj\%PLATFORM%\nul mkdir obj\%PLATFORM%

if NOT EXIST obj\%PLATFORM%\nessus_release\nul mkdir obj\%PLATFORM%\nessus_release

if NOT EXIST release\%PLATFORM%\nul mkdir release\%PLATFORM%

echo ************************************************************
echo ** Building        : %PLATFORM%
if "%LIST%" == "" echo ** Compiling target: pmic_%PLATFORM%.h
echo ** Object directory: obj/%PLATFORM%
echo.

rem Generate all APP/BOOT target files we will need
..\common\tools\h_to_mak2.exe /m config\%PLATFORM%\product.def obj\%PLATFORM%\product.mak >> nul
..\common\tools\pl.exe ..\common\tools\h_to_d.pl config\%PLATFORM%\product.def > obj\%PLATFORM%\defines_asm.txt
..\common\tools\pl.exe ..\common\tools\h_to_d.pl config\%PLATFORM%\product.def > obj\%PLATFORM%\defines_c.txt
..\common\tools\pl.exe h_to_d_cmd.pl config\%PLATFORM%\product.def > obj\%PLATFORM%\defines_cmd.txt

rem Do the normal build of the BOOT/APP image first so it can export the BOOT/APP image header
..\common\tools\make -s %LIST%

if errorlevel 1 goto bad_build

if %NOLOADER%=="FALSE" (
    call :BUILDBOOTLOADER
)

goto DONE

rem Generate all LOADER target files we will need

:BUILDBOOTLOADER

rem make the build directory if it doesn't exist
if NOT EXIST obj\%PLATFORM%\loader_%PLATFORM%\nul mkdir obj\%PLATFORM%\loader_%PLATFORM%

..\common\tools\h_to_mak2.exe /m config\%PLATFORM%\product_loader.def obj\%PLATFORM%\loader_%PLATFORM%\product.mak >> nul
..\common\tools\pl.exe ..\common\tools\h_to_d.pl config\%PLATFORM%\product_loader.def > obj\%PLATFORM%\loader_%PLATFORM%\defines_asm.txt
..\common\tools\pl.exe ..\common\tools\h_to_d.pl config\%PLATFORM%\product_loader.def > obj\%PLATFORM%\loader_%PLATFORM%\defines_c.txt
..\common\tools\pl.exe h_to_d_cmd.pl config\%PLATFORM%\product_loader.def > obj\%PLATFORM%\loader_%PLATFORM%\defines_cmd.txt

rem Do the Bootloader build next now that we can embed the BOOT/APP image with it
set BOOTLOADER=true
..\common\tools\make -s %LIST%

if errorlevel 1 goto bad_build
goto :eof

:CLEAN
if "%PLATFORM%a"=="a" goto errorPlatform
echo Cleaning %PLATFORM%
..\common\tools\rm -rf obj\%PLATFORM%
..\common\tools\rm -rf release\%PLATFORM%
if "%DEBUG%a"=="a" (
  ..\common\tools\rm -f Pmic-%PLATFORM%.bin
)  
goto DONE

:errorPlatformHwv
echo Platform config directory missing platform_hwv.txt file
goto bad_build

:errorPlatform
echo You must specify a valid platform.
goto bad_build

:DONE
endlocal
EXIT /B 0

:bad_build
echo # -------------
echo # Building %PLATFORM% failed.
echo # -------------
echo.
EXIT /B 1
