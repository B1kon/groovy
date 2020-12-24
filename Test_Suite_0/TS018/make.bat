
set PTH_BIN=..\..\bin
set RELEASE=TS018
@set RELEASE_BIN=%PTH_BIN%\%RELEASE%
set LIBS=..\..\libs
set INC_PATH=..\..\include
set LIB_NAME=mc12705load
set LIB_NM=mc12705load_nmc
set LIB_ARM=mc12705load_arm

set LDS_TEMPLATE_NMC=%INC_PATH%\nmc_template.x
set NMC_LDS_FLAGS=-DUSE_ONLY_IM
set LDS_TEMPLATE_ARM=%INC_PATH%\arm_template.x
set ARM_LDS_FLAGS=-DIS_CLUSTER -DUSE_ONLY_IM

@call ..\create_if_not.bat %PTH_BIN% %RELEASE%
if %goto_END% == 1 goto END

:: ======= ARM
setlocal
::set PATH=%PATH%;D:\BACKUP\disk_1\aasubbot\MC12705_install\SDK\ARMSDK\x-tools\HOST-x86_64-w64-mingw32\arm-nm6408-eabihf\bin
::echo %PATH%
@call ..\set_env_armgnu.bat
echo ======= ARM Cross-compiler
set CROSS_ARM=arm-nm6408-eabihf-
set ARM_GCC=%CROSS_ARM%gcc -mcpu=cortex-a5 -mfpu=vfpv4-d16 -mfloat-abi=hard -marm
set ARM_CFLAGS=-O2 -std=gnu99 -Wl,--nmagic -Wl,--gc-sections
set ARM_DISASM=%CROSS_ARM%objdump -D
set ARM=arm_prog_04

echo ========= ARM Cross-compiler %ARM%
echo ========= "ARM_lds"
 %ARM_GCC% -E -P -o MC12705arm.lds -x c-header %LDS_TEMPLATE_ARM% %ARM_LDS_FLAGS%
for  %%n in (%ARM%) do (
 %ARM_GCC% %ARM_CFLAGS% -Wl,-Map=%%n.map %%n.c -o %RELEASE_BIN%\%%n.elf  -I%INC_PATH% -L %LIBS% -Wl,--whole-archive -l%LIB_ARM% -Wl,--no-whole-archive  -Wl,-T MC12705arm.lds 
 %ARM_DISASM% %RELEASE_BIN%\%%n.elf > %%n.dmp
  echo ARM_LOOP
)
echo =======  END ARM Cross-compiler %ARM%
endlocal
echo ======= HOST Compiler
setlocal
@call "%VS140COMNTOOLS%..\..\vc\vcvarsall.bat" amd64
set TC=tc018_01 tc018_02 tc018_03 tc018_04 

echo on
for  %%g in (%TC%) do (
 cl %%g.cpp -W3 -MT -EHsc  -I%INC_PATH% -link -LIBPATH:%LIBS% %LIB_NAME%.lib -out:%RELEASE_BIN%\%%g.exe
)

copy %LIBS%\mc12705load.dll %RELEASE_BIN%\.
endlocal
:END


