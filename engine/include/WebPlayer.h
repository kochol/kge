#ifndef KGE_WEBPLAYER_H
#define KGE_WEBPLAYER_H

#include "kgedef.h"

namespace kge
{
	//! The classes for writing web applications are here :)
	namespace web
	{
		class WebApplication;

		class KGE_API WebPlayer
		{
		public:

			//! Constructor
			/*! The WebPlayer sends events to the user webapplication pointer
				\param pWebApp The user webapplication pointer
				\sa WebApplication
			 */
			WebPlayer(WebApplication* pWebApp);

			//! Destructor
			~WebPlayer();

			//! For internal use
			void Initialise();

			//! For internal use
			bool Update();

			//! For internal use
			void ShutDown();

			//! For internal use
			void SetCanvas(int hwnd, int width, int height);

			//! For internal use
			void SetResourcePath(const char* path);

		protected:

			WebApplication			*	m_pWebApp;	//!< Web application class pointer

		}; // WebPlayer

	} // web

} // kge

#endif // KGE_WEBPLAYER_H
