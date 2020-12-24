
CC = gcc
CFLAGS = -Werror -c -g 

INC_PATH = -I. -I../../include
LIB_PATH = ../../libs

PTH_BIN = ../../bin
RELEASE = TS017
RELEASE_BIN = $(PTH_BIN)/$(RELEASE)

LIB_NAME = mc12705load

LIB_NM = mc12705load_nmc
LIB_ARM = mc12705load_arm

TC = tc017_01 tc017_02 

.PHONY: all clean

all: MKBIN $(TC)


################################################################################


$(TC): 
	$(CC)  $(INC_PATH) $@.cpp -o $(RELEASE_BIN)/$@  \
	-L$(LIB_PATH) -lstdc++ -lrt -l$(LIB_NAME) -pthread \
	-Wl,-rpath,./

MKBIN:
	echo "MKBIN"
	mkdir -p $(PTH_BIN) $(RELEASE_BIN)
	cp $(LIB_PATH)/lib$(LIB_NAME).so* $(RELEASE_BIN)
#	cp $(LIB_PATH)/initmb7707.abs $(RELEASE_BIN)

clean:
	rm -rf $(RELEASE_BIN)
	rm -f *.map *.elf *.asm *.abs *.obj
	rm -f *~ makeout
