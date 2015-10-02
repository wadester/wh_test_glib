#!/usr/bin/make
# Module:   Makefile
# Purpose:  build the GLIB examples
# Author:   Wade Hampton
# Date:     9/25/2015
#

# defines for GLIB, use pkg-config to get from the O/S
# should work with Fedora/CentOS/RHEL/Ubuntu
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
BINS=glib_test1 glib_atomic glib_hash 

# build test programs
all: $(BINS)

# simple build rule for examples, source is same name + ".c"
%: %.c
	$(CC) $(CFLAGS) -o $@ $<  $(LIBS)	

# cleanup:
clean::
	rm -f $(BINS) *.o 

