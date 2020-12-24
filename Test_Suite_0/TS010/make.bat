
set PTH_BIN=..\..\bin
set RELEASE=TS010
@set RELEASE_BIN=%PTH_BIN%\%RELEASE%
set LIBS=..\..\libs
set INC_PATH=..\..\include
set LIB_NAME=mc12705load
set LIB_NM=mc12705load_nmc
set LIB_ARM=mc12705load_arm

@call ..\create_if_not.bat %PTH_BIN% %RELEASE%
if %goto_END% == 1 goto END

echo ======= NM
setlocal
::set NMC_GCC_TOOLPATH=C:\Program Files\Module\NMC-SDK
::@call "%NMC_GCC_TOOLPATH%\nmc4cmd.bat"
@call ..\set_env_nmcgnu.bat
echo ======= NMC Cross-compiler
set NMC_GCC=nmc-gcc -mgas
::set NMC_CFLAGS=-O2 -std=gnu99 -Wl,--nmagic
set NMC_CFLAGS=--entry=_start -O2 -std=gnu99 -Wl,--nmagic
set MNC2LD=-Wl,--whole-archive -l%LIB_NM% -Wl,--no-whole-archive -L %LIBS% -L "%NMC_GCC_TOOLPATH%\\nmc4-ide\\lib"
set NMC_DISASM="nmc-objdump -D"
set NM=nm_prog_05
set NM_protect=nm_prog_0x0
set NM_AVL=0 4 C D
popd
echo ======= NMC Cross-compiler %NM%
::echo on
for  %%n in (%NM%) do (
 %NMC_GCC% %NMC_CFLAGS% -Wl,-Map=%%n.map %%n.c -o %RELEASE_BIN%\%%n.abs  -I%INC_PATH% %MNC2LD% -Wl,-T MC12705nm.lds
)
echo ======= NMC Cross-compiler NM_protect %NM_protect%
for  %%n in (%NM_protect%) do (
 %NMC_GCC% %NMC_CFLAGS% -Wl,-Map=%%n_REG_protect.map %%n.c -o %RELEASE_BIN%\%%n_REG_protect.abs  -I%INC_PATH% %MNC2LD% -Wl,-T MC12705nm_REG_protect.lds
 %NMC_GCC% %NMC_CFLAGS% -Wl,-Map=%%n_absent.map %%n.c -o %RELEASE_BIN%\%%n_absent.abs  -I%INC_PATH% %MNC2LD% -Wl,-T MC12705nm_absent.lds
)
echo =======  END NMC Cross-compiler %NM_protect%

echo ======= NMC Cross-compiler NM_AVL %NM_AVL%
for  %%n in (%NM_AVL%) do (
  %NMC_GCC% %NMC_CFLAGS% -Wl,-Map=nm_prog_%%n.map nm_prog.c -o %RELEASE_BIN%\nm_prog_%%n.abs  -I%INC_PATH% %MNC2LD% -Wl,-T MC12705nm_%%n.lds
)
echo =======  END NMC Cross-compiler %NM_AVLt%
endlocal

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
set ARM=arm_prog_05
set ARM_protect=_REG_protect _CCPU_0x3_protect _absent
set ARM_PC_AVL=PC_0 PC_10 PC_30
set ARM_CCPU_AVL=CCPU_0 CCPU_10
set ARM_PC_AVL_OFFSET=PC_0_OFFSET
set ARM_CCPU_AVL_OFFSET=CCPU_10_OFFSET

echo ========= ARM Cross-compiler %ARM%
for  %%n in (%ARM%) do (
 %ARM_GCC% %ARM_CFLAGS% -Wl,-Map=%%n.map %%n.c -o %RELEASE_BIN%\%%n.elf  -I%INC_PATH% -L %LIBS% -Wl,--whole-archive -l%LIB_ARM% -Wl,--no-whole-archive  -Wl,-T MC12705arm.lds 
 %ARM_DISASM% %RELEASE_BIN%\%%n.elf > %%n.dmp
)
echo =======  END ARM Cross-compiler %ARM%
echo ========= ARM Cross-compiler ARM_protect %ARM_protect%
for  %%n in (%ARM_protect%) do (
 %ARM_GCC% %ARM_CFLAGS% -Wl,-Map=arm_prog_0x0%%n.map arm_prog_0x0.c -o %RELEASE_BIN%\arm_prog_0x0%%n.elf  -I%INC_PATH% -L %LIBS% -Wl,--whole-archive -l%LIB_ARM% -Wl,--no-whole-archive  -Wl,-T MC12705arm%%n.lds 
 %ARM_DISASM% %RELEASE_BIN%\arm_prog_0x0%%n.elf > arm_prog_0x0%%n.dmp
)
echo =======  END ARM Cross-compiler %ARM_protect%
echo ========= ARM Cross-compiler ARM_PC_AVL %ARM_PC_AVL%
for  %%n in (%ARM_PC_AVL%) do (
 %ARM_GCC% %ARM_CFLAGS% -Wl,-Map=arm_prog_%%n.map arm_prog.c -o %RELEASE_BIN%\arm_prog_%%n.elf  -I%INC_PATH% -L %LIBS% -Wl,--whole-archive -l%LIB_ARM% -Wl,--no-whole-archive  -Wl,-T MC12705arm%%n.lds 
 %ARM_DISASM% %RELEASE_BIN%\arm_prog_%%n.elf > arm_prog_%%n.dmp
)
echo =======  END ARM Cross-compiler %ARM_PC_AVL%
echo ========= ARM Cross-compiler ARM_CCPU_AVL %ARM_CCPU_AVL%
for  %%n in (%ARM_CCPU_AVL%) do (
 %ARM_GCC% %ARM_CFLAGS% -Wl,-Map=arm_prog_%%n.map arm_prog.c -o %RELEASE_BIN%\arm_prog_%%n.elf  -I%INC_PATH% -L %LIBS% -Wl,--whole-archive -l%LIB_ARM% -Wl,--no-whole-archive  -Wl,-T MC12705arm%%n.lds 
 %ARM_DISASM% %RELEASE_BIN%\arm_prog_%%n.elf > arm_prog_%%n.dmp
)
echo =======  END ARM Cross-compiler %ARM_CCPU_AVL%
echo ========= ARM Cross-compiler ARM_PC_AVL_OFFSET %ARM_PC_AVL_OFFSET%
for  %%n in (%ARM_PC_AVL_OFFSET%) do (
 %ARM_GCC% %ARM_CFLAGS% -Wl,-Map=arm_prog_%%n.map arm_prog.c -o %RELEASE_BIN%\arm_prog_%%n.elf  -I%INC_PATH% -L %LIBS% -Wl,--whole-archive -l%LIB_ARM% -Wl,--no-whole-archive  -Wl,-T MC12705arm%%n.lds 
 %ARM_DISASM% %RELEASE_BIN%\arm_prog_%%n.elf > arm_prog_%%n.dmp
)
echo =======  END ARM Cross-compiler %ARM_PC_AVL_OFFSET%
echo ========= ARM Cross-compiler ARM_CCPU_AVL_OFFSET %ARM_CCPU_AVL_OFFSET%
for  %%n in (%ARM_CCPU_AVL_OFFSET%) do (
 %ARM_GCC% %ARM_CFLAGS% -Wl,-Map=arm_prog_%%n.map arm_prog.c -o %RELEASE_BIN%\arm_prog_%%n.elf  -I%INC_PATH% -L %LIBS% -Wl,--whole-archive -l%LIB_ARM% -Wl,--no-whole-archive  -Wl,-T MC12705arm%%n.lds 
 %ARM_DISASM% %RELEASE_BIN%\arm_prog_%%n.elf > arm_prog_%%n.dmp
)
echo =======  END ARM Cross-compiler %ARM_CCPU_AVL_OFFSET%
endlocal

echo ======= HOST Compiler
setlocal
@call "%VS140COMNTOOLS%..\..\vc\vcvarsall.bat" amd64
set TC1=tc010_01 tc010_02 tc010_03_0 tc010_03_1 tc010_04_0 tc010_04_1 
set TC2=tc010_05 tc010_06_0 tc010_06_1 tc010_07_0 tc010_07_1 tc010_08
set TC3=tc010_07_1_process
set TC=%TC1% %TC2% %TC3%

::echo on
for  %%g in (%TC%) do (
 cl %%g.cpp -W3 -MT -EHsc  -I%INC_PATH% -link -LIBPATH:%LIBS% %LIB_NAME%.lib -out:%RELEASE_BIN%\%%g.exe
)

copy %LIBS%\mc12705load.dll %RELEASE_BIN%\.
endlocal
:END


