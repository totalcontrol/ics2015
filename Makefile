##### golbal settings #####
#@@ -3,2 +3,32 @@
#-USERPROG = obj/testcase/mov
USERPROG = obj/testcase/mov-c
ENTRY := $(USERPROG)

.PHONY: nemu entry all_testcase kernel run gdb test submit clean

CC := gcc
LD := ld
CFLAGS := -MMD -Wall -Werror -c

LIB_COMMON_DIR := lib-common
NEWLIBC_DIR := $(LIB_COMMON_DIR)/newlib
NEWLIBC := $(NEWLIBC_DIR)/libc.a
#FLOAT := obj/$(LIB_COMMON_DIR)/FLOAT.o

include config/Makefile.git
include config/Makefile.build

all: nemu


##### rules for building the project #####

include nemu/Makefile.part
include testcase/Makefile.part
include lib-common/Makefile.part
include kernel/Makefile.part
include game/Makefile.part

nemu: $(nemu_BIN)
all_testcase: $(testcase_BIN)
kernel: $(kernel_BIN)
game: $(game_BIN)


##### some convinient rules #####

entry: $(ENTRY)
	objcopy -S -O binary $(ENTRY) entry

run: $(nemu_BIN) $(USERPROG) entry
	$(call git_commit, "run")
	$(nemu_BIN) $(USERPROG)

gdb: $(nemu_BIN) $(USERPROG) entry
	gdb -s $(nemu_BIN) --args $(nemu_BIN) $(USERPROG)

test: $(nemu_BIN) $(testcase_BIN) entry
	bash test.sh $(testcase_BIN)

submit: clean
	cd .. && tar cvj $(shell pwd | grep -o '[^/]*$$') > $(STU_ID).tar.bz2

clean:
	-rm -rf obj 2> /dev/null
	-rm -f log.txt entry 2> /dev/null
