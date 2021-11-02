#include "resource_monitor.h"

struct UseData {
	void ( *callback )( gpointer );
	gpointer data;
	guint    timer;
};

static gboolean call( gpointer user_data )
{
	g_message( _( "call callback" ) );

	struct UseData* d = user_data;
	d->callback( d->data );
	d->timer = 0;

	return G_SOURCE_REMOVE;
}

static void changed( GFileMonitor*     self,
                     GFile*            file,
                     GFile*            other_file,
                     GFileMonitorEvent event_type,
                     gpointer          user_data )
{
	char* file_path = g_file_get_path( file );
	g_message( _( "file change: %s" ), file_path );
	g_free( file_path );

	if ( G_FILE_MONITOR_EVENT_CHANGED == event_type ) {
		struct UseData* d = user_data;

		g_clear_handle_id( &d->timer, g_source_remove );
		d->timer = g_timeout_add( 3000, call, user_data );
	}
}

void lx_resource_monitor( const char* file_path, GCallback callback, gpointer data )
{
	GFileMonitor* m = g_file_monitor_file( g_file_new_for_path( file_path ), G_FILE_MONITOR_SEND_MOVED, NULL, NULL );

	struct UseData* use_data = g_new( struct UseData, 1 );
	use_data->callback       = ( void ( * )( gpointer ) )callback;
	use_data->data           = data;
	use_data->timer          = 0;

	g_signal_connect( G_OBJECT( m ), "changed", G_CALLBACK( changed ), use_data );
}