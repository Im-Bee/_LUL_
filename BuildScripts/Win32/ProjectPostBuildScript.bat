echo off

SET outDir=%1
SET libsDir=%2

Rem echo %outDir% %libsDir%_LUL_Base\_LUL_Base_x64.dll
xcopy /y %libsDir%\_LUL_Base\_LUL_Base_x64.dll %outDir%\