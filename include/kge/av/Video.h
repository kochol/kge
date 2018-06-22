// File name: Video.h
// Des: This file contain video class interface
// Date: 01/09/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_VIDEO_H
#define KGE_VIDEO_H

#include "../Plugin.h"
#include <dshow.h>
#include <mmstream.h>
#include <amstream.h> 
#include <ddstream.h>

namespace kge
{
namespace av
{
class KGE_API Video : public Resource
{
public:

	//! Constructor
	Video(Device* pDev, const u32 Handle, char* FileName, char* Name);

	//! Destructor
	~Video();

	//! Loads the movie from file.
	bool LoadMovie(char* FileName);

	/*! Call this function to prepare next frame
		\return Returns false if the next frame time has not come yet.
	 */
    bool NextMovieFrame();

	int GetMovieWidth() { return (m_Movie_rect.right - m_Movie_rect.left);}

	int GetMovieHeight() { return (m_Movie_rect.bottom - m_Movie_rect.top);}  

	//! Render the movie to texture
	void DrawMovie();

	DDSURFACEDESC  ddsd; 

	void Lock();

	void Unlock();

protected:
     IAMMultiMediaStream*     m_pAMStream;
     IMediaStream*            m_pPrimaryVidStream;
     IDirectDrawMediaStream*  m_pDDStream;
     IDirectDrawStreamSample* m_pSample;
     IDirectDrawSurface*      m_pSurface;
     RECT                     m_Movie_rect;
     LONG                     m_MoviePitch;
     void*                    m_MovieBuffer;
     DWORD                    m_Time;
     DWORD                    m_Oldtick;
     BOOL                     m_Flag; 

}; // Video

} // gui

} // kge

#endif // KGE_VIDEO_H
