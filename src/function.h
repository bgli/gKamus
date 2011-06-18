/*
 *  function.h, gKamus (http://gkamus.sourceforge.net)
 */

#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <gtk/gtk.h>

#include "main.h"

gsize write_dict (const gchar*);
gint search_dictb (const gchar*);
gint search_dict (const gchar*);
gint save_dict_func (const gchar*, gchar*, gint);
gint check_dict_file (gpointer);
gint list_compare (const Dictionary*, const Dictionary*);
void free_elements (Dictionary*, gpointer);
GkamusDicStatus load_dict (const gchar*);
void set_treev_word (void);
void show_dicstatus (GkamusDicStatus);

#endif /* __FUNCTION_H__ */
