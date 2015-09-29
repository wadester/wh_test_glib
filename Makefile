#!/usr/bin/make
# Module:  Makefile
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

# define the libs
LIBS=$(GLIB_LIB) -lm 

# define the build flags, turn on all warnings and setup for GDB
DEBUG_FLAGS=-ggdb -Wall 
INCLUDE=-I . $(GLIB_INCLUDE)
CFLAGS=$(DEBUG_FLAGS) $(INCLUDE)

# binaries
BINS=glib_test1

# build test programs
glib_test1:  glib_test1.c
	$(CC) $(CFLAGS) -o glib_test1 glib_test1.c $(LIBS)

# cleanup:
clean::
	rm -f $(BINS) *.o 

