#ifndef KGE_KEYBOARDOIS_H
#define KGE_KEYBOARDOIS_H

#include "../include/Keyboard.h"

namespace OIS
{
	class Keyboard;

} // OIS

namespace kge
{
	namespace io
	{
		class KeyboardOIS: public Keyboard
		{
		public:

			//! IS key down?
			bool isKeyDown(KeyCode key) const;

			// Check modifier status
			bool isModifierDown(Modifier mod) const;

			OIS::Keyboard*	m_pKeyboard;

		}; // KeyboardOIS

	} // io

} // kge

#endif // KGE_KEYBOARDOIS_H
