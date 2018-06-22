// File name: soundsystem.h
// Des: interface for the sound systems
// Date: 6/12/1389 (jalali), Feb-26-2011
// Programmer: Pooya Shahinfar(Pswin)


#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "../kgedef.h"
#include "SoundBuffer.h"
#include "../sn/Sound3D.h"
#include "../sn/Sound2D.h"

namespace kge
{
	namespace av
	{

		// SoundSystemType
		enum SoundSystemType
		{
			ESST_OPENAL,	//! OpenAL
			ESST_FMOD,		//! FMOD
			ESST_BASS,		//! BASS
		}; 

		// SoundSystem
		class SoundSystem
		{
		public:
			//! constructor
			SoundSystem(SoundSystemType type, float musicVolume, float effectsVolume)
				: m_eType(type), m_musicVolume(musicVolume), m_effectsVolume(effectsVolume) {}

			virtual ~SoundSystem() {}

			virtual bool Init(void* hwnd) = 0;

			//! creates a 3D sound and returns a pointer to it
			virtual sn::Sound3D* CreateSound3D(const char* filename) = 0; 

			//! creates a 2D sound and return a pointer to it
			virtual sn::Sound2D* CreateSound2D(const char* filename, bool streaming) = 0;

			//!
			virtual void SetListenerPosition(const math::Vector& pos) = 0;

			//!
			virtual void SetListenerVelocity(const math::Vector& vel) = 0;

			//!
			virtual void SetListenerOrientation(const math::Vector& forward, const math::Vector& up) = 0;

			//! update sound system
			virtual void Update() = 0;

			//! volume: [0-1000]
			virtual void SetMusicVolume(float volume) = 0;

			//! 
			float GetMusicVolume() const { return m_musicVolume; }

			//! volume: [0-1000]
			virtual void SetEffectsVolume(float volume) = 0;

			//! 
			float GetEffectsVolume() const { return m_effectsVolume; }

			//!
			virtual void Pause(bool pause) = 0;

			//!
			virtual void Stop() = 0;

			//! returns type of the sound system
			SoundSystemType GetType() const { return m_eType; }

		protected:
			float           m_musicVolume;
			float           m_effectsVolume;

		private:
			SoundSystemType m_eType;

		}; // SoundSystem

	} // av

} // kge

#endif // SOUND_SYSTEM_H