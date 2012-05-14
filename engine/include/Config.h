#ifndef KGE_CONFIG_H
#define KGE_CONFIG_H

//************************************************************************
//   M E M O R Y    C O N F I G
//************************************************************************
#define KGE_MEMORY_NEDMALLOC 1

// use nedmalloc for default allocator
#define KGE_MEMORY_ALLOCATOR KGE_MEMORY_NEDMALLOC

#define KGE_TRACK_MEMORY


//************************************************************************
//   S I M D    C O N F I G
//************************************************************************

#define KGE_USE_SSE2 0	// SSE 2
#define KGE_USE_SSE3 0	// SSE 3
#define KGE_USE_SSE4 0	// SSE 4
#define KGE_USE_3DNOW 0	// 3DNow!

#if ( KGE_USE_SSE2 + KGE_USE_SSE3 + KGE_USE_SSE4 + KGE_USE_3DNOW ) == 1
	#define KGE_USE_SIMD 1
#elif ( KGE_USE_SSE2 + KGE_USE_SSE3 + KGE_USE_SSE4 + KGE_USE_3DNOW ) > 1
	#error "more than one SIMD config enabled."
#else
	#define KGE_USE_SIMD 0
#endif

//************************************************************************
//   C O N T I N E R S    C O N F I G
//************************************************************************

// arrays
#define KGE_DEFAULT_FIXED_ARRAY kge::core::FixedArray
#define KGE_DEFAULT_DYNAMIC_ARRAY kge::core::DynamicArray
#define KGE_DEFAULT_ARRAY_BLOCK_SIZE 100

// map
#define KGE_DEFAULT_MAP std::map

#endif // KGE_CONFIG_H
