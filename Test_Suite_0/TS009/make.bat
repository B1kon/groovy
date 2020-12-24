
set PTH_BIN=..\..\bin
set RELEASE=TS009
@set RELEASE_BIN=%PTH_BIN%\%RELEASE%
set LIBS=..\..\libs
set INC_PATH=..\..\include
set LIB_NAME=mc12705load
set LIB_NM=mc12705load_nmc
set LIB_ARM=mc12705load_arm

set LDS_TEMPLATE_NMC=%INC_PATH%\nmc_template.x
set NMC_LDS_FLAGS=-DUSE_ONLY_IM

@call ..\create_if_not.bat %PTH_BIN% %RELEASE%
if %goto_END% == 1 goto END

echo ======= NM
setlocal
::set NMC_GCC_TOOLPATH=C:\Program Files\Module\NMC-SDK
::@call "%NMC_GCC_TOOLPATH%\nmc4cmd.bat"
@call ..\set_env_nmcgnu.bat
echo ======= NMC Cross-compiler
set NMC_GCC=nmc-gcc -mgas
set NMC_CFLAGS=-O2 -std=gnu99 -Wl,--nmagic
set MNC2LD=-Wl,--whole-archive -l%LIB_NM% -Wl,--no-whole-archive -L %LIBS% -L "%NMC_GCC_TOOLPATH%\\nmc4-ide\\lib"
set NMC_DISASM="nmc-objdump -D"
set NM=nm_prog
popd
echo ======= NMC Cross-compiler %NM%
echo on
echo ========= "NM_lds"
 %NMC_GCC% -E -P -o MC12705nm.lds -x c-header %LDS_TEMPLATE_NMC% %NMC_LDS_FLAGS%
for  %%n in (%NM%) do (
 %NMC_GCC% %NMC_CFLAGS% -Wl,-Map=%%n.map %%n.c -o %RELEASE_BIN%\%%n.abs  -I%INC_PATH% %MNC2LD% -Wl,-T MC12705nm.lds
echo NM_LOOP
)
endlocal
echo =======  END NMC Cross-compiler %NM%


echo ======= HOST Compiler
setlocal
@call "%VS140COMNTOOLS%..\..\vc\vcvarsall.bat" amd64
set TC=tc009_01 tc009_02 tc009_03 tc009_04 tc009_04_process

echo on
for  %%g in (%TC%) do (
 cl %%g.cpp -W3 -MT -EHsc  -I%INC_PATH% -link -LIBPATH:%LIBS% %LIB_NAME%.lib -out:%RELEASE_BIN%\%%g.exe
)

copy %LIBS%\mc12705load.dll %RELEASE_BIN%\.
endlocal
:END


