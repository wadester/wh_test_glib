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
#include <stdlib.h>
#include <sys/time.h>

#include <glib.h>

/** context for a loop */
typedef struct _LOOP_STR {
    char*           name;
    int             tmo;
    GMainContext*   main_context;
    GMainLoop*      main_loop;
    GSource*        idle;
    GAsyncQueue*    Q;
    GThread*        TH;
} LOOP_STR;

LOOP_STR loop_main;
LOOP_STR loop_1;
LOOP_STR loop_2;

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
/** display a time-stamped message 
 *  -- typically call with twmsg(__func__, "some msg");
 *  -- can use __FUNCTION__ or __PRETTY_FUNCTION__ in C
 *  -- if using __PRETTY_FUNCTION in C++ it will include the type
 */
void twmsg(const char* where, const char* msg) {
    char* tbuf = spt(NULL);
    printf("%s:  %s: %s\n", tbuf, where, msg);
    g_free(tbuf);
}

/************************************************************************/
/** loop thread */
void* loop_thread(void* pv) {
    LOOP_STR* ps = NULL;
    if (pv == NULL) {
	twmsg(__func__, "ERROR pv=NULL");
    } else {
	ps = (LOOP_STR*)pv;
	twmsg(__func__, "STARTING");
	
	/* run the main loop for this thread */
	g_main_loop_run(ps->main_loop);

    }
	
    twmsg(__func__, "EXITING");
    g_thread_exit(NULL);
    return(NULL);
}

/************************************************************************/
/** send msg to a queue */
gboolean MsgSend(LOOP_STR* ps, char* msg) {
    gboolean res = FALSE;
    if (ps == NULL) {
	twmsg(__func__, "ERROR pv=NULL");
    } else {
	/* create new copy of msg and send, then wakeup context */
	char* msg_data = g_strdup(msg);
	g_async_queue_push(ps->Q, msg_data);
	g_main_context_wakeup(ps->main_context);
	res = TRUE;
    }
    return(res);
}

/************************************************************************/
/** cleanup */

/************************************************************************/
/** idle function */
int idle_func(gpointer idle_data) {
    
    LOOP_STR* ps = (LOOP_STR*)idle_data;
    char* pmsg = NULL;
    pmsg = g_strdup_printf("ENTERED D=%s", ps->name);
    twmsg(__func__, pmsg);
    g_free(pmsg);

    
    twmsg(__func__, "EXITING");

    /* return T and it will be called ALL THE TIME 
       return(TRUE);
    */
    return(FALSE);  // one-time call....
}

/************************************************************************/

/************************************************************************/
/** timeout callback, see doc on g_timeout_add() 
 *  -- called until this returns FALSE when it will be removed */
gboolean tmo_callback(gpointer data) {
    //printf("%s\n", (char*)data);
    twmsg(__func__, (char*)data);

    /* send msg to loop 1 and 2 
     * - ideally check to see who caused the tmo 
     */
    MsgSend(&loop_1, "TMO1");
    MsgSend(&loop_2, "TMO2");

    return TRUE;
}

/************************************************************************/
/* program main */
int main()
{
    char* tmsg = NULL;
    int   main_tmo = 5;
    int   loop1_tmo = 2 * main_tmo;
    int   loop2_tmo = 3;

    twmsg(__func__, "STARTING");


    /* setup the main loop structure */
    tmsg=g_strdup_printf("creating main context, TMO=%d", main_tmo);
    twmsg(__func__, tmsg);
    g_free(tmsg);

    loop_main.name         = g_strdup("MAIN");
    loop_main.tmo          = main_tmo;  /* seconds for callback */
    loop_main.main_context = NULL;   /* use default */
    loop_main.main_loop    = g_main_loop_new (loop_main.main_context, FALSE);
    loop_main.idle         = g_idle_source_new();
    g_source_set_callback(loop_main.idle, idle_func, &loop_main, NULL);
    g_source_attach(loop_main.idle, loop_main.main_context);
    loop_main.main_context = NULL;  /* use default */
    loop_main.Q            = NULL;  /* no queue, for now */
    loop_main.TH           = NULL; /* this is the main thread so no thread info */

    /* interval, function callback, userdata 
     * - internally creates source and attaches to main context */
    g_timeout_add_seconds(loop_main.tmo, tmo_callback, "tmo_callback");


    /* create info for the worker thread and set up its context, then run it */
    tmsg=g_strdup_printf("creating loop1 context, TMO=%d", loop1_tmo);
    twmsg(__func__, tmsg);
    g_free(tmsg);
   
    loop_main.name         = g_strdup("LOOP_1");
    loop_1.tmo = loop1_tmo;
    loop_1.main_context    = g_main_context_new();
    loop_1.main_loop       = g_main_loop_new(loop_1.main_context, FALSE);
    loop_1.idle            = g_idle_source_new();
    g_source_set_callback(loop_1.idle, idle_func, &loop_1, NULL);
    g_source_attach(loop_1.idle, loop_1.main_context);
    loop_1.Q               = g_async_queue_new();

    /* manually create new timeout source and attach to main context of nth Loop 
     * - timeout is in ms so multiply seconds by 1000
     * - set priority to 1 or 2
     * - optionally set name 
     */
    GSource *source1 = g_timeout_source_new (loop_1.tmo*1000);
    g_source_set_priority (source1, 1);
    g_source_set_callback (source1, tmo_callback, "tmo_loop1_callback", NULL);
    g_source_set_name (source1, "LOOP1_TMO"); 
    g_source_attach (source1, loop_1.main_context);

    /* run the thread which will run the main context */
    loop_1.TH           = g_thread_new("LOOP1_TH", loop_thread, &loop_1);  



    /* create info for the second worker thread and set up its context, then run it */
    tmsg=g_strdup_printf("creating loop2 context, TMO=%d", loop2_tmo);
    twmsg(__func__, tmsg);
    g_free(tmsg);

    /* init the data for the context */
    loop_2.name         = g_strdup("LOOP_2");
    loop_2.tmo = loop2_tmo;
    loop_2.main_context = g_main_context_new();
    loop_2.main_loop    = g_main_loop_new(loop_2.main_context, FALSE);
    loop_2.idle         = g_idle_source_new();
    g_source_set_callback(loop_2.idle, idle_func, &loop_2, NULL);
    g_source_attach(loop_2.idle, loop_2.main_context);
    loop_2.Q            = g_async_queue_new();

    /* manually create new timeout source and attach to main context of nth Loop 
     * - timeout is in ms so multiply seconds by 1000
     * - set priority to 1 or 2
     * - optionally set name 
     */
    GSource *source2 = g_timeout_source_new (loop_2.tmo*1000);
    g_source_set_priority (source2, 1);
    g_source_set_callback (source2, tmo_callback, "tmo_loop2_callback", NULL);
    g_source_set_name (source2, "LOOP2_TMO"); 
    g_source_attach (source2, loop_2.main_context);

    /* run the second thread */
    loop_2.TH           = g_thread_new("LOOP2_TH", loop_thread, &loop_2);

  
    /* run the main loop */
    g_main_loop_run(loop_main.main_loop);

    
    return 0;
}
