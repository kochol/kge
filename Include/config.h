// Filename:	confing.h
// Des:			this file contains kge compile configurations
//				you can enable or disable any feature listed below
// Date:		6/5/1389 (jalali)
// Programmer:	Pooya Shahinfar (Pswin)

#ifndef CONFIG_H
#define CONFIG_H

//***************************
// Renderer confings
//***************************

//==================
// OpenGL
//==================
// OpenGL , if you don't want to use OpenGL 2.0+ for rendering then comment next line
#define KGE_USE_OPENGL_2X 1
#define KGE_USE_OPENGL_3X 0

#if (KGE_USE_OPENGL_2X + KGE_USE_OPENGL_3X) >= 1
	//#define KGE_USE_OPENGL 1
#endif

//==================
// Microsoft DirectX
//==================
// MS DirectX 9 , if you don't want to use Microsoft DirectX 9.x for rendering then comment next lien
#define KGE_USE_DIRECTX_9 1



//******************************
// Sound System configurations
//******************************

// OpenAL 
#define KGE_USE_OPENAL 0

#if KGE_USE_OPENAL == 1
	// if you don't want to use vorbis OGG format then comment following line
	#define KGE_USE_OGG 1 
#endif 

// FMOD
#define KGE_USE_FMOD 0

// BASS
#define KGE_USE_BASS 1

#if (KGE_USE_FMOD == 1) || (KGE_USE_BASS == 1) || (KGE_USE_OPENAL == 1)
	#define KGE_USE_SOUND_SYSTEM 1
#endif

//*************************************
// third party libraries configuration
//*************************************

// If you don't want to use SDL library then comment next line.
#define KGE_USE_SDL 1

// if you don't want to use NVIDIA PhysX then comment next line
//#define KGE_USE_PHYSX 1

//**************************************
// Use boost
//**************************************
//#define KGE_USE_BOOST 1

//************************************************************************
//   M E M O R Y    C O N F I G
//************************************************************************
#define KGE_MEMORY_NEDMALLOC 1

// use nedmalloc for default allocator
#define KGE_MEMORY_ALLOCATOR KGE_MEMORY_NEDMALLOC

#define KGE_TRACK_MEMORY

//*******************************
// char set configuration
//*******************************

// if you don't want kge use unicode then comment following line
//#define KGE_USE_UNICODE 1

//******************************
// hardware extensions
//******************************

/* note: - Based on the 2009 Steam Hardware Survey the 98.52% of computers having 
		   Steam support SSE2,while SSE3 and SSE4 are supported by 95.54% of computers. but
		   3DNow! are not widely supported
		 - you can get new infos form Steam Hardware survay : http://store.steampowered.com/hwsurvey 

		 - you can enable only one CPU extension at a time
*/

// SSE/SSE2 (needs Pentium 3 or AMD64 or later processors)
//#define KGE_USE_SSE2 1

// SSE3/SSE4 (needs Pentium 4 and later)
//define KGE_USE_SSE4 1

// 3DNow! (works only on AMD processors)
//define KGE_USE_3DNOW 1

#if (KGE_USE_SSE2 + KGE_USE_3DNOW + KGE_USE_SSE4) > 1 
	#error "Two or more CPU extension enabled."
#endif


#endif // CONFIG_H