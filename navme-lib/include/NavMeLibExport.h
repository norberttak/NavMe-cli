#pragma once
#ifdef MAKE_DLL_X
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT
#endif
