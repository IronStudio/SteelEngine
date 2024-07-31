#pragma once


#ifdef SE_STATIC
	#define SE_CORE
#elifdef SE_SHARED
	#ifndef SE_WINDOWS
		#define SE_CORE
	#else
		#ifdef SE_BUILD_ENGINE
			#define SE_CORE __declspec(dllexport)
		#else
			#define SE_CORE __declspec(dllimport)
		#endif
	#endif
#else
	#error The build type (shared or static) has not been specified
#endif
