#include <OISKeyboard.h>

#include "KeyboardOIS.h"

namespace kge
{
	namespace io
	{
		//! IS key down?
		bool KeyboardOIS::isKeyDown(KeyCode key) const
		{
			return m_pKeyboard->isKeyDown((OIS::KeyCode)key);

		} // isKeyDown

		// Check modifier status
		bool KeyboardOIS::isModifierDown(Modifier mod) const
		{
			return m_pKeyboard->isModifierDown((OIS::Keyboard::Modifier)mod);

		} // isModifierDown

	} // io

} // kge
