@echo off

set CLEANBUILD=
if "%1" == "clean" set CLEANBUILD=%1

if exist release\* del /q release\*

SET PLATFORM=kelowna_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=kelowna_rev1_moccasin
SET PMIC=rocket
call :dobuild

SET PLATFORM=kelowna_rev2
SET PMIC=rocket
call :dobuild

SET PLATFORM=kelowna_rev2_moccasin
SET PMIC=rocket
call :dobuild

SET PLATFORM=boldr016_rev2
SET PMIC=rocket
call :dobuild

SET PLATFORM=boldr016_rev3
SET PMIC=rocket
call :dobuild

SET PLATFORM=boldr016_rev4
SET PMIC=rocket
call :dobuild

SET PLATFORM=boldr016u768_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=boldr016u768_rev2
SET PMIC=rocket
call :dobuild

SET PLATFORM=cooper_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=rutherford_a_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=rutherford_b_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=eshel_eno_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=eshel_eno_rev2
SET PMIC=rocket
call :dobuild

SET PLATFORM=curver032_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=curver032_rev2
SET PMIC=rocket
call :dobuild

SET PLATFORM=curver032_rev3
SET PMIC=rocket
call :dobuild

SET PLATFORM=curver032_rev4
SET PMIC=rocket
call :dobuild

SET PLATFORM=noble_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=tedeschi_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=colt_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=rubicon_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=chicago_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=roundrock_rev2
SET PMIC=rocket
call :dobuild

SET PLATFORM=roundrock_rev3
SET PMIC=rocket
call :dobuild

SET PLATFORM=curver049_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=curver050_rev1
SET PMIC=rocket
call :dobuild

SET PLATFORM=curver050_rev1_LDO11
SET PMIC=rocket
call :dobuild

rem SET PLATFORM=roundrock_rev2_lagavulin
rem SET PMIC=lagavulin
rem call :dobuild

rem SET PLATFORM=coolriver_rev1
rem SET PMIC=lagavulin
rem call :dobuild

goto done

:dobuild

echo Building %PLATFORM% %CLEANBUILD%
call make.bat %PLATFORM% %CLEANBUILD%
if errorlevel 1 goto error

if "%CLEANBUILD%" == "clean" goto :eof

if not exist release\%PLATFORM%\%PMIC%.map goto error
if not exist release\%PLATFORM%\%PMIC%.out goto error
if not exist release\%PLATFORM%\%PMIC%_%PLATFORM%.h goto error
if not exist release\%PLATFORM%\%PMIC%_%PLATFORM%_loader.map goto error
if not exist release\%PLATFORM%\%PMIC%_%PLATFORM%_loader.out goto error
if not exist release\%PLATFORM%\%PMIC%_%PLATFORM%_loader.ahx goto error

goto :eof

:error
echo Building %PLATFORM% failed.
echo. > release\failed-%PLATFORM%
goto :eof

:success
echo Builds successful.
goto :eof

:done
echo Build done.
