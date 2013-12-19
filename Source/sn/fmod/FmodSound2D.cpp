// File name: FmodSound2D.cpp
// Des: 2D Sound implementation in FMOD
// Date: 16/12/1389 (jalali), Mar-07-2011
// Programmer: Pooya Shahinfar(Pswin)


#include "../../../Headers/sn/fmod/FmodSound2D.h"

#if KGE_USE_FMOD == 1

#include <fmod.hpp>
#include "../../../include/io/Logger.h"


namespace kge
{
	namespace sn
	{

		// constructor
		FmodSound2D::FmodSound2D( FMOD::System* system, FMOD::ChannelGroup* channelGroup,
			FMOD::Sound* pSound, bool stream)
			: m_stream(stream), m_system(system),
			m_group(channelGroup), m_pSound(pSound), m_pChannel(0)
		{
			m_eNodeType = ENT_Sound;
		}

		// destructor
		FmodSound2D::~FmodSound2D()
		{
			if (m_pChannel) { m_pChannel->stop(); }
			//if (m_pSound) { m_pSound->release(); }
		}

		// SetPan
		void FmodSound2D::SetPan(float pan)
		{
			m_pan = pan;
			if (m_pChannel) { m_pChannel->setPan(pan); }
		}

		// SetVolume
		void FmodSound2D::SetVolume(float vol)
		{
			m_volume = vol;
			if (m_pChannel) { m_pChannel->setVolume(vol); }
		}

		void FmodSound2D::SetLoopingState(bool loop)
		{
			m_looping = loop;

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
		av::SourceStates FmodSound2D::GetState() const
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
		av::SourceType FmodSound2D::GetSoundType() const
		{
			return m_stream ? av::SST_STREAMING : av::SST_STATIC;
		}

		// Play
		void FmodSound2D::Play()
		{
			FMOD_RESULT re = m_system->playSound(FMOD_CHANNEL_FREE, m_pSound, true, &m_pChannel);

			if (re != FMOD_OK)
			{
				io::Logger::Log(io::ELM_Error,"Can't play sound source");
				return;
			}

			m_pChannel->setChannelGroup(m_group);

			if (m_looping)
			{
				m_pChannel->setMode(FMOD_LOOP_NORMAL);
			}
			else
			{
				m_pChannel->setMode(FMOD_LOOP_OFF);
			}

			m_pChannel->setPan(m_pan);
			m_pChannel->setVolume(m_volume);
			m_pChannel->setPaused(false);
		}

		// Pause
		void FmodSound2D::Pause()
		{
			if (m_pChannel) { m_pChannel->setPaused(true); }
		}

		// Stop
		void FmodSound2D::Stop()
		{
			if (m_pChannel) { m_pChannel->stop(); }
		}

		//
		void FmodSound2D::SetFading(float value, unsigned int time)
		{
			// TODO
		}

	} // sn

} // kge

#endif // KGE_USE_FMOD

