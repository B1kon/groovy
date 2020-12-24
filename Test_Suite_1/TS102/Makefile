
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
RELEASE = TS102
RELEASE_BIN = $(PTH_BIN)/$(RELEASE)

LIB_NAME = mc12705load

LIB_NM = mc12705load_nmc
LIB_ARM = mc12705load_arm

TC = tc102_01 tc102_02 tc102_03 tc102_04 tc102_05

NM = nm_prog
ARM = arm_prog

.PHONY: all clean

all: MKBIN $(NM) $(ARM) $(TC)

################################################################################


$(TC): 
	$(CC)  $(INC_PATH) $@.cpp -o $(RELEASE_BIN)/$@ -std=c++0x \
	-L$(LIB_PATH) -lstdc++ -lrt -l$(LIB_NAME) -pthread \
	-Wl,-rpath,./

MKBIN:
	echo "MKBIN"
	mkdir -p $(PTH_BIN) $(RELEASE_BIN)
	cp $(LIB_PATH)/lib$(LIB_NAME).so* $(RELEASE_BIN)

$(NM):	
	echo ========= "NM_part"
	$(NMC_GCC) $(NMC_CFLAGS) -T MC12705nm.lds -Wl,-Map=$(@).map $(INC_PATH) $(@).c nm_init.s -o $(@).abs -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_NM) -Wl,--no-whole-archive
	cp $@.abs $(RELEASE_BIN)/.

$(ARM):
	echo ===== ARM
	$(ARM_GCC) $(ARM_CFLAGS) -T MC12705arm.lds -Wl,-Map=$(@).map $(INC_PATH) $(@).c -o $(@).elf -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_ARM) -Wl,--no-whole-archive
	$(ARM_DISASM) $(@).elf > $(@).dmp
	cp $@.elf $(RELEASE_BIN)/.

clean:
	rm -rf $(RELEASE_BIN)
	rm -f *.map *.elf *.asm *.abs *.obj *.dmp
	rm -f *~ makeout

