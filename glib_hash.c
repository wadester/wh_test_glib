/* 
 * Module:   glib_hash
 * Purpose:  to read the dictionary into a hash and search it
 * Author:   Wade Hampton
 * Date:     9/30/2015
 * Notes:
 * 1)  This uses IO channels to open and read a file into a hash.
 *     Multiple other GLIB functions are also used including string
 *     and timer functions.  
 *
 * 2)  Ref:  GLIB docs
 *     
 */
#include <stdio.h>

#include <glib.h>

/************************************************************************/

GHashTable* ht = NULL;  // test hash table
GIOChannel* ch = NULL;  // file to read

const gchar* DICx="/usr/share/dict/junk";
const gchar* DICT="/usr/share/dict/words";
GError* perr = NULL;

/************************************************************************/
/** get time as a string, for NOW use NULL, user must g_free result */
gchar* get_time(GTimeVal* ptv) {
    if (ptv == NULL) {
	GTimeVal tv;
	ptv = &tv;
	g_get_current_time(ptv);
    }
    gchar* res = g_time_val_to_iso8601(ptv);
    return(res);
}

/************************************************************************/
/** test result and print an error if one is found */
gboolean test_err(char* msg, GError* perr) {
    gboolean res = FALSE;
    if (perr != NULL) {
	gchar* stime = get_time(NULL);
	printf("%s error %s d=%d code=%d, msg=%s\n",
	       stime, msg, 
	       perr->domain, perr->code, perr->message);
	g_free(stime);
	res = TRUE;
    }
    return(res);
}

/************************************************************************/
/** reset any error */
gboolean reset_err(GError** perr) {
    if (perr != NULL) {
	if (*perr != NULL) {
	    g_error_free(*perr);
	    *perr = NULL;
	}
    }
    return(TRUE);
}

/************************************************************************/
gboolean find_entry(GHashTable* pht, gchar* key, gboolean verbose_in) {

    gchar* str;
    GTimer* tmp_timer = NULL;
    if (pht == NULL) {
	g_error("NULL hash table");
    }
    if (verbose_in) {
	tmp_timer=g_timer_new();
	//g_timer_start(my_timer);
    }
    str = g_hash_table_lookup(ht, key);
    if (verbose_in) {
	g_timer_stop(tmp_timer); 
	printf("Found key=%s with value %s in %lf\n", 
	       key, str, g_timer_elapsed(tmp_timer, NULL));
	g_timer_destroy(tmp_timer);
    }
    if (str != NULL) {
	return(TRUE);
    } else {
	return(FALSE);
    }
}

/************************************************************************/
/** function for use with foreach search, etc. */
gboolean srch(gpointer key, gpointer val, gpointer udata) {
    gchar* str = (gchar*)udata;
    gboolean res = g_str_equal(str, key);
    return(res);
}

/************************************************************************/
/** foreach function, GHFunc, print result if we find match */
void fe_srch(gpointer key, gpointer val, gpointer udata) {
    gboolean res = srch(key, val, udata);
    if (res == TRUE) {
	printf("Found key=%s\n", (char*)key);
    }
}

/************************************************************************/
/** test main function */
int main(int argc, char* argv[]) {
    int   idx=0;
    char* str = NULL;
    gsize slen=0;
    gboolean bres;
    GIOStatus gst;
    GTimer* my_timer = NULL;
    gchar* key;

    printf("glib_hash.c:  test glib's hash and other fcns\n");
    
    // FAIL:  open the dictionary, if we fail print error
    ch = g_io_channel_new_file(DICx, "r", &perr);
    if (ch == NULL) {
	test_err("opening DICx", perr);
	reset_err(&perr);
	// for test uncomment:  g_error("open DICx failed");
    }

    // open the dictionary, if we fail print error
    ch = g_io_channel_new_file(DICT, "r", &perr);
    if (ch == NULL) {
	test_err("opening DICT", perr);
	reset_err(&perr);
	g_error("open DICT failed");
    }
    
    // create a hash using strings and default string fcns 
    ht = g_hash_table_new(g_str_hash, g_str_equal);

    // time our read of all lines and insertion into the hash 
    my_timer = g_timer_new();
    gst = G_IO_STATUS_NORMAL;
    while (TRUE) {
	gst = g_io_channel_read_line(ch, &str, &slen, NULL, &perr);
	if (gst != G_IO_STATUS_NORMAL) {
	    test_err("read line error", perr);
	    reset_err(&perr);
	    break;
	}
	//bres = g_hash_table_insert(ht, sx, str);
	// we are using the hash table as a set
	g_strchomp(str);
	bres = g_hash_table_add(ht, str);
	if (!bres) {
	    g_error("hash add failed");
	}
	idx++;
    }
    g_timer_stop(my_timer); 
    printf("Read %d in %lf\n", idx, g_timer_elapsed(my_timer, NULL));

    // close the input channel 
    // deprecated: g_io_channel_close(ch);
    gst = g_io_channel_shutdown(ch, TRUE, &perr);
    if (gst != G_IO_STATUS_NORMAL) {
	test_err("shutdown error", perr);
	reset_err(&perr);
    }    

    // look for some examples in the data
    g_timer_start(my_timer);
    key = g_strdup("Linux");
    str = g_hash_table_lookup(ht, key);
    g_timer_stop(my_timer); 
    printf("Found key=%s with value %s in %lf\n", 
	   key, str, g_timer_elapsed(my_timer, NULL));
    g_free(key);
    
    find_entry(ht, "joker", TRUE);
    find_entry(ht, "joke", TRUE);
    find_entry(ht, "secret", TRUE);

    printf("Finding string using g_hash_table_foreach()\n");
    g_hash_table_foreach(ht, fe_srch, "Linux");

    printf("Finding string using g_hash_table_find()\n");
    str = g_hash_table_find(ht, srch, "Linux");
    printf("Found %s\n", str);

    return(0);
}

