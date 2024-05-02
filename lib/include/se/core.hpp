#pragma once



#if defined(SE_STATIC)
	#define SE_CORE
#elif defined(SE_SHARED)
	#ifdef SE_WINDOWS
		#ifdef SE_BUILD_LIB
			#define SE_CORE __declspec(dllexport)
		#else
			#define SE_CORE __declspec(dllimport)
		#endif
	#elifdef SE_LINUX
		#ifdef SE_BUILD_LIB
			#define SE_CORE __attribute__((visibility("default")))
		#else
			#define SE_CORE
		#endif
	#else
		#define SE_CORE
	#endif
#else
	#error You must define either 'SE_STATIC' or 'SE_SHARED' !
#endif