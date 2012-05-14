#ifndef KGE_WEBAPPLICATION_H
#define KGE_WEBAPPLICATION_H

#include "kgedef.h"

namespace kge
{
	namespace web
	{
		class KGE_API WebApplication
		{
		public:

			//! Constructor
			WebApplication() {}

			//! Destructor
			virtual ~WebApplication() {}

			//! The WebPlayer call this function when you have to initialise your application.
			virtual void Initialise() = 0;

			//! The WebPlayer call this function when you have to update your game step.
			/*! Return false to stop the WebPlayer
			 */
			virtual bool Update() = 0;

			//! The WebPlayer call this function when you have to shutdown your application.
			virtual void ShutDown() = 0;

			//! The WebPlayer call this function give you the window handle and size for render.
			/*!
				\param hwnd The Window handle
				\param width The window width
				\param height The window height
			 */
			virtual void SetCanvas(int hwnd, int width, int height) = 0;

			//! Gives the resource path for loading default resources
			virtual void SetResourcePath(const char* path) = 0;

		}; // WebApplication

	} // web

} // kge

#endif // KGE_WEBAPPLICATION_H
