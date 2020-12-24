
set PTH_BIN=..\..\bin
set RELEASE=TS002
@set RELEASE_BIN=%PTH_BIN%\%RELEASE%
set LIBS=..\..\libs
set INCLD=..\..\include
set LIB_NAME=mc12705load

@call ..\create_if_not.bat %PTH_BIN% %RELEASE%
if %goto_END% == 1 goto END

:: ======= HOST Compiler
setlocal
@call "%VS140COMNTOOLS%..\..\vc\vcvarsall.bat" amd64
set TC=tc002_01

echo on
for  %%g in (%TC%) do (
 cl %%g.cpp -W3 -MT -EHsc  -I%INCLD% -link -LIBPATH:%LIBS% %LIB_NAME%.lib -out:%RELEASE_BIN%\%%g.exe
)

copy %LIBS%\mc12705load.dll %RELEASE_BIN%\.
endlocal
:END


