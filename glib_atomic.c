/* 
 * Module:   glib_test1.c
 * Purpose:  GLIB2 test program with atomic set using old and new methods
 * Author:   Wade Hampton
 * Date:     9/30/2015
 * Notes:
 * 1)  This uses g_atomic_int_compare_and_exchange() to change a value
 *     and also uses g_atomic_int_set().  Note that older GLIB2 such
 *     as that with CentOS 5 does not include g_atomic_int_set().
 * 2)  Ref:  none
 *     
 */
#include <stdio.h>
#include <unistd.h>

#include <glib.h>

/************************************************************************/

gint atomic_val = 0;         // atomic int 
gint atomic_miss = 0;        // miss counter 
gint max_ctr = 10000000;     // loop size

/************************************************************************/

/** set a value using  g_atomic_int_compare_and_exchange() */
static inline gint set_old(gint *atomic, gint newval) {
    gint oldval;
    while(TRUE) {
	oldval = g_atomic_int_get(atomic);
	if (g_atomic_int_compare_and_exchange(atomic, oldval, newval))
	    break;
	g_atomic_int_inc(&atomic_miss);
    }
    return(newval);
}

/** thread 1, set to int value of arg */
void* thread1(void* arg) {
    gint ii = 0;
    gint val=*(gint*)arg;
    GTimer* my_timer = g_timer_new();
    g_thread_yield();
    for (ii=0; ii<max_ctr; ii++) {
	set_old(&atomic_val, val);
    }
    g_timer_stop(my_timer);
    printf("Thread %d elapsed=%lf\n", val, g_timer_elapsed(my_timer, NULL));
    return(NULL);
}

/** thread 2, set to val using g_atomic_int_set */
void* thread2(void* arg) {
    gint ii = 0;
    gint val=*(gint*)arg;
    GTimer* my_timer = g_timer_new();
    g_thread_yield();
    for (ii=0; ii<max_ctr; ii++) {
	set_old(&atomic_val, 1);
    }
    g_timer_stop(my_timer);
    printf("Thread %d elapsed=%lf\n", val, g_timer_elapsed(my_timer, NULL));
    return(NULL);
}

int main(int argc, char* argv[]) {
    int a = 1;
    int b = 2;

    printf("glib_atomic.c:  atomic tests using glib\n");
    printf("Test 1 using g_atomic_int_compare_and_exchange()\n");
    GThread* t1 = g_thread_new("ONE", &thread1, &a);
    GThread* t2 = g_thread_new("TWO", &thread1, &b);

    g_thread_join(t1);
    g_thread_join(t2);
    gint misses = g_atomic_int_get(&atomic_miss);
    printf("Misses=%d\n", misses);

    printf("Test 2 using g_atomic_int_set()\n");
    t1 = g_thread_new("ONE", &thread2, &a);
    t2 = g_thread_new("TWO", &thread2, &b);

    g_thread_join(t1);
    g_thread_join(t2);

    return(0);
}
