
#ifndef BASS_SOUDN3D_H
#define BASS_SOUDN3D_H

#include "../../../Include/sn/Sound3D.h"

#if KGE_USE_BASS == 1

#include "../../av/bass/BassSoundBuffer.h"
#include "../../../Libs/bass/bass.h"

namespace kge
{
	namespace sn
	{
		class BassSound3D : public sn::Sound3D
		{
		public:
			//! Constructor
			BassSound3D(DWORD handle);

			//! Destructor
			~BassSound3D();

			//! Set the position of the sound.
			void SetPosition(const math::Vector& v);

			//! Set sound source velocity
			void SetVelocity(const math::Vector& velocity);

			//! Set the sound source volume 
			void SetVolume (float value);

			//! Sets looping of sound
			void SetLoopingState(bool looping);

			//! sound source state
			av::SourceStates GetState() const;

			//! sound source type (static, streaming, ...)
			av::SourceType GetSoundType() const;

			//! play source (sound)
			void Play();

			//! pause playing source (sound)
			void Pause();

			//! stop playing source (sound)
			void Stop();

			//! set max and min distance
			void SetMinMaxDistance(float _min, float _max = 10000.0f);

			//!
			void SetFading(float value, unsigned int time);

		private:
			DWORD		m_handle;
			float		m_minDistance;
			float		m_maxDistance;
			HCHANNEL	m_channel;
		};

} // sn
} // kge

#endif // KGE_USE_BASS
#endif // BASS_SOUDN3D_H

