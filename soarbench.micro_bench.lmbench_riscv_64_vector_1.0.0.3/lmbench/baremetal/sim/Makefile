
SRC_DIR = ../src
COM_DIR = ../sim
BIN_DIR = ../bin
OBJ_DIR = ../obj

#SRC_DIR = ../src
#COM_DIR = ../../../../baremetal
#INC_DIR = ../../../include
#BIN_DIR = ../bin
#OBJ_DIR = ./obj

CROSS_COMPILE_DIR = /opt/tiger/riscv-toolchain/bin

CROSS_COMPILE = riscv64-unknown-linux-gnu-

AS           = $(CROSS_COMPILE_DIR)/$(CROSS_COMPILE)as
LD           = $(CROSS_COMPILE_DIR)/$(CROSS_COMPILE)ld
CC           = $(CROSS_COMPILE_DIR)/clang
CPP          = $(CROSS_COMPILE_DIR)/$(CROSS_COMPILE)cpp

CFLAGS       = -march=rv64imafdcv_zba_zbb_zbc_zbs_zicond_zicbom_zicbop_zicboz_zicsr_zifencei_zihintpause_zawrs_zfa_zacas -mcpu=jupiter-a2 -mabi=lp64d -mcmodel=medany -static -g -std=c89 -O2 -I .

LNK_SCRIPT   = $(COM_DIR)/boot.ld
LNK_FILE_OPT = -nostartfiles -Xlinker -T$(LNK_SCRIPT)

ifeq ($(PAGE),1G)
PAGE_FLAG = -DPAGE_1G
endif
ifeq ($(PAGE),2M)
PAGE_FLAG = -DPAGE_2M
endif
ifeq ($(PAGE),4K)
PAGE_FLAG = -DPAGE_4K
endif
ifeq ($(PAGE_RANDOM), 1)
PAGE_FLAG += -DPAGE_RANDOM
endif
CFLAGS += $(PAGE_FLAG)

TEST_NAME = $(TEST)

ifneq ($(SIZE), )
CFLAGS += -D SIZE=$(SIZE)
TEST_NAME += $(subst *,,$(SIZE))
endif

ifneq ($(STRIDE), )
CFLAGS += -D STRIDE=$(STRIDE)
#TEST_NAME += STRDIE$(STRIDE)
endif

ifneq ($(WARMLOOP), )
CFLAGS += -D WARMLOOP=$(WARMLOOP)
#TEST_NAME += WARMLOOP$(WARMLOOP)
endif

ifneq ($(BENCHLOOP), )
CFLAGS += -D BENCHLOOP=$(BENCHLOOP)
#TEST_NAME += BENCHLOOP$(BENCHLOOP)
endif

ifneq ($(PAGE), )
#CFLAGS += -D PAGE=$(PAGE)
TEST_NAME += $(PAGE)
endif

EMPTY =
SPACE = $(EMPTY) $(EMPTY)
TEST_NAME := $(subst $(SPACE),_,$(TEST_NAME))

OBJS = boot.o pte.o $(TEST_NAME).o

BINS := $(TEST_NAME).elf
BINS := $(addprefix $(BIN_DIR)/, $(BINS))

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) $(LNK_FILE_OPT) $(addprefix $(OBJ_DIR)/, $(OBJS)) -o $@ -static

$(TEST_NAME).o:
	$(CC) $(INC_DIR) $(SRC_DIR)/$(TEST).c $(CFLAGS) -c -o $(OBJ_DIR)/$(TEST_NAME).o


boot.o:
	$(CPP) $(COM_DIR)/boot.s $(CFLAGS) | $(AS) -o $(OBJ_DIR)/boot.o

pte.o:
	$(CPP) $(COM_DIR)/pte.s $(CFLAGS) | $(AS) -o $(OBJ_DIR)/pte.o

ifndef TEST
	@echo "ERROR: Please add TEST=xxx"
endif

clean: 
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*.elf 


