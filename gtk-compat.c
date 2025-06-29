#include <config.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include "rep-gtk.h"

/* This whole file is rated XXX. */

const gchar*
gtk_label_get_interp (GtkLabel *label)
{
  const gchar *str = gtk_label_get_text (label);
  return str;
}

/* cheap cop-out. */

static void
menu_popup_position (GtkMenu *menu, gint *xp, gint *yp, gboolean *p, gpointer data)
{
    gulong coded = (gulong) data;
    gint x = coded & 0xffff;
    gint y = coded >> 16;

    /* copied from gtkmenu.c:gtk_menu_position () */

    GtkRequisition requisition;
    gint screen_width = gdk_screen_width ();
    gint screen_height = gdk_screen_height ();

    gtk_widget_size_request (GTK_WIDGET (menu), &requisition);

    x = CLAMP (x - 2, 0, MAX (0, screen_width - requisition.width));
    y = CLAMP (y - 2, 0, MAX (0, screen_height - requisition.height));

    *xp = x;
    *yp = y;
}

void
gtk_menu_popup_interp (GtkMenu *menu,
		       GtkWidget *parent_menu_shell,
		       GtkWidget *parent_menu_item,
		       gint button,
		       guint32 activate_time,
		       repv position)
{
  GtkMenuPositionFunc func = 0;
  gpointer func_data = 0;

  if (rep_CONSP (position)
      && rep_INTP(rep_CAR(position)) && rep_INTP(rep_CDR(position)))
  {
      gulong coded = (rep_INT(rep_CAR(position))
		      | (rep_INT(rep_CDR(position)) << 16));

      func = menu_popup_position;
      func_data = (void *) coded;
  }

  gtk_menu_popup (menu, parent_menu_shell, parent_menu_item,
		  func, func_data, button, activate_time);
}

#if 1 /* || (GTK_MAJOR_VERSION < 2 || (GTK_MAJOR_VERSION == 2 && GTK_MINOR_VERSION < 4)) */
GtkWidget*
gtk_radio_menu_item_new_with_label_from_widget (GtkRadioMenuItem *group,
		                                                const gchar      *label)
{
	  GSList *g = group? gtk_radio_menu_item_get_group (group) : NULL;
	    return gtk_radio_menu_item_new_with_label (g, label);
}

GtkWidget*
gtk_radio_menu_item_new_with_mnemonic_from_widget (GtkRadioMenuItem *group,
		                                                   const gchar      *label)
{
	  GSList *g = group? gtk_radio_menu_item_get_group (group) : NULL;
	    return gtk_radio_menu_item_new_with_mnemonic (g, label);
}

GtkWidget*
gtk_radio_menu_item_new_from_widget (GtkRadioMenuItem *group)
{
	  GSList *g = group? gtk_radio_menu_item_get_group (group) : NULL;
	    return gtk_radio_menu_item_new (g);
}
#endif /* < 2.4 */

#if 0
GtkWidget*
gtk_pixmap_new_interp (gchar *file,
		       GtkWidget *intended_parent)
{
  GtkStyle *style;
  GdkPixmap *pixmap;
  GdkBitmap *mask;

  style = gtk_widget_get_style (intended_parent);
  pixmap = gdk_pixmap_create_from_xpm (GDK_ROOT_PARENT(), &mask,
				       &style->bg[GTK_STATE_NORMAL],
				       file);
  return gtk_pixmap_new (pixmap, mask);
}
#endif

GdkColor*
gdk_color_parse_interp (char *spec)
{
  /* not reentrant */
  static GdkColor color;
  if (!gdk_color_parse (spec, &color))
    return NULL;
  return &color;
}

GdkColor*
gtk_style_get_white_interp (GtkStyle *style)
{
  return &style->white;
}

#ifndef HAVE_GTK_WIDGET_PEEK_COLORMAP
GdkColormap *
gtk_widget_peek_colormap ()
{
  return gtk_widget_get_default_colormap ();
}
#endif

#if 0
void
gtk_list_append_item (GtkList *list, GtkListItem *item)
{
  GList *items = g_list_alloc ();
  items->data = item;
  gtk_list_append_items (list, items);
}

void
gtk_list_prepend_item (GtkList *list, GtkListItem *item)
{
  GList *items = g_list_alloc ();
  items->data = item;
  gtk_list_prepend_items (list, items);
}
#endif

#if 0
gboolean
gtk_type_get_info (GtkType type, GtkTypeInfo *info)
{
  g_warning("Your version of Gtk+ does not support gtk_type_get_info");
  return FALSE;
}
#endif

#if 0
void
gtk_signal_set_class_function_full (GtkType            type,
				    const gchar       *signal,
				    GtkSignalFunc      func,
				    GtkCallbackMarshal marshal,
				    gpointer           data,
				    GtkDestroyNotify   destroy_func)
{
  g_warning("Your version of Gtk+ does not support"
	    " gtk_signal_set_class_function_full");
}
#endif

#if 0
void
gtk_color_selection_set_color_interp (GtkColorSelection *selection, GdkColor *color)
{
  gdouble vals[4];

  vals[0] = color->red / 65535.0;
  vals[1] = color->green / 65535.0;
  vals[2] = color->blue / 65535.0;
  vals[3] = 1.0;

  gtk_color_selection_set_color (selection, vals);
}
#endif

void
gtk_color_selection_set_color_interp (GtkColorSelection *selection, GdkColor *color)
{
  gtk_color_selection_set_current_color(selection, color);
}

GdkColor *
gtk_color_selection_get_color_interp (GtkColorSelection *selection)
{
  GdkColor *color;

  gtk_color_selection_get_current_color (selection, color);

  return color;
}

char *
gtk_color_button_get_color_interp (GtkColorButton *button)
{
  GdkColor color;
  gchar *str;

  gtk_color_button_get_color (button, &color);

  str = g_strdup_printf ("#%02x%02x%02x", color.red >> 8,
		  color.green >> 8, color.blue >> 8);

  return str;
}

void
gtk_widget_draw_interp (GtkWidget *widget)
{
    //gtk_widget_draw (widget, NULL);
}

/* status icon hack */

void
gtk_status_icon_popup_menu(GtkStatusIcon *status_icon,
			   GtkMenu *menu,
			   guint button,
			   guint32 activate_time)
{
     GtkMenuPositionFunc pos_func = gtk_status_icon_position_menu;
     gpointer user_data = status_icon;
     gtk_menu_popup (menu, NULL, NULL,
		     pos_func, user_data, button, activate_time);
}

gboolean
gtk_status_icon_get_geometry_interp(GtkStatusIcon *status_icon,
				    gint *x,
				    gint *y,
				    GtkOrientation *orientation)
{
     GdkRectangle area;
     gboolean ret = gtk_status_icon_get_geometry(status_icon,
					     NULL,
					     &area,
					     orientation);
     if(!ret)
	  return ret;
     *x = area.x;
     *y = area.y;
     return ret;
}
