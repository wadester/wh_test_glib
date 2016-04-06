/* 
 * Module:   glib_test1.c
 * Purpose:  GLIB2 test program 1 with very simple thread
 * Author:   Wade Hampton
 * Date:     9/29/2015
 * Notes:
 * 1)  This is a very simple example of creating a glib2 thread.
 * 2)  To build:  see Makefile
 * 3)  Reference:
 *       https://developer.gnome.org/glib/2.46/glib-Threads.html
 *       https://computing.llnl.gov/tutorials/pthreads/
 * 4)  The GLIB_CHECK_VERSION macro may be used to support 
 *     the deprecated thread API (prior to version 2.32.0).
 */

#include <stdio.h>
#include <unistd.h>

#include <glib.h>

/************************************************************************/

volatile int vv = 0;
GThread* th = NULL;

// test thread updating a volatile int variable, not really safe
void *my_thread(void *arg) {

    int ii;
    for (ii=1; ii<11; ii++) {
	printf("th1:  ii=%d vv=%d\n", ii, vv);
	sleep(1);
    }

    g_thread_exit(NULL);
    return(NULL);
}
GThreadFunc p_thread = my_thread;


// initialize the module
void Init() {
    printf("Init:  called\n");

    vv=1;

    // This is automatic in newer GLIB2, deprecated interface
    // Use the below only for old code: 
    //  if (!g_thread_supported()) {
    //	  g_thread_init(NULL);
    //  }

#if !GLIB_CHECK_VERSION(2,32,0)
    if (!g_thread_supported()) {
	g_thread_init(NULL);
    }
#endif

    // g_thread_create has been deprecated since version 2.32 
    // and should not be used in newly-written code.
    // Use g_thread_new() instead
    // Any of the following work:
    //   th = g_thread_create(p_thread, NULL, 1, NULL);
    //   th = g_thread_create(my_thread, NULL, 1, NULL);

#if GLIB_CHECK_VERSION(2,32,0)
    th = g_thread_new("My Thread", my_thread, NULL);
#else
    th = g_thread_create(my_thread, NULL, 1, NULL);
#endif
}

// cleanup at the end
void Cleanup() {
    printf("Cleanup:  called\n");
    g_thread_join(th);
    printf("Cleanup:  joined with thread\n");
}

// module main, call init, do work, call cleanup when done
int main(int argc, char* argv[]) {
    Init();
    printf("Hello\n");

    int ii;
    for (ii=0; ii<40; ii++) {
	vv++;
	usleep(300000);
    }

    Cleanup();

    printf("Done\n");
    return(0);
}
