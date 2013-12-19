
#ifndef BASS_SOUND_2D_H
#define BASS_SOUND_2D_H

#include "../../../Include/sn/Sound2D.h"

#if KGE_USE_BASS == 1

#include "../../av/bass/BassSoundBuffer.h"
#include "../../../Libs/bass/bass.h"

namespace kge
{
	namespace sn
	{
		class BassSound2D : public sn::Sound2D
		{
		public:
			//! constructor
			BassSound2D(DWORD handle, bool stream);

			//! destructor
			~BassSound2D();

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
			const bool	m_stream;
			DWORD		m_handle;
			HCHANNEL	m_channel;
		};

	} // sn
} // kge

#endif // KGE_USE_BASS
#endif // BASS_SOUND_2D_H

