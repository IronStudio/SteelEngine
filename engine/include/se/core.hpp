#pragma once


#ifdef WIN32
	#define SE_PLATFORM_WINDOWS
#endif


#ifdef SE_BUILD_ENGINE
	#ifdef SE_BUILD_SHARED
		#ifdef SE_PLATFORM_WINDOWS
			#define SE_CORE __declspec(dllexport)
		#else
			#define SE_CORE
		#endif
	#else
		#define SE_CORE
	#endif
#else
	#ifdef SE_BUILD_SHARED
		#ifdef SE_PLATFORM_WINDOWS
			#define SE_CORE __declspec(dllimport)
		#else
			#define SE_CORE
		#endif
	#else
		#define SE_CORE
	#endif
#endif
