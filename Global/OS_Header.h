#ifndef OS_Header_h
#define OS_Header_h
//------------------------------------------------------------------------------

#if defined(__WIN32)
 #define UNICODE

 #include <windows.h>
//------------------------------------------------------------------------------

#elif defined(__linux__)
 #error Linux not yet supported at this point.
//------------------------------------------------------------------------------

#elif defined(__APPLE__)
 #error MAC OS not yet supported at this point.
//------------------------------------------------------------------------------

#else
 #error Your operating system is not supported at this point.
#endif
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
