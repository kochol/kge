#ifndef KGE_MOUSEOIS_H
#define KGE_MOUSEOIS_H

#include "../include/Mouse.h"

namespace OIS
{
	class Mouse;

} // OIS

namespace kge
{
	namespace io
	{
		// OIS Mouse
		class MouseOIS : public Mouse
		{
		public :

			void CopyState();

			OIS::Mouse        *mMouse;

		}; // MouseOIS

	} // io

} // kge

#endif // KGE_MOUSEOIS_H
