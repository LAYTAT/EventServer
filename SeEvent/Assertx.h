#pragma once

#include "SePlatForm.h"
#include <cassert>

void __assert__(const char* file, uint32_t line, const char* func, const char* expr);
void __assertex__(const char* file, uint32_t line, const char* func, const char* expr, const char* msg);

#if defined(NDEBUG)
#define Assert(expr) ((void)0)
#define AssertEx(expr, msg) ((void)0)
#elif defined SF_PLATFORM_WIN
#include <crtdbg.h>

#define Assert(expr) {if(!(expr)){ __assert__(__FILE__, __LINE__, __FUNCTION__, #expr);}}
#define AssertEx(expr, msg) {if(!(expr)){ __assertex__(__FILE__, __LINE__, __FUNCTION__, #expr, msg);}}
#elif defined SF_PLATFORM_LINUX
#define Assert(expr) {if(!(expr)){ __assert__(__FILE__, __LINE__, __FUNCTION__, #expr);}}
#define AssertEx(expr, msg) {if(!(expr)){ __assertex__(__FILE__, __LINE__, __FUNCTION__, #expr, msg);}}
#endif