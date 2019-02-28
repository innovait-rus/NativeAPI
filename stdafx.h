// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#ifdef _WINDOWS
    #include <windows.h>
	#include <process.h>
#endif //_WINDOWS

#if defined(__linux__) || defined(__APPLE__)
	#define LINUX_OR_MACOS
#endif