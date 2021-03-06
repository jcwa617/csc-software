#
# BLUESAT Critical Systems Computer Builder Script
# Copyright BLUESAT Project 2011
# Version 2.0 
# Author: Colin Tan
#
# This script will build the various components of the OS and perform linking.
# An image file will than be created from the component compiles. 
# Burning can also be done via this file
#

#----------
# Includes
#----------

default: clean hex

#------------- 
# Directories
#-------------
CSC_DIR           	=../BLUEsat-CSC
APP_SOURCE_DIR		=$(CSC_DIR)/Applications
CORE_SOURCE_DIR		=$(CSC_DIR)/FreeRTOS/post_src
BUILDS_DIR			=../Builds
IMAGE_DIR			=../Dist
SCRIPTS_DIR			=.
UNIT_TESTS_DIR		=../Unit_Tests
TEST_SUITES			=$(UNIT_TESTS_DIR)/Test_Suites

#---------------
# Utility Files
#---------------
EXCLUSION_FILE    	=$(CSC_DIR)/SysBootAgent/include/AppExcList.xml
INCLUSION_LIST	  	=$(CSC_DIR)/SysBootAgent/include/ActiveModuleIncList.h

#---------------------
# Compiler / Settings
#--------------------- #Werror
C			=gcc
CC			=arm-elf-gcc
PP			=arm-elf-cpp
OBJCOPY	=arm-elf-objcopy
WARNINGS =-Wall -Wextra -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wsign-compare \
-Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunused -Wformat-security
CRT0		=$(CORE_SOURCE_DIR)/boot.s
DEBUG		=#-D NO_DEBUG ###[NO_DEBUG] remember to check for functions uses usDebugRead()
MEM_TEST	=#-D MEM_TEST 
CDEFINES	= -D GCC_ARM7 -D RUN_FROM_ROM $(DEBUG) $(MEM_TEST)
CFLAGS	= $(WARNINGS) $(CDEFINES) -I. $(patsubst %,-I%,$(subst :, ,$(INC_DIRS))) -mcpu=arm7tdmi -fomit-frame-pointer \
-fno-strict-aliasing -nostdlib -Os -T $(CORE_SOURCE_DIR)/lpc2468-rom.ld

LINKER_FLAGS=-lgcc -Xlinker -o$(IMAGE_DIR)/csc.elf -Xlinker -M -Xlinker -Map=$(IMAGE_DIR)/csc.map

TESTWARNINGS 	=-Wall
TESTCFLAGS 		=-I$(UNIT_TESTS_DIR) $(patsubst %,-I%,$(subst :, ,$(INC_DIRS))) -DUNIT_TEST
TESTOUT			=-o $(IMAGE_DIR)/$(<F).exe
BURN_DEV =/dev/ttyUSB0

#--------------
# Source files
#--------------
# files which should not be compiled need to be of another type instead of *.c/*.h
# it is suggested changing to *.c.ignore/*.h.ignore
#
TEST_CODE_TEMPLATE =test_*.c
CODE_TEMPLATE		 =*.c
EXEC_FILES    = $(shell  ls $(IMAGE_DIR)/test_*.exe)
TEST_SUBDIRS  =/test/
FIND_TESTS    =/usr/bin/find .. -name $(TEST_CODE_TEMPLATE) | grep $(TEST_SUBDIRS)
CODE_SUBDIRS  =/src/
FIND_CODE     =/usr/bin/find .. -name $(CODE_TEMPLATE) | grep $(CODE_SUBDIRS)
POST_CODE_SUBDIR =/post_src/
POST_FIND_CODE   =/usr/bin/find .. -name $(CODE_TEMPLATE) | grep $(POST_CODE_SUBDIR)
INC_SUBDIRS   =include
FIND_INCLUDES =/usr/bin/find .. -type d -name $(INC_SUBDIRS)

ALL_POST_SRC = $(shell $(POST_FIND_CODE))
ALL_PRE_SRC  = $(shell $(FIND_CODE))

INC_DIRS = $(shell $(FIND_INCLUDES))
TESTS    = $(shell  $(FIND_TESTS))

CUTEST = $(shell ls $(UNIT_TESTS_DIR)/*.c) 
SUITES = $(shell ls $(TEST_SUITES)/*.c) 

TEST_CORE = $(CUTEST)\
$(SUITES)

#-------------------------------------------
# Start of Selective Compilation and Linking
#-------------------------------------------

# Define all object files
ALL_TESTS 	 = $(TESTS:.c=.o)
ALL_PRE      = $(ALL_PRE_SRC:.c=.i)
ALL_POST_PRE = $(ALL_POST_SRC:.c=.i)
	
hex: elf
	$(OBJCOPY) $(IMAGE_DIR)/csc.elf -O ihex $(IMAGE_DIR)/csc.hex

elf: preprocess_stage1 $(ALL_POST_PRE)
	cd $(BUILDS_DIR); $(CC) $(CFLAGS) -c *.i
	$(CC) $(CFLAGS)  $(BUILDS_DIR)/*.o -nostartfiles $(CRT0) $(LINKER_FLAGS)

preprocess_stage1:$(ALL_PRE)
	perl $(SCRIPTS_DIR)/preprocess.pl $(EXCLUSION_FILE) $(BUILDS_DIR) $(INCLUSION_LIST)
#	$(shell perl $(SCRIPTS_DIR)/preprocess.pl $(EXCLUSION_FILE) $(BUILDS_DIR) $(INCLUSION_LIST))
 
   
$(ALL_PRE): %.i : %.c
	$(PP) $(CFLAGS) $< -o $(BUILDS_DIR)/$(@F)

$(ALL_POST_PRE): %.i : %.c
	$(PP) $(CFLAGS) $< -o $(BUILDS_DIR)/$(@F)

#-------------------------------------------
# End of Selective Compilation and Linking
#-------------------------------------------

#-------------------------------------------
# Start of Do All Compilation and Linking
#-------------------------------------------

ALL_SRC = $(shell $(FIND_CODE)) $(ALL_POST_SRC)
ALL_OBJ = $(ALL_SRC:.c=.o)
ALL_TARGET_OBJ=$(patsubst %,$(BUILDS_DIR)/%,$(notdir $(ALL_OBJ)))

compile_all: clean $(ALL_OBJ) $(CRT0)
	$(CC) $(CFLAGS) $(ALL_TARGET_OBJ) -nostartfiles $(CRT0) $(LINKER_FLAGS)
	$(OBJCOPY) $(IMAGE_DIR)/csc.elf -O ihex $(IMAGE_DIR)/csc.hex
	
$(ALL_OBJ): %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $(BUILDS_DIR)/$(@F)

#-------------------------------------------
# End of Do All Compilation and Linking
#-------------------------------------------

#-------------------------------------------
# Start of Unit Testing
#-------------------------------------------
$(ALL_TESTS) : %.o : %.c
	$(C) -g $(TEST_CORE) $< $(subst /test/,/src/,$(subst test_,,$<)) $(TESTCFLAGS) $(TESTWARNINGS) $(TESTOUT)		
	
unitTests: tests runAllTests

tests: $(ALL_TESTS)	
	
runAllTests: 
	sh run_tests.sh $(IMAGE_DIR)
	
#-------------------------------------------
# End of Unit Testing
#-------------------------------------------

#-----------
# Utilities
#-----------

burn:
	lpc21isp $(IMAGE_DIR)/csc.hex $(BURN_DEV) 115200 14746
	
keys: 
	perl $(SCRIPTS_DIR)/genDtmfKeys.pl > $(APP_SOURCE_DIR)/Dtmf_keys.c
	
combo:
	$(MAKE) default burn

#----------
# Clean Up
#----------

clean: cleantests
	rm -f $(BUILDS_DIR)/*
	rm -f $(IMAGE_DIR)/csc.*
	rm -f $(INCLUSION_LIST)
	
cleantests:
	rm -f $(IMAGE_DIR)/test_*.exe
	rm -f $(IMAGE_DIR)/AutoTestResults.txt
