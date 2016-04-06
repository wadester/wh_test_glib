#!/usr/bin/make
# Module:   Makefile
# Purpose:  build the GLIB examples
# Author:   Wade Hampton
# Date:     9/25/2015
# 1)  This builds the examples.  Type make clean to cleanup.
# 2)  The pkg-config tools defines GLIB build options.  
#     Use pkg-config to get definitions from the O/S.
# 3)  These packages should work with Fedora/CentOS/RHEL/Ubuntu.
#
GLIB_LIB     = $(shell pkg-config --libs gthread-2.0)
GLIB_INCLUDE = $(shell pkg-config --cflags glib-2.0)

# define the compiler
CC=gcc

# define the libs, include lm for completeness
LIBS=$(GLIB_LIB) -lm 

# define the build flags, turn on all warnings and setup for GDB
DEBUG_FLAGS=-ggdb -Wall 
INCLUDE=-I . $(GLIB_INCLUDE)
CFLAGS=$(DEBUG_FLAGS) $(INCLUDE)

# define binaries so we have a list for build rules and cleanup
BINS=glib_test1 glib_atomic glib_hash glib_async_queue \
	glib_main_loop1 glib_main_loop2

# build test programs
all: $(BINS)

# simple build rule for examples, source is same name + ".c"
%: %.c
	$(CC) $(CFLAGS) -o $@ $<  $(LIBS)	

# cleanup:
clean::
	rm -f $(BINS) *.o 

