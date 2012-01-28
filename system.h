#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef WIN32
#include <windows.h>
#define PATH_SEP "\\"
#else
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PATH_SEP "/"

// windef.h
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef unsigned short WORD;

typedef struct {
	long x;
	long y;
} POINT;

typedef struct {
	int x;
	int y;
} MOUSEPOINT;

typedef struct {
	int left;
	int right;
	int top;
	int bottom;
} RECT;

#define MAX_PATH 260
#define DATA_PATH "data/resources/"

// APIs replacement
#define GetCurrentDirectory(x,y) getcwd(y,x)
#define ZeroMemory(x,y) memset(x,0,y)

// Needed by DeltaTime.cpp
#define timeGetTime() time(NULL)

// Needed by Logger.cpp
#define SYSTEMTIME struct tm
#define GetSystemTime(x) *x = localtime(time(NULL))
#endif

static inline int powerOfTwo( int value )
{
	int result = 1 ;

	while ( result < value )
		result *= 2 ;

	return result ;
}

#define debug(...) fprintf(stderr, __FILE__ ", l." XSTRING(__LINE__) ": " __VA_ARGS__)
#define XSTRING(x) STRING(x)
#define STRING(x)  #x

#endif // SYSTEM_H
