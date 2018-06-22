#include "../../include/kge/gui/libRocketKGESystem.h"
#include "../../include/kge/io/Logger.h"
#include "../../include/kge/core/Timer.h"
#include "../../include/kge/gfx/Renderer.h"
#include "../../include/kge/io/Mouse.h"
#include "../../include/kge/io/Keyboard.h"
#include "../../include/kge/core/ptr_fun.h"

KGE_IMPORT extern kge::gfx::Renderer*	g_pRenderer;
Rocket::Core::Context				*	g_pCurrentContex = NULL;

namespace kge
{
	Rocket::Core::Input::KeyIdentifier keymap[io::EK_LAST];
	void BuildKeyMap();
	Rocket::Core::word GetCharacterCode(Rocket::Core::Input::KeyIdentifier key_identifier, int key_modifier_state);
	int GetKeyModifier();

	// Mouse event handler
	void MouseMotion(gui::EventArgs* pe)
	{
		if (g_pCurrentContex)
		{
			int km = GetKeyModifier();
			gui::MouseEventArgs* p = (gui::MouseEventArgs*)pe;
			if (p->m_EventType == gui::EET_MouseMotion)
				g_pCurrentContex->ProcessMouseMove(p->X, p->Y, km);
			if (p->m_EventType == gui::EET_MouseDown)
				g_pCurrentContex->ProcessMouseButtonDown(p->Mousebutton, km);
			if (p->m_EventType == gui::EET_MouseUp)
				g_pCurrentContex->ProcessMouseButtonUp(p->Mousebutton, km);
		}

	} // MouseMotion

	// Keyboard event handler
	void KeyEvents(gui::EventArgs* pe)
	{
		if (g_pCurrentContex)
		{
			int km = GetKeyModifier();
			gui::KeyEventArgs* p = (gui::KeyEventArgs*)pe;
			Rocket::Core::Input::KeyIdentifier key = keymap[p->currentKeyDown];
			if (p->m_EventType == gui::EET_KeyDown)
			{
				g_pCurrentContex->ProcessKeyDown(key, km);
				Rocket::Core::word character = GetCharacterCode(key, km);
				if (character > 0)
					g_pCurrentContex->ProcessTextInput(character);
			}
			if (p->m_EventType == gui::EET_KeyUp)
				g_pCurrentContex->ProcessKeyUp(key, km);
		}
		
	} // KeyEvents

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	libRocketKGESystem::libRocketKGESystem(): m_pContext(NULL)
	{
		io::Mouse m;
		m.Connect(gui::EET_MouseMotion, core::ptr_fun(MouseMotion));
		m.Connect(gui::EET_MouseUp, core::ptr_fun(MouseMotion));
		m.Connect(gui::EET_MouseDown, core::ptr_fun(MouseMotion));
		io::Keyboard k;
		k.Connect(gui::EET_KeyDown, core::ptr_fun(KeyEvents));
		k.Connect(gui::EET_KeyUp, core::ptr_fun(KeyEvents));
		BuildKeyMap();

	} // Constructor

	//------------------------------------------------------------------------------------
	// Get the number of seconds elapsed since the start of the application.
	//------------------------------------------------------------------------------------
	float libRocketKGESystem::GetElapsedTime()
	{
		static core::Timer t;

		return t.GetTimeElapsed();

	} // GetElapsedTime

	//------------------------------------------------------------------------------------
	// Log the specified message.
	//------------------------------------------------------------------------------------
	bool libRocketKGESystem::LogMessage( Rocket::Core::Log::Type type, const Rocket::Core::String& message )
	{
		switch (type)
		{
		case Rocket::Core::Log::Type::LT_ASSERT:
		case Rocket::Core::Log::Type::LT_ERROR:
			io::Logger::Error("LibRocket: %s", message.CString());
			break;

		case Rocket::Core::Log::Type::LT_DEBUG:
			io::Logger::Debug("LibRocket: %s", message.CString());
			break;

		case Rocket::Core::Log::Type::LT_WARNING:
			io::Logger::Warning("LibRocket: %s", message.CString());
			break;

		case Rocket::Core::Log::Type::LT_INFO:
			io::Logger::Info("LibRocket: %s", message.CString());
			break;
		}

		return true;

	} // LogMessage

	//------------------------------------------------------------------------------------
	// Sets the rocket context for sending inputs to it.
	//------------------------------------------------------------------------------------
	void libRocketKGESystem::SetContext( Rocket::Core::Context* pContext )
	{
		m_pContext		 = pContext;
		g_pCurrentContex = pContext;

	} // SetContext

	//------------------------------------------------------------------------------------
	// Updates and render the context
	//------------------------------------------------------------------------------------
	void libRocketKGESystem::Render()
	{
 		g_pRenderer->Enable(gfx::ERF_2D);
 		g_pRenderer->Disable(gfx::ERF_DepthBuffer);
 		g_pRenderer->SetTextureParams(gfx::ETP_Clamp);
		m_pContext->Update();
		m_pContext->Render();
 		g_pRenderer->Enable(gfx::ERF_DepthBuffer);
 		g_pRenderer->Disable(gfx::ERF_2D);

	} // Render

	using namespace io;
	using namespace Rocket;
	using namespace Core;
	using namespace Input;

	void BuildKeyMap()
	{
		memset(keymap, 0, sizeof(keymap));
		keymap[EK_a] = KI_A;
		keymap[EK_b] = KI_B;
		keymap[EK_c] = KI_C;
		keymap[EK_d] = KI_D;
		keymap[EK_e] = KI_E;
		keymap[EK_f] = KI_F;
		keymap[EK_g] = KI_G;
		keymap[EK_h] = KI_H;
		keymap[EK_i] = KI_I;
		keymap[EK_j] = KI_J;
		keymap[EK_k] = KI_K;
		keymap[EK_l] = KI_L;
		keymap[EK_m] = KI_M;
		keymap[EK_n] = KI_N;
		keymap[EK_o] = KI_O;
		keymap[EK_p] = KI_P;
		keymap[EK_q] = KI_Q;
		keymap[EK_r] = KI_R;
		keymap[EK_s] = KI_S;
		keymap[EK_t] = KI_T;
		keymap[EK_u] = KI_U;
		keymap[EK_v] = KI_V;
		keymap[EK_w] = KI_W;
		keymap[EK_x] = KI_X;
		keymap[EK_y] = KI_Y;
		keymap[EK_z] = KI_Z;

		keymap[EK_0] = KI_0;
		keymap[EK_1] = KI_1;
		keymap[EK_2] = KI_2;
		keymap[EK_3] = KI_3;
		keymap[EK_4] = KI_4;
		keymap[EK_5] = KI_5;
		keymap[EK_6] = KI_6;
		keymap[EK_7] = KI_7;
		keymap[EK_8] = KI_8;
		keymap[EK_9] = KI_9;

		keymap[EK_KP0] = KI_NUMPAD0;
		keymap[EK_KP1] = KI_NUMPAD1;
		keymap[EK_KP2] = KI_NUMPAD2;
		keymap[EK_KP3] = KI_NUMPAD3;
		keymap[EK_KP4] = KI_NUMPAD4;
		keymap[EK_KP5] = KI_NUMPAD5;
		keymap[EK_KP6] = KI_NUMPAD6;
		keymap[EK_KP7] = KI_NUMPAD7;
		keymap[EK_KP8] = KI_NUMPAD8;
		keymap[EK_KP9] = KI_NUMPAD9;

		keymap[EK_SPACE] = KI_SPACE;
		keymap[EK_BACKSPACE] = KI_BACK;
		keymap[EK_DELETE] = KI_DELETE;
		keymap[EK_RETURN] = KI_RETURN;
		keymap[EK_LEFT] = KI_LEFT;
		keymap[EK_RIGHT] = KI_RIGHT;
		keymap[EK_UP] = KI_UP;
		keymap[EK_DOWN] = KI_DOWN;
		keymap[EK_HOME] = KI_HOME;
		keymap[EK_END] = KI_END;
		keymap[EK_TAB] = KI_TAB;
		keymap[EK_PERIOD] = KI_OEM_PERIOD;
	}

	int GetKeyModifier()
	{
		static Keyboard k;
		int s = 0;
		if (k.KeyDown(EK_LSHIFT) || k.KeyDown(EK_RSHIFT))
			s |= KM_SHIFT;
		if (k.KeyDown(EK_LCTRL) || k.KeyDown(EK_RCTRL))
			s |= KM_CTRL;
		if (k.KeyDown(EK_LALT) || k.KeyDown(EK_RALT))
			s |= KM_ALT;
		if (k.KeyDown(EK_CAPSLOCK))
			s |= KM_CAPSLOCK;
		if (k.KeyDown(EK_NUMLOCK))
			s |= KM_NUMLOCK;
		if (k.KeyDown(EK_SCROLLOCK))
			s |= KM_SCROLLLOCK;

		return s;

	} // GetKeyModifier

	/**
	This map contains 4 different mappings from key identifiers to character codes. Each entry represents a different
	combination of shift and capslock state.
 */

char ascii_map[4][51] =
{
    // shift off and capslock off
    {
		0,
		' ',
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'a',
		'b',
		'c',
		'd',
		'e',
		'f',
		'g',
		'h',
		'i',
		'j',
		'k',
		'l',
		'm',
		'n',
		'o',
		'p',
		'q',
		'r',
		's',
		't',
		'u',
		'v',
		'w',
		'x',
		'y',
		'z',
		';',
		'=',
		',',
		'-',
		'.',
		'/',
		'`',
		'[',
		'\\',
		']',
		'\'',
		0,
		0
	},

	// shift on and capslock off
    {
		0,
		' ',
		')',
		'!',
		'@',
		'#',
		'$',
		'%',
		'^',
		'&',
		'*',
		'(',
		'A',
		'B',
		'C',
		'D',
		'E',
		'F',
		'G',
		'H',
		'I',
		'J',
		'K',
		'L',
		'M',
		'N',
		'O',
		'P',
		'Q',
		'R',
		'S',
		'T',
		'U',
		'V',
		'W',
		'X',
		'Y',
		'Z',
		':',
		'+',
		'<',
		'_',
		'>',
		'?',
		'~',
		'{',
		'|',
		'}',
		'"',
		0,
		0
	},

	// shift on and capslock on
    {
		0,
		' ',
		')',
		'!',
		'@',
		'#',
		'$',
		'%',
		'^',
		'&',
		'*',
		'(',
		'a',
		'b',
		'c',
		'd',
		'e',
		'f',
		'g',
		'h',
		'i',
		'j',
		'k',
		'l',
		'm',
		'n',
		'o',
		'p',
		'q',
		'r',
		's',
		't',
		'u',
		'v',
		'w',
		'x',
		'y',
		'z',
		':',
		'+',
		'<',
		'_',
		'>',
		'?',
		'~',
		'{',
		'|',
		'}',
		'"',
		0,
		0
	},

	// shift off and capslock on
    {
		0,
		' ',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'0',
		'A',
		'B',
		'C',
		'D',
		'E',
		'F',
		'G',
		'H',
		'I',
		'J',
		'K',
		'L',
		'M',
		'N',
		'O',
		'P',
		'Q',
		'R',
		'S',
		'T',
		'U',
		'V',
		'W',
		'X',
		'Y',
		'Z',
		';',
		'=',
		',',
		'-',
		'.',
		'/',
		'`',
		'[',
		'\\',
		']',
		'\'',
		0,
		0
	}      
};

char keypad_map[2][18] = 
{
	{
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'\n',
		'*',
		'+',
		0,
		'-',
		'.',
		'/',
		'='
	},

	{
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		'\n',
		'*',
		'+',
		0,
		'-',
		0,
		'/',
		'='
	}
};

	//------------------------------------------------------------------------------------
	// Returns the character code for a key identifier / key modifier combination.
	//------------------------------------------------------------------------------------
	Rocket::Core::word GetCharacterCode(Rocket::Core::Input::KeyIdentifier key_identifier, int key_modifier_state)
	{
		// Check if we have a keycode capable of generating characters on the main keyboard (ie, not on the numeric
		// keypad; that is dealt with below).
		if (key_identifier <= Rocket::Core::Input::KI_OEM_102)
		{
			// Get modifier states
			bool shift = (key_modifier_state & Rocket::Core::Input::KM_SHIFT) > 0;
			bool capslock = (key_modifier_state & Rocket::Core::Input::KM_CAPSLOCK) > 0;

			// Return character code based on identifier and modifiers
			if (shift && !capslock)
				return ascii_map[1][key_identifier];

			if (shift && capslock)
				return ascii_map[2][key_identifier];	

			if (!shift && capslock)
				return ascii_map[3][key_identifier];

			return ascii_map[0][key_identifier];
		}

		// Check if we have a keycode from the numeric keypad.
		else if (key_identifier <= Rocket::Core::Input::KI_OEM_NEC_EQUAL)
		{
			if (key_modifier_state & Rocket::Core::Input::KM_NUMLOCK)
				return keypad_map[0][key_identifier - Rocket::Core::Input::KI_NUMPAD0];
			else
				return keypad_map[1][key_identifier - Rocket::Core::Input::KI_NUMPAD0];
		}

		else if (key_identifier == Rocket::Core::Input::KI_RETURN)
			return '\n';

		return 0;

	} // GetCharacterCode

} // kge
