WH Test GLIB Repository
=======================
Wade Hampton <wadehamptoniv@gmail.com>
0.2.1, 4/6/2016: updated, another example

Summary:
--------

This is a simple repository with multiple sample GLIB programs.  These are 
simple programs that may be used as examples for writing GLIB programs.  
This project may be public, but it does not yet serve any specific purpose.  
It may in the future....  At this point it includes a bunch of sample 
GLIB programs, some based on GLIB examples.

History:
--------

  0.1.0  09/29/2015  created

  0.2.0  10/27/2015  updated, links added

  0.3.0  04/06/2016  updated, another example added

Notes:
------

1.  This document is in AsciiDoc format based on my Minidoc format.

2.  This project is mainly to experiment with github and with GLIB.  It is
    loosely based on my wh_test_py github repository.

3.  License for ALL files is GPL, see LICENSE.

4.  Please e-mail me with any updates, suggestions, etc.

5.  Programs include TBD.

6.  After running the programs, run make clean to remove test files.


Why GLIB:
---------

I first found glib circa 2005 when I was writing some software to 
support communications and synchronization between threads in some
new C software I was writing.  For many years, I had simply used 
pipes and sockets on Xenix, SCO Unix, Solaris, Linux, etc., but 
I needed a new, clean library.  My code was almost completed when
I found an IBM tutorial on GLIB, so I kept only the top layers of 
my software and implemented the low-level features with glib.  
It has worked very well with most of my glib-based daemons running 
for well over a year without issue.

Prior to this, I had an assortment of code mainly using shared 
memory plus UNIX's pipes and select (this dated to about 1985
and ran on Xenix, SCO Unix, Solaris, and eventually Linux).

Links:
------

1.  Official API documentation:  https://developer.gnome.org/glib

2.  IBM tutorial on GLIB:  http://www.ibm.com/developerworks/linux/tutorials/l-glib/

