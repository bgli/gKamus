/*
 *  gui.h, gKamus (http://gkamus.sourceforge.net)
 */

#ifndef __GUI_H__
#define __GUI_H__

#include <gtk/gtk.h>

#include "main.h"

enum
{
  COLUMN_WORD,
  COLUMN_N
};

extern GtkWidget *window_main;
extern GtkWidget *menu_save;
extern GtkWidget *menu_autosearch;
extern GtkWidget *menu_tool_ei;
extern GtkWidget *menu_tool_ie;
extern GtkWidget *entry_search;
extern GtkWidget *treev_word;
extern GtkWidget *textv_definition;
extern GtkWidget *window_modify;
extern GtkWidget *entry_mod;
extern GtkWidget *textv_mod;
extern GtkWidget *window_alpha;
extern GtkWidget *window_verb;
extern GtkWidget *treev_verb;
extern GtkWidget *window_tenses;
extern GtkWidget *textv_tenses;

void set_sensitive (GtkWidget*, gboolean);
gint create_dialog (GtkMessageType, gpointer, const gchar*, const gchar*, ...);
GtkWidget* create_window_main (void);
GtkWidget* create_window_modify (GkamusModifyMode);
GtkWidget* create_window_alpha (void);
GtkWidget* create_window_verb (void);
GtkWidget* create_window_tenses (void);
GtkWidget* create_dialog_about (void);
GtkWidget* create_dialog_file (gpointer, GtkFileChooserAction);

#endif /* __GUI_H__ */

