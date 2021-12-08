#include "stdafx.h"

#include "utils.h"
#include "resource_monitor.h"
#include "widgets/button_1.h"
#include "res.h"

static void windows_motion_notify( GtkEventControllerMotion* self, gdouble x, gdouble y, GtkEventController* user_data )
{
	GtkWindow* win = GTK_WINDOW( gtk_event_controller_get_widget( GTK_EVENT_CONTROLLER( self ) ) );

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
	gtk_css_provider_load_from_path( *provider, DATAPATH( "res/css/main.css" ) );
	gtk_style_context_add_provider_for_display(
	  gdk_display_get_default(), GTK_STYLE_PROVIDER( *provider ), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION );
}

static void reload_ui( GtkApplication* app )
{
	GtkBuilder* builder = gtk_builder_new();

	GtkBuilderScope* builder_callback_symbol = gtk_builder_cscope_new();
	gtk_builder_cscope_add_callback_symbol(
	  GTK_BUILDER_CSCOPE( builder_callback_symbol ), "quit_program", G_CALLBACK( gtk_window_close ) );
	gtk_builder_cscope_add_callback_symbol(
	  GTK_BUILDER_CSCOPE( builder_callback_symbol ), "reload_ui", G_CALLBACK( reload_ui ) );
	gtk_builder_set_scope( builder, builder_callback_symbol );

	gtk_builder_expose_object( builder, "app", G_OBJECT( app ) );
	gtk_builder_expose_object( builder, "Button1", G_OBJECT( button_1_new() ) );

	GError* load_error = NULL;
	if ( !gtk_builder_add_from_file( builder, DATAPATH( "res/ui/main.ui" ), &load_error ) ) {
		g_message( _( "%s" ), load_error->message );
		g_error_free( load_error );
		goto err;
	}

	GtkWindow* old_window = g_object_get_data( G_OBJECT( app ), "old_window" );
	GtkWindow* window     = GTK_WINDOW( gtk_builder_get_object( builder, "window" ) );

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

	gtk_window_set_application( GTK_WINDOW( window ), app );

	if ( old_window ) {
		g_object_unref( old_window );
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

	GtkCssProvider* provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path( provider, DATAPATH( "res/css/main.css" ) );

	gtk_style_context_add_provider_for_display(
	  gdk_display_get_default(), GTK_STYLE_PROVIDER( provider ), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION );

	GtkCssProvider** provider_p = g_new( GtkCssProvider*, 1 );
	*provider_p                 = provider;

	reload_ui( app );

	lx_resource_monitor( DATAPATH( "res/css/main.css" ), G_CALLBACK( reload_css ), provider_p );
	lx_resource_monitor( DATAPATH( "res/ui/main.ui" ), G_CALLBACK( reload_ui ), app );
}

int main( int argc, char* argv[] )
{
#ifdef G_OS_WIN32
	g_assert_cmpint( AddFontResourceEx( TDATAPATH( "res/font/iconfont.ttf" ), FR_PRIVATE, 0 ), >, 0 );
#endif
	g_assert( FcConfigAppFontAddDir( NULL, DATAPATH( "res/font/" ) ) == FcTrue );

	g_resources_register( res_get_resource() );

	GtkApplication* app = gtk_application_new( "fun.bilo.demo.example", G_APPLICATION_FLAGS_NONE );
	g_signal_connect( app, "activate", G_CALLBACK( activate ), NULL );

	int status = g_application_run( G_APPLICATION( app ), argc, argv );
	g_object_unref( app );

	return status;
}
