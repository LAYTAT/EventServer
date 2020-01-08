#include "Assertx.h"

void __assert__(const char* file, UINT line, const char* func, const char* expr)
{
	char out_msg[1024] = { 0 };
	sprintf_s(out_msg, 1023, "[%s][%d][%s][%s]", file, line, func, expr);
#if defined SF_PLATFORM_WIN
	MessageBoxA(0, out_msg, expr, MB_RETRYCANCEL | MB_ICONERROR);
	assert(0);
#else
	fprintf(stderr, "%s\n", out_msg);
	abort();
#endif
}

void __assertex__(const char* file, UINT line, const char* func, const char* expr, const char* msg)
{
	char out_msg[1024] = { 0 };
	sprintf_s(out_msg, 1023, "[%s][%d][%s][%s]\n[%s]", file, line, func, expr, msg);
#if defined SF_PLATFORM_WIN
	MessageBoxA(0, out_msg, expr, MB_RETRYCANCEL | MB_ICONERROR);
	assert(0);
#else
	fprintf(stderr, "%s\n", out_msg);
	abort();
#endif
	
}