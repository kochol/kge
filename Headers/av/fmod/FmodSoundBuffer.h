// File name: FmodSoundBuffer.h
// Des: Fmod Sound buffer
// Date: 15/12/1389 (jalali), Mar-06-2011
// Programmer: Pooya Shahinfar(Pswin)

#ifndef FMOD_SOUND_BUFFER_H
#define FMOD_SOUND_BUFFER_H

#include "../../../include/kge/config.h"
#if KGE_USE_FMOD == 1
#include <fmod.hpp>

namespace kge
{
	namespace av
	{
		class FmodSoundBuffer
		{
		public:
			//! Constructor
			FmodSoundBuffer(FMOD::System* system, const char* filename, bool is3D = true, bool streaming = false);

			//! destructor
			~FmodSoundBuffer();

			//! return a pointer to the FMOD Sound Class
			FMOD::Sound* GetBuffer() const { return m_sound; }

		private:
			FMOD::System*	m_system;
			FMOD::Sound*	m_sound;

		}; // FmodSoundBuffer

	} // av

} // kge

#endif // KGE_USE_FMOD
#endif // FMOD_SOUND_BUFFER_H

