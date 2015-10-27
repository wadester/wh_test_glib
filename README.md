wh_test_glib
============
WH Test GLIB Repository -- GLIB test files

Summary:
========

This is a simple repository with multiple sample GLIB programs.  These are 
simple programs that may be used as examples for writing GLIB programs.  
This project may be public, but it does not yet serve any specific purpose.  
It may in the future....  At this point it includes a bunch of sample 
GLIB programs, some based on GLIB examples.

History:
========

1) 0.1   9/29/2015    created

2) 0.2	 10/27/2015   updated, links added

Notes:
======

1)  This document is in a parsable format similar to asciidoc.  I have used
    this format for MANY years (I may upload a tool or migrate to asciidoc).

2)  This project is mainly to experiment with github and with GLIB.  It is
    loosely based on my wh_test_py github repository.

3)  License for ALL files is GPL, see LICENSE.

4)  Please e-mail me with any updates, suggestions, etc.

5)  Programs include TBD.

6)  After running the programs, run make clean to remove test files.


Why GLIB:
=========

I first found glib circa 2005 when I was writing some software to 
support inter-thread communications in some new C/C++ software.  
For many years I had simply used pipes and sockets on Xenix, SCO Unix,
Solaris, Linux, etc., but I needed a new library.  I had my code 
nearly completely written when I found the IBM tutorial on GLIB.

I kept only the top layers of my software and implemented the low-level
features with glib.  It has worked very well with some of my glib-based
daemons running for well over a year without issue.

Prior to my use of glib, I had my own software, largely-based on 
UNIX's pipes and select.  Note this ran on Xenix, SCO Unix, Solaris,
and eventually Linux.

Links:
======

1)  Official API documentation:

    https://developer.gnome.org/glib

2)  IBM tutorial on GLIB:  

    http://www.ibm.com/developerworks/linux/tutorials/l-glib/