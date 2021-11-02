#include "stdafx.h"

#include "platform.h"
#include "resource_monitor.h"
#include "widgets/button_1.h"
#include "res.h"

static void print_hello( GtkWidget* widget, gpointer data )
{
	g_print( "%s\n", gtk_button_get_label( GTK_BUTTON( widget ) ) );
}

static void quit_cb( GtkWindow* window )
{
	gtk_window_close( window );
}

static void reload_css( GtkCssProvider** provider )
{
	gtk_style_context_remove_provider_for_display( gdk_display_get_default(), GTK_STYLE_PROVIDER( *provider ) );
	g_object_unref( *provider );

	*provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path( *provider, "res/css/main.css" );
	gtk_style_context_add_provider_for_display(
	  gdk_display_get_default(), GTK_STYLE_PROVIDER( *provider ), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION );
}

static void reload_ui( GtkApplication* app )
{
	GtkBuilder* builder = gtk_builder_new();
	gtk_builder_expose_object( builder, "Button1", G_OBJECT( button_1_new() ) );

	GError* load_error = NULL;
	if ( !gtk_builder_add_from_file( builder, "res/ui/main.ui", &load_error ) ) {
		g_message( _( "%s" ), load_error->message );
		g_error_free( load_error );
	}

	GObject* window = gtk_builder_get_object( builder, "window" );
	gtk_window_set_application( GTK_WINDOW( window ), app );

	gtk_widget_show( GTK_WIDGET( window ) );
	g_object_unref( builder );
}

static void activate( GtkApplication* app, gpointer user_data )
{
	/* Construct a GtkBuilder instance and load our UI description */
	GtkBuilder* builder = gtk_builder_new();
	// 很是疑惑，为什么不是绑定 type，而是实例？
	gtk_builder_expose_object( builder, "Button1", G_OBJECT( button_1_new() ) );

	GError* load_error = NULL;
	if ( !gtk_builder_add_from_file( builder, "res/ui/main.ui", &load_error ) ) {
		g_error( _( "%s" ), load_error->message );
		g_error_free( load_error );
	}

	/* Connect signal handlers to the constructed widgets. */
	GObject* window = gtk_builder_get_object( builder, "window" );
	gtk_window_set_application( GTK_WINDOW( window ), app );

	GtkCssProvider* provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path( provider, "res/css/main.css" );

	gtk_style_context_add_provider_for_display(
	  gdk_display_get_default(), GTK_STYLE_PROVIDER( provider ), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION );

	GtkCssProvider** provider_p = g_new( GtkCssProvider*, 1 );
	*provider_p                 = provider;

	lx_resource_monitor( "res/css/main.css", G_CALLBACK( reload_css ), provider_p );
	lx_resource_monitor( "res/ui/main.ui", G_CALLBACK( reload_ui ), app );

	gtk_widget_show( GTK_WIDGET( window ) );

	// g_object_unref( provider );

	/* We do not need the builder any more */
	g_object_unref( builder );
}

int main( int argc, char* argv[] )
{
#ifdef G_OS_WIN32
	AddFontResourceEx( TEXT( "res/font/iconfont.ttf" ), FR_PRIVATE, 0 );
#endif

	g_resources_register( res_get_resource() );

	GtkApplication* app = gtk_application_new( "fun.bilo.demo.example", G_APPLICATION_FLAGS_NONE );
	g_signal_connect( app, "activate", G_CALLBACK( activate ), NULL );

	int status = g_application_run( G_APPLICATION( app ), argc, argv );
	g_object_unref( app );

	return status;
}