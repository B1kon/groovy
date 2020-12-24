
del *.obj
del *.abs
del *.asm
del *.map
del *.elf
del *.dmp
del makeout

set PTH_BIN=..\..\bin
set RELEASE=TS101
set RELEASE_BIN=%PTH_BIN%\%RELEASE%
del /F /S /Q %RELEASE_BIN%\*
