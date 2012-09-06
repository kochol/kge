#include "../include/WebPlayer.h"
#include "../include/WebApplication.h"
#include "../include/PluginManager.h"
#include "../include/Logger.h"
#include "InputMgrJava.h"

#ifndef NULL
#define NULL 0
#endif

kge::web::WebPlayer				*	g_pWebPlayer = NULL;
kge::io::InputMgerJavaPlugin	*	g_pInputMgrPlugin;

namespace kge
{
	namespace web
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		WebPlayer::WebPlayer(WebApplication* pWebApp): m_pWebApp(pWebApp)			
		{
			g_pWebPlayer		= this;
			g_pInputMgrPlugin = KGE_NEW(io::InputMgerJavaPlugin)();
			printf("Java input manager plugin registered automatically for using in web application. version: %d.%d , Index: %d", 
				g_pInputMgrPlugin->GetEngineMajorVersion(), 
				g_pInputMgrPlugin->GetEngineMinorVersion(),
				PluginManager::GetSingletonPtr()->RegisterInputManager(g_pInputMgrPlugin));			

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		WebPlayer::~WebPlayer()
		{
			g_pWebPlayer = NULL;

		} // Destructor

		//------------------------------------------------------------------------------------
		// Initialise
		//------------------------------------------------------------------------------------
		void WebPlayer::Initialise()
		{
			m_pWebApp->Initialise();

		} // Initialise

		//------------------------------------------------------------------------------------
		// Update
		//------------------------------------------------------------------------------------
		bool WebPlayer::Update()
		{
			return m_pWebApp->Update();

		} // Update

		//------------------------------------------------------------------------------------
		// ShutDown
		//------------------------------------------------------------------------------------
		void WebPlayer::ShutDown()
		{
			m_pWebApp->ShutDown();

		} // ShutDown

		//------------------------------------------------------------------------------------
		// SetCanvas
		//------------------------------------------------------------------------------------
		void WebPlayer::SetCanvas(int hwnd, int width, int height)
		{
			m_pWebApp->SetCanvas(hwnd, width, height);

		} // SetCanvas

		//------------------------------------------------------------------------------------
		// SetResourcePath
		//------------------------------------------------------------------------------------
		void WebPlayer::SetResourcePath(const char* path)
		{
			m_pWebApp->SetResourcePath(path);

		} // SetResourcePath

	} // web

} // kge
