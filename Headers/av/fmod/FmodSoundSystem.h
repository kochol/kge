// File name: FmodSoundSystem.h
// Des: FMOD
// Date: 7/12/1389 (jalali), Feb-27-2011
// Programmers: Pooya Shahinfar (Pswin)

#ifndef FMOD_SOUND_SYSTEM_H
#define FMOD_SOUND_SYSTEM_H

#include "../../../include/kge/kgedef.h"
#include "../../../include/kge/av/soundsystem.h"
#include <vector>
#include <map>
#include <string>

#if KGE_USE_FMOD == 1
#include <fmod.hpp>

namespace kge
{
	namespace av
	{
		class FmodSoundBuffer;

		class FmodSoundSystem : public SoundSystem
		{
		public:
			//! constructor
			FmodSoundSystem(float musicVolume, float effectsVolume);

			//! destructor
			~FmodSoundSystem();

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
			FMOD::System*		m_system;
			FMOD::ChannelGroup*	m_musicGroup;
			FMOD::ChannelGroup*	m_effectsGroup;

			typedef std::map<std::string, FmodSoundBuffer*> BufferMap;
			BufferMap			m_buffers;

			struct Listener
			{
				FMOD_VECTOR position;
				FMOD_VECTOR velocity;
				FMOD_VECTOR forward;
				FMOD_VECTOR up;
			};

			Listener			m_listener;

		};

	} // av

} // kge

#endif // KGE_USE_FMOD
#endif // FMOD_SOUND_SYSTEM_H

