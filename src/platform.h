#pragma once

#include "stdafx.h"

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#endif

const char* RESPATH( const char* res_path );
/**
 * @brief 直接读取文件返回资源，不用每次都打包。
 *
 * 避免每次修改资源就打包一次
 *
 * @param res_path
 * @return const char* 拼接 ID 的路径
 */
const char* GRESPATH( const char* res_path );
#ifdef _WIN32
const TCHAR* TRESPATH( const char* res_path );
#endif
