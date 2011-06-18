/*
 *  gui.c, gKamus (http://gkamus.sourceforge.net)
 *  Copyright (C) 2008-2011, Ardhan Madras <ajhwb@knac.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <gtk/gtk.h>

#include <stdlib.h>

#include "main.h"
#include "gui.h"
#include "callback.h"
#include "function.h"


#define UI_FILE "gkamus.ui"

/* window_main */
GtkWidget *window_main = NULL;
GtkWidget *menu_save = NULL;
GtkWidget *menu_autosearch = NULL;
GtkWidget *menu_tool_ei = NULL;
GtkWidget *menu_tool_ie = NULL;
GtkWidget *entry_search = NULL;
GtkWidget *treev_word = NULL;
GtkWidget *textv_definition = NULL;
/* window_modify */
GtkWidget *window_modify = NULL;
GtkWidget *entry_mod = NULL;
GtkWidget *textv_mod = NULL;
/* window_alpha */
GtkWidget *window_alpha = NULL;
/* window_verb */
GtkWidget *window_verb = NULL;
GtkWidget *treev_verb = NULL;
/* window_tenses */
GtkWidget *window_tenses = NULL;
GtkWidget *combob_tenses = NULL;
GtkWidget *textv_tenses = NULL;

static GtkBuilder *builder = NULL;

static void builder_init (void);
static void builder_destroy (void);
static GtkWidget* get_widget (const gchar*);
static void cell_data_func (GtkTreeViewColumn*, GtkCellRenderer*, 
                GtkTreeModel*, GtkTreeIter*, gpointer);
static gboolean set_icon (GtkWindow*);

/*
 * Inisialisasi GtkBuilder.
 */
static void
builder_init (void)
{
  gchar *ui;
  gboolean test;
  guint res;

#ifdef PACKAGE_DATA_DIR
  ui = g_build_filename (PACKAGE_DATA_DIR, UI_FILE, NULL);
#else
  gchar *tmp = g_get_current_dir ();
  ui = g_build_filename (tmp, "share", "data", UI_FILE, NULL);
  g_free (tmp);
#endif
  test = g_file_test (ui, G_FILE_TEST_IS_REGULAR | G_FILE_TEST_EXISTS);
  if (!test)
    {
      g_free (ui);
      g_critical ("Interface file not found.\n");
      create_dialog (GTK_MESSAGE_ERROR, NULL, "Error", "File interface gKamus "
                     "tidak ditemukan.");
      exit (EXIT_FAILURE);
    }
  builder = gtk_builder_new ();
  res = gtk_builder_add_from_file (builder, ui, NULL);
  g_free (ui);
  if (!res)
    {
      g_critical ("Error in interface file.\n");
      create_dialog (GTK_MESSAGE_ERROR, NULL, "Error", "Kesalahan dengan file "
                     "interface gKamus.");
      exit (EXIT_FAILURE);
    }
}

/*
 * Dereferensi GtkBuilder.
 */
static void
builder_destroy (void)
{
  g_object_unref (builder);
}

/*
 * Ambil widget dari GtkBuilder.
 */
static GtkWidget*
get_widget (const gchar *name)
{
  GtkWidget *widget;
  
  g_return_val_if_fail (name != NULL || builder != NULL, NULL);
  widget = (GtkWidget *) gtk_builder_get_object (builder, name);
  if (!widget)
    {
      g_critical ("Could not find a widget named \"%s\".\n", name);
      create_dialog (GTK_MESSAGE_ERROR, NULL, "Error", "Kesalahan dengan "
                     "file interface gKamus.");
      exit (EXIT_FAILURE);
    }
  return widget;
}

/*
 * GtkTreeCellDataFunc untuk merubah warna cell.
 * TODO: Gunakan style property "even-row-color" GtkTreeView.
 */
static void
cell_data_func (GtkTreeViewColumn *column,
                GtkCellRenderer   *renderer,
                GtkTreeModel      *model,
                GtkTreeIter       *iter,
                gpointer           data)
{
  GtkTreePath *path;
  gint *id;
  
  path = gtk_tree_model_get_path (model, iter);
  id = gtk_tree_path_get_indices (path);
  g_object_set (renderer, "cell-background", *id % 2 ? "#ededed" : "#ffffff", NULL);
  gtk_tree_path_free (path);
}

/*
 * Dengan GtkBuilder pada versi gtk+ 2.12.8, gtk_widget_set_sensitive() 
 * tidak bekerja untuk widget-widget tertentu seperti GtkMenuItem, pesan
 * assertion gagal GTK_IS_WIDGET (widget). Ini mungkin kesalahan di
 * gtk_builder_get_object().
 */
void
set_sensitive (GtkWidget *widget,
               gboolean   state)
{
  g_object_set (widget, "sensitive", state, NULL);
}

static gboolean
set_icon (GtkWindow *window)
{
  g_return_val_if_fail (window != NULL, FALSE);

  gchar *icon;
  gboolean ret;

#ifdef PACKAGE_PIXMAPS_DIR
  icon = g_build_filename (PACKAGE_PIXMAPS_DIR, "gkamus.png", NULL);
#else
  icon = g_build_filename ("share", "pixmaps", "gkamus.png", NULL);
#endif

  ret = gtk_window_set_icon_from_file (window, icon, NULL);
  g_free (icon);

  return ret;
}

/*
 * Wrapper gtk_message_dialog_new().
 */
gint
create_dialog (GtkMessageType  type, 
               gpointer        parent, 
               const gchar    *head,
               const gchar    *msg, 
               ...)
{
    g_return_val_if_fail (type >= GTK_MESSAGE_INFO &&
                          type <= GTK_MESSAGE_OTHER, GTK_RESPONSE_NONE);
    g_return_val_if_fail (msg != NULL, GTK_RESPONSE_NONE);

    static const gchar * const title[] =
    {
        "Informasi",
        "Peringatan",
        "Konfirmasi",
        "Error",
        "gKamus"
    };
    GtkWidget *dialog;
    gchar *tmp;
    gint ret;
    va_list args;

    va_start (args, msg);
    tmp = g_strdup_vprintf (msg, args);
    va_end (args);
    dialog = gtk_message_dialog_new ((GtkWindow *) parent,
                                     GTK_DIALOG_MODAL, type, GTK_BUTTONS_NONE,
                                     "%s", (head != NULL ? head : tmp));
    if (head)
        gtk_message_dialog_format_secondary_text 
            ((GtkMessageDialog *) dialog, "%s", tmp);
    switch (type)
    {
        case GTK_MESSAGE_QUESTION:
          gtk_dialog_add_buttons ((GtkDialog *) dialog, 
                                  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, 
                                  GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
          break;
        default:
          if (type == GTK_MESSAGE_OTHER)
          {
            GtkWidget *img = gtk_image_new_from_stock ("gtk-dialog-info",
                                                       GTK_ICON_SIZE_DIALOG);
            if (img != NULL)
            {
                gtk_message_dialog_set_image ((GtkMessageDialog *) dialog, img);
                gtk_widget_show (img);
            }
          }
          gtk_dialog_add_button ((GtkDialog *) dialog, "gtk-ok",
                                 GTK_RESPONSE_OK);
    }
    gtk_window_set_title ((GtkWindow *) dialog, title[type]);
    if (parent == NULL)
        gtk_window_set_position ((GtkWindow *) dialog, GTK_WIN_POS_CENTER);
    if ((ret = gtk_dialog_run ((GtkDialog *) dialog)) != GTK_RESPONSE_NONE)
        gtk_widget_destroy (dialog);
    g_free (tmp);

    return ret;
}

/*
 * Buat window_main.
 */
GtkWidget*
create_window_main (void)
{
  builder_init ();

  GtkWidget *window;
  GtkWidget *menu_quit;
  GtkWidget *menu_copy;
  GtkWidget *menu_paste;
  GtkWidget *menu_alpha;
  GtkWidget *menu_verb;
  GtkWidget *menu_tenses;
  GtkWidget *menu_about;
  GtkWidget *label_search;
  GtkWidget *vbox_search;
  GtkWidget *button_paste;
  GtkWidget *button_find;
  GtkListStore *store;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeSelection *selection;

  window = get_widget ("window_main");
  set_icon ((GtkWindow *) window);
  gtk_window_set_title ((GtkWindow *) window, PROGRAM_NAME);
  g_signal_connect ((GObject *) window, "delete-event",
                    (GCallback) on_window_main_delete_event, NULL);

#if 0
  menu_save = get_widget ("menu_save");
  set_sensitive (menu_save, FALSE);
  g_signal_connect ((GObject *) menu_save, "activate",
                    (GCallback) on_menu_save_activate, NULL);
  
  menu_saveas = get_widget ("menu_saveas");
  g_signal_connect ((GObject *) menu_saveas, "activate",
                    (GCallback) on_menu_saveas_activate, NULL);
#endif

  menu_quit = get_widget ("menu_quit");
  g_signal_connect ((GObject *) menu_quit, "activate",
                    (GCallback) on_menu_quit_activate, NULL);
  
  menu_copy = get_widget ("menu_copy");
  g_signal_connect ((GObject *) menu_copy, "activate",
                    (GCallback) on_menu_copy_activate, NULL);
  
  menu_paste = get_widget ("menu_paste");
  g_signal_connect ((GObject *) menu_paste, "activate",
                    (GCallback) on_button_paste_clicked, NULL);

#if 0
  menu_add = get_widget ("menu_add");
  g_signal_connect ((GObject *) menu_add, "activate",
                    (GCallback) on_menu_add_activate, NULL);
  
  menu_edit = get_widget ("menu_edit");
  g_signal_connect ((GObject *) menu_edit, "activate",
                    (GCallback) on_menu_edit_activate, NULL);

  menu_delete = get_widget ("menu_delete");
  g_signal_connect ((GObject *) menu_delete, "activate",
                    (GCallback) on_menu_delete_activate, NULL);
#endif

  menu_tool_ei = get_widget ("menu_tool_ei");
  g_signal_connect ((GObject *) menu_tool_ei, "activate",
                    (GCallback) on_menu_tool_ei_activate, NULL);
  
  menu_tool_ie = get_widget ("menu_tool_ie");
  g_signal_connect ((GObject *) menu_tool_ie, "activate",
                    (GCallback) on_menu_tool_ie_activate, NULL);
                    
  menu_autosearch = get_widget ("menu_autosearch");
  
  menu_alpha = get_widget ("menu_alpha");
  g_signal_connect ((GObject *) menu_alpha, "activate",
                    (GCallback) on_menu_alpha_activate, NULL);
                    
  menu_verb = get_widget ("menu_verb");
  g_signal_connect ((GObject *) menu_verb, "activate",
                    (GCallback) on_menu_verb_activate, NULL);

  menu_tenses = get_widget ("menu_tenses");
  g_signal_connect ((GObject *) menu_tenses, "activate",
                    (GCallback) on_menu_tenses_activate, NULL);

#if 0
  menu_short = get_widget ("menu_short");

#ifdef G_OS_UNIX /* Shortcut hanya untuk UNIX */
  GtkWidget *menu_short_desktop;
  GtkWidget *menu_short_menu;
  
  menu_short_desktop = get_widget ("menu_short_desktop");
  g_signal_connect ((GObject *) menu_short_desktop, "activate",
                    (GCallback) on_menu_short_desktop_activate, NULL);
  
  menu_short_menu = get_widget ("menu_short_menu");
  g_signal_connect ((GObject *) menu_short_menu, "activate",
                    (GCallback) on_menu_short_menu_activate, NULL);
#else
  g_object_set (menu_short, "visible", FALSE, NULL);
#endif
#endif

  menu_about = get_widget ("menu_about");
  g_signal_connect ((GObject *) menu_about, "activate",
                    (GCallback) on_menu_about_activate, NULL);
  
  vbox_search = get_widget ("vbox_search");

  entry_search = gtk_entry_new ();
  gtk_entry_set_icon_from_stock ((GtkEntry *) entry_search,
                                 GTK_ENTRY_ICON_SECONDARY,
                                 GTK_STOCK_CLEAR);
  gtk_entry_set_activates_default ((GtkEntry *) entry_search, TRUE);
  gtk_entry_set_max_length ((GtkEntry *) entry_search, 20);
  gtk_container_add ((GtkContainer *) vbox_search, entry_search);
  g_signal_connect ((GObject *) entry_search, "icon-press",
                    (GCallback) on_entry_search_icon_pressed, NULL);
  g_signal_connect ((GObject *) entry_search, "changed",
                    (GCallback) on_entry_search_changed, NULL);

  label_search = get_widget ("label_searcx");
  gtk_label_set_mnemonic_widget ((GtkLabel *) label_search, entry_search);

  button_paste = get_widget ("button_paste");
  g_signal_connect ((GObject *) button_paste, "clicked",
                    (GCallback) on_button_paste_clicked, NULL);
  
  button_find = get_widget ("button_find");
  g_signal_connect ((GObject *) button_find, "clicked",
                    (GCallback) on_button_find_clicked, NULL);

  store = gtk_list_store_new (COLUMN_N, G_TYPE_STRING);
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Word", renderer,
                                                     "text", COLUMN_WORD, NULL);
  
  treev_word = get_widget ("treev_word");
  gtk_tree_view_append_column ((GtkTreeView *) treev_word, column);
  gtk_tree_view_set_model ((GtkTreeView *) treev_word, (GtkTreeModel *) store);
  
  selection = gtk_tree_view_get_selection ((GtkTreeView *) treev_word);
  g_signal_connect ((GObject *) selection, "changed",
                    (GCallback) on_treev_word_columns_changed, NULL);
  
  textv_definition = get_widget ("textv_definition");

  builder_destroy ();

  return window;
}

/*
 * Buat window_modify.
 */
GtkWidget*
create_window_modify (GkamusModifyMode mode)
{
  g_return_val_if_fail (mode >= GKAMUS_MODIFY_ADD &&
                        mode <= GKAMUS_MODIFY_EDIT, NULL);
  builder_init ();

  GtkWidget *window;
  GtkWidget *button_mod_ok;
  GtkWidget *img_ok;
  GtkWidget *button_mod_cl;
  GtkWidget *img_cl;
  GtkTextBuffer *textv_mod_buffer;

  window = get_widget ("window_modify");
  gtk_window_set_type_hint ((GtkWindow *) window, GDK_WINDOW_TYPE_HINT_DIALOG);
  gtk_window_set_transient_for ((GtkWindow *) window, (GtkWindow *) window_main);
  gtk_window_set_title ((GtkWindow *) window, mode == GKAMUS_MODIFY_ADD ? 
                        "Tambah Kata" : "Edit Kata");
  set_icon ((GtkWindow *) window);
  g_signal_connect ((GObject *) window, "destroy", 
                    (GCallback) on_window_modify_destroy, NULL);
    
  entry_mod = get_widget ("entry_mod");

  textv_mod = get_widget ("textv_mod");
  textv_mod_buffer = gtk_text_view_get_buffer ((GtkTextView *) textv_mod);
  g_signal_connect ((GObject *) textv_mod_buffer, "changed",
                    (GCallback) on_textv_mod_buffer_changed, NULL);

  button_mod_ok = get_widget ("button_mod_ok");
  gtk_button_set_label ((GtkButton *) button_mod_ok, "_OK");
  img_ok = gtk_image_new_from_stock (GTK_STOCK_OK, GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image ((GtkButton *) button_mod_ok, img_ok);
  g_signal_connect ((GObject *) button_mod_ok, "clicked",
                    (GCallback) on_button_mod_ok_clicked, GINT_TO_POINTER (mode));
  g_signal_connect ((GObject *) textv_mod, "key-press-event",
                    (GCallback) on_textv_mod_key_press_event, button_mod_ok);

  button_mod_cl = get_widget ("button_mod_cl");
  img_cl = gtk_image_new_from_stock (GTK_STOCK_CANCEL, GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image ((GtkButton *) button_mod_cl, img_cl);
  g_signal_connect ((GObject *) button_mod_cl, "clicked",
                    (GCallback) on_window_modify_destroy, NULL);

  builder_destroy ();

  return window;
}

/*
 * Buat window_alpha.
 */
GtkWidget*
create_window_alpha (void)
{
  builder_init ();
  
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *textv;
  GtkTextBuffer *buf;
  
  const gchar alpha[] =
  (
    "A  \t  - \t  el   \t\t\t  N  \t  - \t  en     \n"
    "B  \t  - \t  bi:  \t\t\t  O  \t  - \t  ou     \n"
    "C  \t  - \t  si:  \t\t\t  P  \t  - \t  pi     \n"
    "D  \t  - \t  di:  \t\t\t  Q  \t  - \t  kyu    \n"
    "E  \t  - \t  i:   \t\t\t  R  \t  - \t  a:     \n"
    "F  \t  - \t  ef   \t\t\t  S  \t  - \t  es     \n"
    "G  \t  - \t  ji   \t\t\t  T  \t  - \t  ti:    \n"
    "H  \t  - \t  eitc \t\t\t  U  \t  - \t  you    \n"
    "I  \t  - \t  ai   \t\t\t  V  \t  - \t  vi:    \n"
    "J  \t  - \t  jei  \t\t\t  W  \t  - \t  dablyu \n"
    "K  \t  - \t  kei  \t\t\t  X  \t  - \t  eks    \n"
    "L  \t  - \t  el   \t\t\t  Y  \t  - \t  wai    \n"
    "M  \t  - \t  em   \t\t\t  Z  \t  - \t  sed      "
  );
  
  gint i = 0, j = 0;
  gchar tmp[1024]; /* so you can clearly read the alpha string ;[ */

  window = get_widget ("window_alpha");
  gtk_window_set_title ((GtkWindow *) window, "English Alphabet List");
  set_icon ((GtkWindow *) window);
  g_signal_connect ((GObject *) window, "delete-event", 
                    (GCallback) on_window_alpha_delete_event, NULL);

  button = get_widget ("button_alpha");
  g_signal_connect ((GObject *) button, "clicked", 
                    (GCallback) on_window_alpha_delete_event, NULL);

  textv = get_widget ("textv_alpha");
  buf = gtk_text_view_get_buffer ((GtkTextView *) textv);
  while (alpha[i])
    {
      if (alpha[i] != ' ')
        tmp[j++] = alpha[i];
      i++;
    }
  tmp[j] = 0;
  gtk_text_buffer_set_text (buf, tmp, -1);
  
  builder_destroy ();

  return window;
}

/*
 * Buat window_verb. 
 */
GtkWidget*
create_window_verb (void)
{
  builder_init ();
  
  GtkWidget *window;
  GtkWidget *button_verb;
  GtkListStore *store;
  GtkCellRenderer *render_present;
  GtkCellRenderer *render_past;
  GtkCellRenderer *render_participle;
  GtkTreeViewColumn *column_present;
  GtkTreeViewColumn *column_past;
  GtkTreeViewColumn *column_participle;
  
  window = get_widget ("window_verb");
  gtk_window_set_title ((GtkWindow *) window, "Irregular Verbs");
  set_icon ((GtkWindow *) window);
  g_signal_connect ((GObject *) window, "destroy",
                    (GCallback) on_window_verb_destroy, NULL);

  store = gtk_list_store_new (3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  render_present = gtk_cell_renderer_text_new ();
  column_present = gtk_tree_view_column_new_with_attributes ("Present Tense",
                        render_present, "text", 0, NULL);
  gtk_tree_view_column_set_expand (column_present, TRUE);
  gtk_tree_view_column_set_cell_data_func (column_present, render_present,
        (GtkTreeCellDataFunc) cell_data_func, NULL, NULL);
      
  render_past = gtk_cell_renderer_text_new ();
  column_past = gtk_tree_view_column_new_with_attributes ("Past Tense",
                    render_past, "text", 1, NULL);
  gtk_tree_view_column_set_expand (column_past, TRUE);
  gtk_tree_view_column_set_cell_data_func (column_past, render_past,
        (GtkTreeCellDataFunc) cell_data_func, NULL, NULL);

  render_participle = gtk_cell_renderer_text_new ();
  column_participle = gtk_tree_view_column_new_with_attributes ("Past Participle",
                          render_participle, "text", 2, NULL);
  gtk_tree_view_column_set_expand (column_participle, TRUE);
  gtk_tree_view_column_set_cell_data_func (column_participle, render_participle,
        (GtkTreeCellDataFunc) cell_data_func, NULL, NULL);
  
  treev_verb = get_widget ("treev_verb");
  gtk_tree_view_set_model ((GtkTreeView *) treev_verb, (GtkTreeModel *) store);
  gtk_tree_view_append_column ((GtkTreeView *) treev_verb, column_present);
  gtk_tree_view_append_column ((GtkTreeView *) treev_verb, column_past);
  gtk_tree_view_append_column ((GtkTreeView *) treev_verb, column_participle);
  
  button_verb = get_widget ("button_verb");
  g_signal_connect ((GObject *) button_verb, "clicked",
                    (GCallback) on_window_verb_destroy, NULL);
                    
  builder_destroy ();

  return window;
}

/*
 * Buat window_tenses.
 */
GtkWidget*
create_window_tenses (void)
{
  builder_init ();

  GtkWidget *window;
  GtkWidget *button_tenses;
  GtkTreeModel *model;
  
  gint i = 0;
  static const gchar * const tenses[] =
  {
    "Simple Present",
    "Present Continuous",
    "Simple Past",
    "Past Continuous",
    "Present Perfect",
    "Present Perfect Continuous",
    "Past Perfect",
    "Past Perfect Continuous",
    "Simple Future",
    "Future Continuous",
    "Future Perfect",
    "Future Perfect Continuous"
  };
  
  window = get_widget ("window_tenses");
  gtk_window_set_title ((GtkWindow *) window, "English Tenses");
  set_icon ((GtkWindow *) window);
  g_signal_connect ((GObject *) window, "destroy",
                    (GCallback) on_window_tenses_destroy, NULL);

  combob_tenses = get_widget ("combob_tenses");
  model = gtk_combo_box_get_model ((GtkComboBox *) combob_tenses);
  gtk_list_store_clear ((GtkListStore *) model);
  do {
    gtk_combo_box_insert_text ((GtkComboBox *) combob_tenses, i, tenses[i]);
  } while (++i < G_N_ELEMENTS (tenses));

  textv_tenses = get_widget ("textv_tenses");
  
  g_signal_connect ((GObject *) combob_tenses, "changed",
                    (GCallback) on_combob_tenses_changed, NULL);
  gtk_combo_box_set_active ((GtkComboBox *) combob_tenses, 0);

  button_tenses = get_widget ("button_tenses");
  g_signal_connect ((GObject *) button_tenses, "clicked",
                    (GCallback) on_window_tenses_destroy, NULL);

  builder_destroy ();
  
  return window;
}

/* 
 * Buat GtkAboutDialog.
 */
GtkWidget*
create_dialog_about (void)
{
  GtkWidget *dialog;
  GdkPixbuf *pix;

  /* set properti dialog_about */
  static const gchar copyright[] =
  (
    "Copyright \302\251 2008 - 2011 Ardhan Madras"
  );

  static const gchar license[] =
  (
    "gKamus is free software; you can redistribute it and/or modify it "
    "under the terms of the GNU General Public License version 2 as "
    "published by the Free Software Foundation.\n\n"
    "gKamus is distributed in the hope that it will be useful, but "
    "WITHOUT ANY WARRANTY; without even the implied warranty of "
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU "
    "General Public License for more details.\n\n"
    "You should have received a copy of the GNU General Public License "
    "along with gKamus; if not, write to the Free Software Foundation, "
    "Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA."
  );

  static const gchar comments[] =
  (
    PROGRAM_DESC
#ifdef G_OS_WIN32
    "\nGCC " __VERSION__   /* versi mingw jika di windows :p */
#endif
  );

  static const gchar *authors[] =
  {
    "Ardhan Madras <ajhwb@knac.com>",
    NULL
  };

  dialog = gtk_about_dialog_new ();
  set_icon ((GtkWindow *) dialog);
  gtk_about_dialog_set_name ((GtkAboutDialog *) dialog, PROGRAM_NAME);
  gtk_about_dialog_set_version ((GtkAboutDialog *) dialog, PROGRAM_VERS);
  gtk_about_dialog_set_copyright ((GtkAboutDialog *) dialog, copyright);
  gtk_about_dialog_set_comments ((GtkAboutDialog *) dialog, comments);
  gtk_about_dialog_set_license ((GtkAboutDialog *) dialog, license);
  gtk_about_dialog_set_wrap_license ((GtkAboutDialog *) dialog, TRUE);
  gtk_about_dialog_set_website ((GtkAboutDialog *) dialog, PROGRAM_SITE);
  gtk_about_dialog_set_website_label ((GtkAboutDialog *) dialog, PROGRAM_SITE);

  gtk_about_dialog_set_authors ((GtkAboutDialog *) dialog, authors);
  gtk_window_set_transient_for ((GtkWindow *) dialog, (GtkWindow *) window_main);
    
#ifdef PACKAGE_PIXMAPS_DIR
  pix = gdk_pixbuf_new_from_file (g_build_filename (PACKAGE_PIXMAPS_DIR,
                                  "gkamus.png", NULL), NULL);
#else
  pix = gdk_pixbuf_new_from_file (g_build_filename ("share", "pixmaps",
                                  "gkamus.png", NULL), NULL);
#endif
  gtk_about_dialog_set_logo ((GtkAboutDialog *) dialog, pix);
  g_signal_connect ((GObject *) dialog, "destroy",
                    (GCallback) gtk_widget_destroy, NULL);
  g_object_unref (pix);

  return dialog;
}

/*
 * Wrapper gtk_file_chooser_dialog_new()
 */
GtkWidget*
create_dialog_file (gpointer             parent, 
                    GtkFileChooserAction action)
{
  g_return_val_if_fail (action >= GTK_FILE_CHOOSER_ACTION_OPEN &&
                        action <= GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER, NULL);

  GtkWidget *dialog;

  static const gchar * const title[] =
  {
    "Buka kamus",
    "Simpan kamus",
    "Pilih folder",
    "Buat folder",
  };

  dialog = gtk_file_chooser_dialog_new (title[action], (GtkWindow *) parent,
                                        action, GTK_STOCK_CANCEL,
                                        GTK_RESPONSE_CANCEL, GTK_STOCK_OK,
                                        GTK_RESPONSE_OK, NULL);
  set_icon ((GtkWindow *) dialog);
  if (action == GTK_FILE_CHOOSER_ACTION_OPEN || action == GTK_FILE_CHOOSER_ACTION_SAVE)
    {
      GtkFileFilter *filter = gtk_file_filter_new ();
      gtk_file_filter_set_name (filter, "gKamus Dictionary (*.dict)");
      gtk_file_filter_add_pattern (filter, "*.dict");
      gtk_file_chooser_add_filter ((GtkFileChooser *) dialog, filter);
    }
  return dialog;
}
