#include "platform.h"

static GString* real_path;
static GString* greal_path;

// 拼接资源路径
// linux 上是 /usr/share/...
// win   上是 ./res/...
#ifndef RESDIR
#define RESDIR "res/"
#endif

const char* RESPATH( const char* res_path )
{
	if ( !res_path || res_path[ 0 ] == '\0' )
		g_error( "res_path is NULL!!!" );

	if ( real_path == NULL )
		real_path = g_string_new( RESDIR );

	if ( res_path[ 0 ] == '/' )
		res_path += 1;

	g_string_printf( real_path, "%s%s", RESDIR, res_path );

	return real_path->str;
}

const char* GRESPATH( const char* res_path )
{
	// TODO 注册资源

	return res_path;
}

#ifdef _WIN32
const TCHAR* TRESPATH( const char* res_path )
{
	RESPATH( res_path );

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
