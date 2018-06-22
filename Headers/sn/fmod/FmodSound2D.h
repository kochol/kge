// File name: FmodSound2D.h
// Des: FMOD Sound 2D interface
// Date: 16/12/1389 (jalali), Mar-07-2011
// Programmer: Pooya Shahinfar(Pswin)

#ifndef FMOD_SOUND_2D_H
#define FMOD_SOUND_2D_H

#include "../../../include/kge/sn/Sound2D.h"
#include "../../av/fmod/FmodSoundBuffer.h"

#if KGE_USE_FMOD == 1
#include <fmod.hpp>

// FMOD class prototypes
namespace FMOD
{
	class Sound;
	class Channel;
} // FMOD

namespace kge
{
	namespace sn
	{
		class FmodSound2D : public sn::Sound2D
		{
		public:
			//! constructor
			FmodSound2D(FMOD::System* system, FMOD::ChannelGroup* channelGroup,
				FMOD::Sound* pSound, bool stream);

			//! destructor
			~FmodSound2D();

			//! sets pan of sound in stereo speakers 
			void SetPan(float pan);

			//! sets volume of the sound
			void SetVolume(float vol);

			//! sets lopping of the sound
			void SetLoopingState(bool loop);

			//! sound source state
			av::SourceStates GetState() const;

			//! sound source type (static, streaming, ...)
			av::SourceType GetSoundType() const;

			//! plays sound 
			void Play();

			//! pause playing sound
			void Pause();

			//! stops playing sound
			void Stop();

			//!
			void SetFading(float value, unsigned int time);

		private:
			const bool			m_stream;
			FMOD::System*		m_system;
			FMOD::ChannelGroup* m_group;
			FMOD::Sound*		m_pSound;
			FMOD::Channel*		m_pChannel;

		}; // FmodSound2D

	} // sn

} // kge

#endif // KGE_USE_FMOD
#endif // FMOD_SOUND_2D_H

