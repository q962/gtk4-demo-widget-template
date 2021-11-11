#include "stdafx.h"

#include "platform.h"
#include "resource_monitor.h"
#include "widgets/button_1.h"
#include "res.h"

static void windows_motion_notify( GtkEventControllerMotion* self, gdouble x, gdouble y, GtkEventController* user_data )
{
	GtkWindow* win = GTK_WINDOW( gtk_event_controller_get_widget( GTK_EVENT_CONTROLLER( self ) ) );
	g_message( "%s: %lf,%lf", gtk_event_controller_get_name( GTK_EVENT_CONTROLLER( self ) ), x, y );

	gint w_w, w_h;

	gtk_window_get_default_size( win, &w_w, &w_h );

	if ( x > w_w + 30 || y > w_h + 30 ) {
		// G_BREAKPOINT();
		g_message( "other window" );
	}
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
		goto err;
	}

	GtkWindow* old_window = g_object_get_data( G_OBJECT( app ), "old_window" );
	if ( old_window ) {
		g_object_unref( old_window );
	}

	GtkWindow* window = GTK_WINDOW( gtk_builder_get_object( builder, "window" ) );

	g_object_set_data( G_OBJECT( app ), "old_window", window );

	{
		GtkEventController* motion_m = gtk_event_controller_motion_new();
		g_signal_connect( motion_m, "motion", G_CALLBACK( windows_motion_notify ), motion_m );

		GString* ec_name = g_string_new( "metion" );
		g_string_append_printf( ec_name, ": %d", g_get_monotonic_time() );

		gtk_event_controller_set_name( motion_m, ec_name->str );

		gtk_widget_add_controller( GTK_WIDGET( window ), motion_m );

		g_string_free( ec_name, TRUE );
	}

	gtk_widget_show( GTK_WIDGET( window ) );

err:
	g_object_unref( builder );
}

static void activate( GtkApplication* app, gpointer user_data )
{
	g_object_set(
	  gtk_settings_get_for_display( gdk_display_get_default() ), "gtk-font-name", "Microsoft Yahei Normal 12", NULL );

	char* cur_font_desc = NULL;
	g_object_get( gtk_settings_get_for_display( gdk_display_get_default() ), "gtk-font-name", &cur_font_desc );

	if ( g_strcmp0( cur_font_desc, "Microsoft Yahei Normal 12" ) == 0 ) {
		g_object_set(
		  gtk_settings_get_for_display( gdk_display_get_default() ), "gtk-xft-dpi", ( int )( 0.96 * 96 * 1024 ), NULL );
	}

	g_free( cur_font_desc );

	/* Construct a GtkBuilder instance and load our UI description */
	GtkBuilder* builder = gtk_builder_new();

	GtkBuilderScope* builder_callback_symbol = gtk_builder_cscope_new();
	gtk_builder_cscope_add_callback_symbol(
	  GTK_BUILDER_CSCOPE( builder_callback_symbol ), "quit_program", G_CALLBACK( gtk_window_close ) );
	gtk_builder_cscope_add_callback_symbol(
	  GTK_BUILDER_CSCOPE( builder_callback_symbol ), "reload_ui", G_CALLBACK( reload_ui ) );

	gtk_builder_expose_object( builder, "app", G_OBJECT( app ) );
	gtk_builder_set_scope( builder, builder_callback_symbol );

	GError* load_error = NULL;
	if ( !gtk_builder_add_from_file( builder, "res/ui/quit_win.ui", &load_error ) ) {
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

	reload_ui( app );

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
