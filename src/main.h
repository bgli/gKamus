/*
 *  main.h, gKamus (http://gkamus.sourceforge.net)
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <gtk/gtk.h>

typedef struct
{
  gchar *word;
  gchar *definition;
} Dictionary;

typedef enum
{
  GKAMUS_MODIFY_ADD = 0,
  GKAMUS_MODIFY_EDIT,
} GkamusModifyMode;

typedef enum
{
  GKAMUS_DIC_EN = 0,
  GKAMUS_DIC_ID
} GkamusDicMode;

typedef enum
{
  GKAMUS_DIC_STATUS_ERROR = 0,
  GKAMUS_DIC_STATUS_NOT_FOUND,
  GKAMUS_DIC_STATUS_INVALID,
  GKAMUS_DIC_STATUS_MAX_ENTRY,
  GKAMUS_DIC_STATUS_OK
} GkamusDicStatus;

#define PROGRAM_NAME    "gKamus"
#define	PROGRAM_VERS    "1.0"
#define PROGRAM_DESC    "Simple English - Indonesia dictionary"
#define PROGRAM_SITE    "http://gkamus.sourceforge.net"
#define PROGRAM_ICON    "gkamus.png"
#define MAX_WORD        16
#define MAX_DEFINITION  256
#define MAX_ENTRY       100000

extern GList *dict;
extern gchar *dict_file;
extern const gchar const * dict_en;
extern const gchar const * dict_id;
extern gchar *prg_name;
extern const gchar * const dic_errmsg[];

#endif /* __MAIN_H__ */

