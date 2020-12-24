
del *.obj
del *.abs
del *.asm
del *.map
del *.dmp
del *.elf
del makeout

set PTH_BIN=..\..\bin
set RELEASE=TS015
set RELEASE_BIN=%PTH_BIN%\%RELEASE%
del /F /S /Q %RELEASE_BIN%\*
