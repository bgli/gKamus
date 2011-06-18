/*
 *  callback.h, gKamus (http://gkamus.sourceforge.net)
 */

#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include <gtk/gtk.h>

gboolean on_window_main_delete_event (GtkWidget*, GdkEvent*, gpointer);
void on_entry_search_icon_pressed (GtkWidget*, GtkEntryIconPosition, GdkEvent*, gpointer);
void on_entry_search_changed (GtkWidget*, gpointer);
void on_window_modify_destroy (GtkWidget*, gpointer);
void on_button_mod_ok_clicked (GtkWidget*, gpointer);
gboolean on_textv_mod_key_press_event (GtkWidget*, GdkEventKey*, gpointer);
void on_textv_mod_buffer_changed (GtkTextBuffer*, gpointer);
void select_treev_word (gint);
void on_menu_save_activate (GtkWidget*, gpointer);
void on_menu_saveas_activate (GtkWidget*, gpointer);
void on_menu_quit_activate (GtkWidget*, gpointer);
void on_menu_copy_activate (GtkWidget*, gpointer);
void on_menu_paste_activate (GtkWidget*, gpointer);
void on_menu_add_activate (GtkWidget*, gpointer);
void on_menu_edit_activate (GtkWidget*, gpointer);
void on_menu_delete_activate (GtkWidget*, gpointer);
void on_menu_tool_ei_activate (GtkWidget*, gpointer);
void on_menu_tool_ie_activate (GtkWidget*, gpointer);
void on_menu_alpha_activate (GtkWidget*, gpointer);
void on_menu_verb_activate (GtkWidget*, gpointer);
void on_menu_tenses_activate (GtkWidget*, gpointer);
void on_menu_about_activate (GtkWidget *, gpointer);
void on_treev_word_columns_changed (GtkWidget *, gpointer);
void on_button_paste_clicked (GtkWidget *, gpointer);
void on_button_find_clicked (GtkWidget *, gpointer);
gboolean on_window_alpha_delete_event (GtkWidget*, GdkEvent*, gpointer);
void on_window_verb_destroy (GtkWidget*, gpointer);
void on_window_tenses_destroy (GtkWidget*, gpointer);
void on_combob_tenses_changed (GtkComboBox*, gpointer);

#endif /* __CALLBACK_H__ */

