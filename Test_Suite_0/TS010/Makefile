
# ======= NMC Cross-compiler
NMC_GCC		:=nmc-gcc -mgas 
#NMC_GCC		:=nmc-gcc -mgas -Xassembler -nmc4
NMC_CFLAGS	:= --entry=_start -O2 -std=gnu99 -Wl,--nmagic
# -T MC12705nm.lds 
NMC_DISASM	:=nmc-objdump -D

# ======= ARM Cross-compiler
ifndef CROSS_ARM 
CROSS_ARM = arm-nm6408-eabihf-
endif
ARM_GCC		:=${CROSS_ARM}gcc -mcpu=cortex-a5 -mfpu=vfpv4-d16 -mfloat-abi=hard -marm
ARM_CFLAGS	:= --entry=_start -O2 -std=gnu99 -Wl,--nmagic -Wl,--gc-sections
# -T MC12705arm.lds
ARM_DISASM	:=${CROSS_ARM}objdump -D

# ======= HOST Compiler
CC = gcc
CFLAGS = -Werror -c -g 

INC_PATH = -I. -I../../include
LIB_PATH = ../../libs

PTH_BIN = ../../bin
RELEASE = TS010
RELEASE_BIN = $(PTH_BIN)/$(RELEASE)

LIB_NAME = mc12705load

LIB_NM = mc12705load_nmc
LIB_ARM = mc12705load_arm

TC1 = tc010_01 tc010_02 tc010_03_0 tc010_03_1 tc010_04_0 tc010_04_1 
TC2 =  tc010_05 tc010_06_0 tc010_06_1 tc010_07_0 tc010_07_1 tc010_08
TC = $(TC1) $(TC2)
NM = nm_prog_05
NM_protect = nm_prog_0x0
NM_AVL = 0 4 C D

ARM = arm_prog_05
ARM_protect = arm_prog_0x0
ARM_PC_AVL = PC_0 PC_10 PC_30
ARM_CCPU_AVL = CCPU_0 CCPU_10
ARM_PC_AVL_OFFSET = PC_0_OFFSET
ARM_CCPU_AVL_OFFSET = CCPU_10_OFFSET

.PHONY: all clean

all: MKBIN $(NM_protect) $(NM_AVL) $(NM) $(ARM_protect) $(ARM_PC_AVL) $(ARM_CCPU_AVL)  $(ARM_PC_AVL_OFFSET)  $(ARM_CCPU_AVL_OFFSET) $(ARM) $(TC)

################################################################################


$(TC): 
	$(CC)  $(INC_PATH) $@.cpp -o $(RELEASE_BIN)/$@ -std=c++0x \
	-L$(LIB_PATH) -lstdc++ -lrt -l$(LIB_NAME) -pthread \
	-Wl,-rpath,./

MKBIN:
	echo "MKBIN"
	mkdir -p $(PTH_BIN) $(RELEASE_BIN)
	cp $(LIB_PATH)/lib$(LIB_NAME).so* $(RELEASE_BIN)
#	cp $(LIB_PATH)/initmb7707.abs $(RELEASE_BIN)

$(NM):
		echo ===== NM
	$(NMC_GCC) $(NMC_CFLAGS) -T MC12705nm.lds -Wl,-Map=$(@).map $(INC_PATH) $(@).c -o $(@).abs -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_NM) -Wl,--no-whole-archive
	cp $@.abs $(RELEASE_BIN)/.

$(NM_protect):
		echo ===== NM_REG_protect
	$(NMC_GCC) $(NMC_CFLAGS) -T MC12705nm_REG_protect.lds -Wl,-Map=$@_REG_protect.map $(INC_PATH) $(@).c -o $@_REG_protect.abs -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_NM) -Wl,--no-whole-archive
	cp $@_REG_protect.abs $(RELEASE_BIN)/.

# absent
	echo ===== NM_absent
	$(NMC_GCC) $(NMC_CFLAGS) -T MC12705nm_absent.lds -Wl,-Map=$@_absent.map $(INC_PATH) $(@).c -o $@_absent.abs -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_NM) -Wl,--no-whole-archive
	cp $@_absent.abs $(RELEASE_BIN)/.

$(NM_AVL):
		echo ===== NM_AVL
	$(NMC_GCC) $(NMC_CFLAGS) -T MC12705nm_$@.lds -Wl,-Map=nm_prog_$@.map $(INC_PATH) nm_prog.c -o nm_prog_$@.abs -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_NM) -Wl,--no-whole-archive
	cp nm_prog_$@.abs $(RELEASE_BIN)/.

$(ARM):
	echo ===== ARM
	$(ARM_GCC) $(ARM_CFLAGS) -T MC12705arm.lds -Wl,-Map=$(@).map $(INC_PATH) $(@).c -o $(@).elf -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_ARM) -Wl,--no-whole-archive
	$(ARM_DISASM) $(@).elf > $(@).dmp
	cp $@.elf $(RELEASE_BIN)/.

$(ARM_protect):
	echo ===== ARM_REG_PROTECT
	$(ARM_GCC) $(ARM_CFLAGS) -T MC12705arm_REG_protect.lds -Wl,-Map=$@_REG_protect.map $(INC_PATH) $(@).c -o $@_REG_protect.elf -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_ARM) -Wl,--no-whole-archive
	$(ARM_DISASM) $@_REG_protect.elf > $@_REG_protect.dmp
	cp $@_REG_protect.elf $(RELEASE_BIN)/.
# CCPU_0x3_protect
	echo ===== ARM_CCPU_0x3_protect
	$(ARM_GCC) $(ARM_CFLAGS) -T MC12705arm_CCPU_0x3_protect.lds -Wl,-Map=$@_CCPU_0x3_protect.map $(INC_PATH) $(@).c -o $@_CCPU_0x3_protect.elf -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_ARM) -Wl,--no-whole-archive
	$(ARM_DISASM) $@_CCPU_0x3_protect.elf > $@_CCPU_0x3_protect.dmp
	cp $@_CCPU_0x3_protect.elf $(RELEASE_BIN)/.
# absent
	echo ===== ARM_absent
	$(ARM_GCC) $(ARM_CFLAGS) -T MC12705arm_absent.lds -Wl,-Map=$@_absent.map $(INC_PATH) $(@).c -o $@_absent.elf -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_ARM) -Wl,--no-whole-archive
	$(ARM_DISASM) $@_absent.elf > $@_absent.dmp
	cp $@_absent.elf $(RELEASE_BIN)/.

$(ARM_PC_AVL):
	echo ===== ARM_PC_AVL
	$(ARM_GCC) $(ARM_CFLAGS) -T MC12705arm$@.lds -Wl,-Map=arm_prog_$@.map $(INC_PATH) arm_prog.c -o arm_prog_$@.elf -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_ARM) -Wl,--no-whole-archive
	$(ARM_DISASM) arm_prog_$@.elf > arm_prog_$@.dmp
	cp arm_prog_$@.elf $(RELEASE_BIN)/.

$(ARM_CCPU_AVL):
	echo ===== ARM_CCPU_AVL
	$(ARM_GCC) $(ARM_CFLAGS) -T MC12705arm$@.lds -Wl,-Map=arm_prog_$@.map $(INC_PATH) arm_prog.c -o arm_prog_$@.elf -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_ARM) -Wl,--no-whole-archive
	$(ARM_DISASM) arm_prog_$@.elf > arm_prog_$@.dmp
	cp arm_prog_$@.elf $(RELEASE_BIN)/.

$(ARM_PC_AVL_OFFSET):
	echo ===== ARM_PC_AVL
	$(ARM_GCC) $(ARM_CFLAGS) -T MC12705arm$@.lds -Wl,-Map=arm_prog_$@.map $(INC_PATH) arm_prog.c -o arm_prog_$@.elf -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_ARM) -Wl,--no-whole-archive
	$(ARM_DISASM) arm_prog_$@.elf > arm_prog_$@.dmp
	cp arm_prog_$@.elf $(RELEASE_BIN)/.


$(ARM_CCPU_AVL_OFFSET):
	echo ===== ARM_PC_AVL
	$(ARM_GCC) $(ARM_CFLAGS) -T MC12705arm$@.lds -Wl,-Map=arm_prog_$@.map $(INC_PATH) arm_prog.c -o arm_prog_$@.elf -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_ARM) -Wl,--no-whole-archive
	$(ARM_DISASM) arm_prog_$@.elf > arm_prog_$@.dmp
	cp arm_prog_$@.elf $(RELEASE_BIN)/.

clean:
	rm -rf $(RELEASE_BIN)
	rm -f *.map *.elf *.asm *.abs *.obj *.dmp
	rm -f *~ makeout

