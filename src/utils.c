#include "utils.h"

static GString* real_path;
static GString* greal_path;

#ifndef APPID
#error Lost APPID
#endif

const char* DATAPATH( const char* res_path )
{
	if ( !res_path || res_path[ 0 ] == '\0' )
		g_error( "res_path is NULL!!!" );

	if ( real_path == NULL )
		real_path = g_string_new( "" );

	if ( res_path[ 0 ] == '/' )
		res_path += 1;

	static const char* install_dir = NULL;

	if ( !install_dir )
		install_dir = g_getenv( APPID ".DATAPATH" );

#ifdef G_OS_WIN32
	if ( !install_dir )
		install_dir = g_win32_get_package_installation_directory_of_module( NULL );
#else
	if ( !install_dir )
		install_dir = DATA_PREFIX;
#endif

	char last_ch = *( install_dir + strlen( install_dir ) - 1 );
	if ( last_ch == '/' || last_ch == '\\' ) {
		g_string_printf( real_path, "%s%s/%s/%s", install_dir, "share", APPID, res_path );
	}
	else {
		g_string_printf( real_path, "%s/%s/%s/%s", install_dir, "share", APPID, res_path );
	}

	return real_path->str;
}

const char* GDATAPATH( const char* res_path )
{
	// TODO 注册资源

	return res_path;
}

#ifdef _WIN32
const TCHAR* TDATAPATH( const char* res_path )
{
	DATAPATH( res_path );

#ifdef _UNICODE
	if ( real_path->allocated_len <= real_path->len * 2 + 2 )
		g_string_set_size( real_path, real_path->len * 2 + 2 );

	glong      u16_len = 0;
	gunichar2* u16     = g_utf8_to_utf16( real_path->str, -1, NULL, &u16_len, NULL );

	memmove( real_path->str, u16, u16_len * 2 + 2 );
	real_path->len = u16_len * 2;

	g_free( u16 );
#endif

	return ( const TCHAR* )real_path->str;
}
#endif
