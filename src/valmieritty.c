#include <gtk/gtk.h>
#include <vte/vte.h>

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Valmieritty");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 500);

    // CSS para forzar bordes cuadrados y el borde industrial
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "window { border-radius: 0px !important; border: 2px solid #333333; background-color: #121212; }\n"
        "terminal { padding: 10px; }\n", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GtkWidget *terminal = vte_terminal_new();

    // Colores del Rice
    GdkRGBA bg, fg, cur;
    gdk_rgba_parse(&bg, "#121212");
    gdk_rgba_parse(&fg, "#ffffff");
    gdk_rgba_parse(&cur, "#00ffff");

    vte_terminal_set_colors(VTE_TERMINAL(terminal), &fg, &bg, NULL, 0);
    vte_terminal_set_color_cursor(VTE_TERMINAL(terminal), &cur);
    vte_terminal_set_cursor_shape(VTE_TERMINAL(terminal), VTE_CURSOR_SHAPE_BLOCK);
    vte_terminal_set_cursor_blink_mode(VTE_TERMINAL(terminal), VTE_CURSOR_BLINK_OFF);

    // Fuente
    PangoFontDescription *font_desc = pango_font_description_from_string("Terminess Nerd Font 11.3");
    vte_terminal_set_font(VTE_TERMINAL(terminal), font_desc);
    pango_font_description_free(font_desc);

    // Configuración del Shell
    gchar **envv = g_get_environ();
    const gchar *shell_path = g_environ_getenv(envv, "SHELL");
    if (!shell_path) shell_path = "/bin/bash";

    // Arreglo de los warnings de 'const'
    char *command[] = {(char *)shell_path, NULL};

    // La función spawn_async corregida con TODOS sus argumentos (VTE 0.48+)
    vte_terminal_spawn_async(
        VTE_TERMINAL(terminal),
        VTE_PTY_DEFAULT,
        NULL,         // working directory
        command,      // argv
        envv,         // envv
        G_SPAWN_DEFAULT,
        NULL, NULL,   // child setup
        NULL,         // child setup data destroy
        -1,           // timeout (aquí faltaba el -1)
        NULL,         // cancellable
        NULL, NULL    // callback y user_data
    );

    g_strfreev(envv);

    g_signal_connect(terminal, "child-exited", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_container_add(GTK_CONTAINER(window), terminal);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
