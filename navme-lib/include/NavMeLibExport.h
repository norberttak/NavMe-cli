#pragma once
#ifdef MAKE_DLL
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT 
#endif
