/* 
 * Module:   glib_main_loop2.c
 * Purpose:  to test the main loop code (long version)
 * Author:   Wade Hampton
 * Date:     10/5/2015
 * Notes:
 * 1)  This uses 
 * 2)  Ref:  TBD
 *     
 */
#define TSIZE 30

#include <stdio.h>

#include <glib.h>
#include <sys/time.h>

/************************************************************************/
/* print time as time_t.usec, if input is NULL then allocate buffer */
char* spt(char* res) {
    if (res == NULL) {
	res = g_malloc(TSIZE);
    }
    struct timeval tv;
    gettimeofday(&tv, NULL);
    snprintf(res, TSIZE, "%010ld.%06ld", tv.tv_sec, tv.tv_usec);
    // res = g_strdup_printf("%010ld.%06ld", tv.tv_sec, tv.tv_usec);
    return(res);
}

/************************************************************************/
/** display a time-stamped message */
void tmsg(char* msg) {
    char* tbuf = spt(NULL);
    printf("%s:  %s\n", tbuf, msg);
    g_free(tbuf);
}

/************************************************************************/
/** timeout callback, see doc on g_timeout_add() 
 *  -- called until this returns FALSE when it will be removed */
gboolean tmo_callback(gpointer data) {
    //printf("%s\n", (char*)data);
    tmsg((char*)data);
    return TRUE;
}

/************************************************************************/
/* program main */
int main()
{
    tmsg("Starting");

    int tmo = 5;  /* seconds for callback */
    GMainLoop* main_loop = NULL;

    main_loop = g_main_loop_new (NULL, FALSE);

    /* interval, function callback, userdata */
    g_timeout_add_seconds(tmo, tmo_callback, "callback_function");

    g_main_loop_run(main_loop);
    return 0;
}
