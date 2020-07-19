#include <carbontray/tray.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <inttypes.h>

typedef struct {
  int bar_width;
  int bar_height;
  int bar_icon_size;
  int bar_icon_spacing;
  char *bar_position;
} Config;

typedef struct {
  long left, right, top, bottom, left_start_y, left_end_y, right_start_y,
      right_end_y, top_start_x, top_end_x, bottom_start_x, bottom_end_x;
} Struts;

Struts *struts_new() {
  long struts[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  return g_memdup(&struts, 12 * sizeof(long));
}

void struts_destory(Struts *struts) { g_free(struts); }

Config *config_default_new() {
  Config *config = g_malloc(sizeof(Config));

  config->bar_width = 100;
  config->bar_height = 25;
  config->bar_icon_size = 16;
  config->bar_icon_spacing = 4;
  config->bar_position = "top-right";

  return config;
}

void config_destroy(Config *config) {
  g_free(config->bar_position);
  g_free(config);
}

GdkPoint *get_coordinates(GtkWidget *win, Config *config) {
  GdkPoint *point = g_malloc(sizeof(GdkPoint));

  GdkRectangle *geometry = g_malloc(sizeof(GdkRectangle));
  gdk_monitor_get_geometry(
      gdk_display_get_primary_monitor(
          gdk_screen_get_display(gtk_window_get_screen((GTK_WINDOW(win))))),
      geometry);

  // initialise with top-right by default
  point->x = geometry->width - config->bar_width;
  point->y = 0;

  if (g_strcmp0(config->bar_position, "top-left") == 0 ||
      g_strcmp0(config->bar_position, "top-right")) {
    point->y = 0;
  }

  if (g_strcmp0(config->bar_position, "bottom-left") == 0 ||
      g_strcmp0(config->bar_position, "bottom-right") == 0) {
    point->y = geometry->height - config->bar_height;
  }

  if (g_strcmp0(config->bar_position, "top-right") == 0 ||
      g_strcmp0(config->bar_position, "bottom-right") == 0) {
    point->x = geometry->width - config->bar_width;
  }

  if (g_strcmp0(config->bar_position, "top-left") == 0 ||
      g_strcmp0(config->bar_position, "bottom-left") == 0) {
    point->x = 0;
  }

  g_free(geometry);

  return point;
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

void setup_struts(GtkWidget *win, Config *config, int scale_factor) {
  GdkWindow *gdk_window = gtk_widget_get_window(win);

  g_message("X11 window id: 0x%" PRIx64 "", GDK_WINDOW_XID(gdk_window));

  Struts *struts = struts_new();

  struts->top = config->bar_height * scale_factor;
  struts->top_start_x = 3840 - config->bar_width * scale_factor;
  struts->top_end_x = 3840 - 1;

  gdk_property_change(gdk_window, gdk_atom_intern("_NET_WM_STRUT", false),
                      gdk_atom_intern("CARDINAL", false), 32,
                      GDK_PROP_MODE_REPLACE, (const guchar *)struts, 4);

  gdk_property_change(gdk_window,
                      gdk_atom_intern("_NET_WM_STRUT_PARTIAL", false),
                      gdk_atom_intern("CARDINAL", false), 32,
                      GDK_PROP_MODE_REPLACE, (const guchar *)struts, 12);
  struts_destory(struts);
}

int main(int argc, char **argv) {
  GtkWidget *win;
  GdkScreen *screen;
  CarbonTray *tray;

  Config *config = config_default_new();

  GOptionEntry entries[] = {
      {"width", 'w', 0, G_OPTION_ARG_INT, &config->bar_width, "Bar width", "w"},
      {"height", 'h', 0, G_OPTION_ARG_INT, &config->bar_height, "Bar height",
       "h"},
      {"icon-size", 'i', 0, G_OPTION_ARG_INT, &config->bar_icon_size,
       "Bar icon size", "i"},
      {"icon-spacing", 's', 0, G_OPTION_ARG_INT, &config->bar_icon_spacing,
       "Bar icon spacing", "s"},
      {"position", 'p', 0, G_OPTION_ARG_STRING, &config->bar_position,
       "Bar position"
       "p"},
      {NULL}};

  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new("- system tray");

  g_option_context_add_main_entries(context, entries, NULL);
  g_option_context_add_group(context, gtk_get_option_group(TRUE));

  if (!g_option_context_parse(context, &argc, &argv, &error)) {
    g_print("Option parsing failed: %s\n", error->message);
    exit(1);
  }

  g_free(context);

  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_title(GTK_WINDOW(win), "carbontray");
  gtk_widget_set_name(GTK_WIDGET(win), "carbontray");

  screen = gtk_widget_get_screen(win);

  int scale_factor = gdk_monitor_get_scale_factor(
      gdk_display_get_primary_monitor(gdk_screen_get_display(screen)));
  g_message("GTK scale factor: %d", scale_factor);

  load_user_stylesheet(screen);

  GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
  gtk_widget_set_visual(GTK_WIDGET(win), visual);

  g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  tray = carbon_tray_new(GTK_ORIENTATION_HORIZONTAL, config->bar_icon_size,
                         config->bar_icon_spacing);

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

  gtk_window_resize(GTK_WINDOW(win), config->bar_width, config->bar_height);
  gtk_widget_set_size_request(GTK_WIDGET(win), config->bar_width,
                              config->bar_height);

  GdkPoint *coordinates = get_coordinates(win, config);

  gtk_window_move(GTK_WINDOW(win), coordinates->x, coordinates->y);

  g_free(coordinates);

  setup_struts(win, config, scale_factor);

  config_destroy(config);

  gtk_main();
}
