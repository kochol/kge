#ifndef KGE_PDEF_H
#define KGE_PDEF_H

//=====================================
// DLL export / import
//=====================================
#ifdef PROFILER_EXPORTS
#define PKGE_API __declspec( dllexport )
#else
#define PKGE_API __declspec( dllimport )
#endif

#endif // KGE_PDEF_H