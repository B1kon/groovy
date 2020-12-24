
# ======= NMC Cross-compiler
NMC_GCC		:=nmc-gcc -mgas 
#NMC_GCC		:=nmc-gcc -mgas -Xassembler -nmc4
NMC_CFLAGS	:= --entry=_start -T MC12705nm.lds -O2 -std=gnu99 -Wl,--nmagic
NMC_DISASM	:=nmc-objdump -D

# ======= ARM Cross-compiler
ifndef CROSS_ARM 
CROSS_ARM = arm-nm6408-eabihf-
endif

ARM_GCC		:=${CROSS_ARM}gcc -mcpu=cortex-a5 -mfpu=vfpv4-d16 -mfloat-abi=hard -marm
ARM_CFLAGS	:= --entry=_start -T MC12705arm.lds -O2 -std=gnu99 -Wl,--nmagic -Wl,--gc-sections
ARM_DISASM	:=${CROSS_ARM}objdump -D

# ======= HOST Compiler
CC = gcc
CFLAGS = -Werror -c -g 

INC_PATH = -I. -I../../include
LIB_PATH = ../../libs

PTH_BIN = ../../bin
RELEASE = TS011
RELEASE_BIN = $(PTH_BIN)/$(RELEASE)

LIB_NAME = mc12705load

LIB_NM = mc12705load_nmc
LIB_ARM = mc12705load_arm

TC1 = tc011_01 tc011_02 tc011_03 tc011_04 tc011_05 tc011_06
TC2 = tc011_07_0 tc011_07_1 tc011_08
TC = $(TC1) $(TC2)

NM = nm_prog_04 nm_prog_05

ARM = arm_prog_04 arm_prog_05

.PHONY: all clean

all: MKBIN $(NM) $(ARM) $(TC)
# $(NM) $(ARM)

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
	echo ========= "NM_part"
	$(NMC_GCC) $(NMC_CFLAGS) -Wl,-Map=$(@).map $(INC_PATH) $(@).c nm_init.s -o $(@).abs -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_NM) -Wl,--no-whole-archive

	cp $@.abs $(RELEASE_BIN)/.

$(ARM):
	echo ========= "ARM_part"  "CROSS_ARM" ${CROSS_ARM}
	$(ARM_GCC) $(ARM_CFLAGS) -Wl,-Map=$(@).map $(INC_PATH) $(@).c -o $(@).elf -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_ARM) -Wl,--no-whole-archive
	$(ARM_DISASM) $(@).elf > $(@).dmp
	cp $@.elf $(RELEASE_BIN)/.

clean:
	rm -rf $(RELEASE_BIN)
	rm -f *.map *.elf *.asm *.abs *.obj *.dmp
	rm -f *~ makeout
