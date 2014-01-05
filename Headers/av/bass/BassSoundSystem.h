
#ifndef BASS_SOUND_SYSTEM_H
#define BASS_SOUND_SYSTEM_H

#include "../../../Include/kgedef.h"
#include "../../../Include/av/soundsystem.h"
#include <vector>
#include <map>
#include <string>

#include "../../../Libs/bass/bass.h"

#if KGE_USE_BASS == 1

namespace kge
{
	namespace av
	{
		class BassSoundBuffer;

		class BassSoundSystem : public SoundSystem
		{
		public:
			//! constructor
			BassSoundSystem(float musicVolume, float effectsVolume);

			//! destructor
			~BassSoundSystem();

			// initialization
			bool Init(void* hwnd);

			//! creates a 3D sound and returns a pointer to it
			sn::Sound3D* CreateSound3D(const char* filename); 

			//! creates a 2D sound and return a pointer to it
			sn::Sound2D* CreateSound2D(const char* filename, bool streaming);

			//!
			void SetListenerPosition(const math::Vector& pos);

			//!
			void SetListenerVelocity(const math::Vector& vel);

			//!
			void SetListenerOrientation(const math::Vector& front, const math::Vector& top);

			//! update sound system
			void Update();

			//!
			void SetMusicVolume(float volume);

			//!
			void SetEffectsVolume(float volume);

			//!
			void Pause(bool pause);

			//!
			void Stop();

		private:
			//! creates a sound buffer and return a pointer to it
			av::BassSoundBuffer* CreateSoundBuffer(const char* filename, bool is3D, bool streaming);

			typedef std::map<std::string, BassSoundBuffer*> BufferMap;
			BufferMap	m_buffers;

			struct Listener
			{
				BASS_3DVECTOR position;
				BASS_3DVECTOR velocity;
				BASS_3DVECTOR front;
				BASS_3DVECTOR top;
			};

			Listener	m_listener;
		};

	} // av

} // kge

#endif // KGE_USE_BASS
#endif // BASS_SOUND_SYSTEM_H

