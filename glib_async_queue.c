/* 
 * Module:   glib_async_queue
 * Purpose:  to test glib async queues
 * Author:   Wade Hampton
 * Date:     10/26/2016
 * Notes:
 * 1)  This demonstrates the use of g_async_queue*
 *     
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <glib.h>

#define TSIZE 30

/** thread control structure */
typedef struct _TH_STR {
    GAsyncQueue* Q; 
    GThread*     TH;
} TH_STR;

/************************************************************************/
/** print time as time_t.usec, if input is NULL then allocate buffer */
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
/** th1 */
void* th1(void* pv) {
    TH_STR* ps;
    gboolean done_flag = FALSE;
    if (pv != NULL) {
	ps=(TH_STR*)pv;
    } else {
	char* ts = spt(NULL);
	printf("%s ERROR:  th1 could not initialize, no thread data\n", ts);
	g_free(ts);
    }
    /* inc ref counter on the queue indicating we are using it */
    g_async_queue_ref(ps->Q);

    /* loop until we receive a quit message, 
     * note reversed check on done_flag */
    while(FALSE == done_flag) {
	void* pv = g_async_queue_pop(ps->Q);
	if (pv != NULL) {
	    char* pmsg = (char*) pv;
	    char* ts = spt(NULL);
	    printf("%s TH1 MSG:  %s\n", ts, pmsg);

	    /* check for Q for quit */
	    if (pmsg[0] == 'Q') {
		printf("%s TH1 DONE!\n", ts);
		done_flag = TRUE;
	    }
	    g_free(ts);
	    g_free(pv);
	}
    }
    /* unref, when count->0 the queue will be destroyed */
    g_async_queue_unref(ps->Q);

    g_thread_exit(NULL);
    return(NULL);
}

/************************************************************************/
/** send msg to thread */
gboolean send_msg(const TH_STR* pth, const char* msg) {
    gboolean res = FALSE;
    char* ts = spt(NULL);
    if (pth == NULL) {
	printf("%s ERROR:  send_msg NULL thread struct\n", ts);
    } else {
	char* omsg = g_strdup_printf("%s [%s]", msg, ts);
	g_async_queue_push(pth->Q, omsg);
	res = TRUE;
    }
    return(res);
}

/************************************************************************/
/** output message with timestamp */
void o_msg(const char* msg) {
    char* ts = spt(NULL);
    printf("%s %s\n", ts, msg);
    g_free(ts);
}

/************************************************************************/
/** main */
int main (int argc, char* argv[]) {

    TH_STR my_th;

    /** create the queue and reference it */
    my_th.Q = g_async_queue_new();
    g_async_queue_ref(my_th.Q);

    /** create the thread */
    my_th.TH = g_thread_new("TH1", th1, &my_th);

    /** send multiple messages to the thread, end with a Q */
    send_msg(&my_th, "AAA");
    send_msg(&my_th, "AAB");
    send_msg(&my_th, "AAC");
    o_msg("Sending Q");
    send_msg(&my_th, "Q");

    /** unreference the queue */
    g_async_queue_unref(my_th.Q);

    /** join the thread and then exit */
    g_thread_join(my_th.TH);
    o_msg("Joined thread, exiting");
    
    exit(0);
}
