// File name: Video.cpp
// Des: This file contain video plugin setup and video class
// Date: 01/09/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/av/Video.h"

static GUID MY_CLSID_AMMultiMediaStream={0x49C47CE5,0x9BA4,0x11D0,0x82,0x12,0x00,0xC0,0x4F,0xC3,0x2C,0x45};
static GUID MY_IID_IAMMultiMediaStream={0xBEBE595C,0x9A6F,0x11D0,0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D};
static GUID MY_MSPID_PrimaryVideo={0xA35FF56A,0x9FDA,0x11D0,0x8F,0xDF,0x00,0xC0,0x4F,0xD9,0x18,0x9D};
static GUID MY_IID_IDirectDrawMediaStream={0xF4104FCE,0x9A70,0x11D0,0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D};  
static GUID MY_MSPID_PrimaryAudio={0xA35FF56B,0x9FDA,0x11D0,0x8F,0xDF,0x00,0xC0,0x4F,0xD9,0x18,0x9D};

namespace kge
{
namespace av
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Video::Video(Device* pDev, const u32 Handle, char* FileName, char* Name):
		Resource(Handle, FileName, Name, NULL)
	{
		m_pAMStream         = 0;
		m_pPrimaryVidStream = 0;  
		m_pDDStream         = 0;    
		m_pSample           = 0;
		m_pSurface          = 0;
		m_Time              = 40; // Set 25 fps
		m_Oldtick			= 0;
		io::Logger::Log("****** Video Constructor.");

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Video::~Video()
	{
		m_pPrimaryVidStream->Release();
		m_pDDStream->Release();
		m_pSample->Release();
		m_pSurface->Release();
		m_pAMStream->Release();

	} // Destructor

	//------------------------------------------------------------------------------------
	// Loads the movie from file.
	//------------------------------------------------------------------------------------
	bool Video::LoadMovie(char* FileName)
	{
        WCHAR buf[512];
        MultiByteToWideChar(CP_ACP, 0, FileName, -1, buf, 512); 
        CoCreateInstance(MY_CLSID_AMMultiMediaStream,0,1,MY_IID_IAMMultiMediaStream,(void**)&m_pAMStream);
        m_pAMStream->Initialize((STREAM_TYPE) 0, 0, NULL);
        m_pAMStream->AddMediaStream( 0, &MY_MSPID_PrimaryVideo, 0, NULL); 
        m_pAMStream->OpenFile(buf,4); //AMMSF_RENDERTOEXISTING); // 
        m_pAMStream->GetMediaStream( MY_MSPID_PrimaryVideo, &m_pPrimaryVidStream);
        m_pPrimaryVidStream->QueryInterface(MY_IID_IDirectDrawMediaStream,(void**)&m_pDDStream);
        m_pDDStream->CreateSample(0,0,0,&m_pSample);
        m_pSample->GetSurface(&m_pSurface,&m_Movie_rect);
        m_pAMStream->SetState(STREAMSTATE_RUN);

		return true;

	} // LoadMovie

	//------------------------------------------------------------------------------------
	// Call this function to prepare next frame
	//------------------------------------------------------------------------------------
	bool Video::NextMovieFrame()
	{
		if(GetTickCount() - m_Oldtick < m_Time)
		{
			m_Flag = false ;
			return false;
		}
		m_Oldtick = GetTickCount(); 
		m_Flag    = true;
		m_pSample->Update( 0, NULL, NULL, 0);
		
		return true;

	} // NextMovieFrame

	//------------------------------------------------------------------------------------
    // Render the movie to texture
	//------------------------------------------------------------------------------------
	void Video::DrawMovie()
	{   
		//DDSURFACEDESC  ddsd; 
		//ddsd.dwSize=sizeof(DDSURFACEDESC);
		//m_pSurface->Lock( NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT , NULL); 
		////glTexSubImage2D();
		//m_pSurface->Unlock(NULL);
	}  

	void Video::Lock()
	{
		ddsd.dwSize=sizeof(DDSURFACEDESC);
		m_pSurface->Lock( NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT , NULL); 
	}

	void Video::Unlock()
	{
		m_pSurface->Unlock(NULL);
	}

} // av

} // kge
