#include <X11/X.h>
#include <X11/Xlib.h>
#include <carbontray/tray.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <inttypes.h>

static int bar_height = 25;
static int bar_width = 100;
static int bar_icon_size = 16;
static int bar_spacing = 4;

static int screen_width = 1920;

void load_user_stylesheet(GdkScreen *screen);

int main(int argc, char **argv) {
  GtkWidget *win;
  GdkScreen *screen;
  CarbonTray *tray;

  gtk_init(&argc, &argv);
  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_title(GTK_WINDOW(win), "carbontray");
  gtk_widget_set_name(GTK_WIDGET(win), "carbontray");

  screen = gtk_widget_get_screen(win);

  load_user_stylesheet(screen);

  GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
  gtk_widget_set_visual(GTK_WIDGET(win), visual);

  g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  tray =
      carbon_tray_new(GTK_ORIENTATION_HORIZONTAL, bar_icon_size, bar_spacing);

  if (tray == NULL) {
    g_message("Failed to initialise the tray!");
    GtkWidget *label;
    label = gtk_label_new("Tray unavailable");
    gtk_widget_show(label);
    gtk_container_add(GTK_CONTAINER(win), label);
  } else {
    g_message("Tray initialised!");
    carbon_tray_add_to_container(tray, GTK_CONTAINER(win));
    carbon_tray_register(tray, screen);
  }

  gtk_widget_show_all(win);
  gtk_widget_queue_draw(win);

  gtk_window_set_type_hint(GTK_WINDOW(win), GDK_WINDOW_TYPE_HINT_DOCK);
  gtk_window_set_decorated(GTK_WINDOW(win), FALSE);
  gtk_window_set_skip_taskbar_hint(GTK_WINDOW(win), TRUE);
  gtk_window_set_keep_below(GTK_WINDOW(win), TRUE);
  gtk_window_stick(GTK_WINDOW(win));

  gtk_window_resize(GTK_WINDOW(win), bar_width, bar_height);
  gtk_widget_set_size_request(GTK_WIDGET(win), bar_width, bar_height);

  gtk_window_move(GTK_WINDOW(win), screen_width - bar_width, 0);

  Display *xdpy = GDK_SCREEN_XDISPLAY(screen);
  Window xwin = GDK_WINDOW_XID(gtk_widget_get_window(GTK_WIDGET(win)));
  Atom a_wm_strut = XInternAtom(xdpy, "_NET_WM_STRUT", false);
  Atom a_wm_strut_partial = XInternAtom(xdpy, "_NET_WM_STRUT_PARTIAL", false);
  Atom a_cardinal = XInternAtom(xdpy, "CARDINAL", false);

  g_message("X11 window id: 0x%" PRIx64 "", xwin);

  long struts[] = {0, 0, bar_height * 2, 0};
  long struts_partial[] = {0, 0, bar_height * 2,       0,        0, 0,
                           0, 0, 3840 - bar_width * 2, 3840 - 1, 0, 0};

  XChangeProperty(xdpy, xwin, a_wm_strut, a_cardinal, 32, PropModeReplace,
                  (unsigned char *)&struts, 4);
  XChangeProperty(xdpy, xwin, a_wm_strut_partial, a_cardinal, 32,
                  PropModeReplace, (unsigned char *)&struts_partial, 12);

  g_message("GTK scale factor: %d",
            gtk_widget_get_scale_factor(GTK_WIDGET(win)));

  g_message("X display size: %dx%d", XDisplayWidth(xdpy, 0),
            XDisplayHeight(xdpy, 0));

  gtk_main();
}

void load_user_stylesheet(GdkScreen *screen) {
  gchar *stylesheet_path = g_build_filename(g_get_user_config_dir(),
                                            "carbontray", "style.css", NULL);

  GError *err = NULL;
  GtkCssProvider *css_provider = gtk_css_provider_new();

  gtk_css_provider_load_from_path(css_provider, stylesheet_path, &err);
  if (err != NULL) {
    g_message("Loading CSS from %s failed: %s", stylesheet_path, err->message);
    g_error_free(err);
  } else {
    gtk_style_context_add_provider_for_screen(
        screen, (GtkStyleProvider *)css_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  }

  g_free(stylesheet_path);
}
