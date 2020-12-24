
# ======= NMC Cross-compiler
NMC_GCC		:=nmc-gcc -mgas 
NMC_CFLAGS	:= -T MC12705nm.lds -O2 -std=gnu99 -Wl,--nmagic
NMC_DISASM	:=nmc-objdump -D

# ======= HOST Compiler
CC = gcc
CFLAGS = -Werror -c -g 

LLC_INCL = ../../include
INC_PATH = -I. -I${LLC_INCL}
LIB_PATH = ../../libs

PTH_BIN = ../../bin
RELEASE = TS009
RELEASE_BIN = $(PTH_BIN)/$(RELEASE)

LIB_NAME = mc12705load
LIB_NM = mc12705load_nmc

LDS_TEMPLATE_NMC = ${LLC_INCL}/nmc_template.x
NMC_LDS_FLAGS = -DUSE_ONLY_IM

TC = tc009_01 tc009_02 tc009_03 tc009_04 

NM = nm_prog

.PHONY: all clean

all: MKBIN $(NM)  $(TC)


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

$(NM): MC12705nm.lds
	echo ========= "NM_part"
	$(NMC_GCC) $(NMC_CFLAGS) -Wl,-Map=$(@).map $(INC_PATH) $(@).c  -o $(@).abs -L $(LIB_PATH) -Wl,--whole-archive -l$(LIB_NM) -Wl,--no-whole-archive
	cp $@.abs $(RELEASE_BIN)/.

MC12705nm.lds:
	echo ========= "NM_lds"
	$(NMC_GCC) -E -P -o $(@) -x c-header $(LDS_TEMPLATE_NMC) $(NMC_LDS_FLAGS)

clean:
	rm -rf $(RELEASE_BIN)
	rm -f *.map *.elf *.asm *.abs *.obj *.dmp
	rm -f *~ makeout
