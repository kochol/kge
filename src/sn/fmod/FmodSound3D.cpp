// File name: FmodSound3D.cpp
// Des: interface for 3D sound sources
// Date: 6/15/1389 (jalali), Mar-06-2011
// Programmers: Pooya Shahinfar (Pswin)

#include "../../../Headers/sn/fmod/FmodSound3D.h"

#if KGE_USE_FMOD == 1

#include <fmod.hpp>
#include "../../../include/io/Logger.h"


namespace kge
{
	namespace sn
	{
		// Constructor
		FmodSound3D::FmodSound3D(FMOD::System* system, FMOD::ChannelGroup* channelGroup,
			FMOD::Sound* pSound)
			: m_system(system), m_group(channelGroup), m_pSound(pSound),
			m_pChannel(0), m_fMinDistance(1.0f), m_fMaxDistance(10000.0f)
		{
			m_eNodeType = ENT_Sound;
		}

		// destructor
		FmodSound3D::~FmodSound3D()
		{
			if (m_pChannel) { m_pChannel->stop(); }
			//if (m_pSound) { m_pSound->release(); }
		}

		// SetPosition
		void FmodSound3D::SetPosition(const math::Vector &v)
		{
			m_Position = v;
			FMOD_VECTOR pos = {m_Position.x,m_Position.y,m_Position.z};
			FMOD_VECTOR vel = {m_velocity.x,m_velocity.y,m_velocity.z};
			if (m_pChannel) { m_pChannel->set3DAttributes(&pos,&vel); }
		}

		// SetVelocity
		void FmodSound3D::SetVelocity(const math::Vector& v)
		{
			m_velocity = v;
			FMOD_VECTOR pos = {m_Position.x,m_Position.y,m_Position.z};
			FMOD_VECTOR vel = {m_velocity.x,m_velocity.y,m_velocity.z};
			if (m_pChannel) { m_pChannel->set3DAttributes(&pos,&vel); }
		}

		// SetVolume
		void FmodSound3D::SetVolume(float value)
		{
			m_volume = value;
			if (m_pChannel) { m_pChannel->setVolume(value); }
		}

		// SetLoopingState
		void FmodSound3D::SetLoopingState(bool looping)
		{
			m_looping = looping;

			if (m_pChannel)
			{
				if (m_looping)
				{
					m_pChannel->setMode(FMOD_LOOP_NORMAL);
				}
				else
				{
					m_pChannel->setMode(FMOD_LOOP_OFF);
				}
			}
		}

		// GetState
		av::SourceStates FmodSound3D::GetState() const
		{
			bool bTest = false;

			if (m_pChannel) { m_pChannel->isPlaying(&bTest); }

			if (bTest)
			{
				return av::SSS_PLAYING;
			}

			if (m_pChannel) { m_pChannel->getPaused(&bTest); }

			if (bTest)
			{
				return av::SSS_PAUSED;
			}

			return av::SSS_STOPPED;
		}

		// GetSourceType
		av::SourceType FmodSound3D::GetSoundType() const
		{
			return av::SST_STATIC;
		}

		// play
		void FmodSound3D::Play()
		{			
			FMOD_RESULT re = m_system->playSound(FMOD_CHANNEL_FREE,m_pSound,true,&m_pChannel);

			if (re != FMOD_OK)
			{
				io::Logger::Log(io::ELM_Error,"Can't play sound source");
				return;
			}

			m_pChannel->setChannelGroup(m_group);

			FMOD_VECTOR pos = {m_Position.x,m_Position.y,m_Position.z };
			FMOD_VECTOR vel = {m_velocity.x,m_velocity.y,m_velocity.z};
			m_pChannel->set3DAttributes(&pos,&vel);

			if (m_looping)
			{
				m_pChannel->setMode(FMOD_LOOP_NORMAL);
			}
			else
			{
				m_pChannel->setMode(FMOD_LOOP_OFF);
			}

			m_pChannel->setVolume(m_volume);
			m_pChannel->set3DMinMaxDistance(m_fMinDistance,m_fMaxDistance);
			m_pChannel->setPaused(false);
		}

		// Pause
		void FmodSound3D::Pause()
		{
			if (m_pChannel) { m_pChannel->setPaused(true); }
		}

		// Stop
		void FmodSound3D::Stop()
		{
			if (m_pChannel) { m_pChannel->stop(); }
		}

		// SetMinMaxDistance
		void FmodSound3D::SetMinMaxDistance( float _min, float _max /*= 10000.0f*/ )
		{
			m_pChannel->set3DMinMaxDistance(_min, _max);
			m_fMinDistance = _min;
			m_fMaxDistance = _max;
		}

		//
		void FmodSound3D::SetFading(float value, unsigned int time)
		{
			// TODO
		}

	} // sn

} // kge

#endif // KGE_USE_FMOD

