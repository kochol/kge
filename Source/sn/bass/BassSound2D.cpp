
#include "../../../Headers/sn/bass/BassSound2D.h"

#if KGE_USE_BASS == 1

#include "../../../Libs/bass/bass.h"
#include "../../../include/io/Logger.h"


namespace kge
{
	namespace sn
	{

		// Constructor
		BassSound2D::BassSound2D(DWORD handle, bool stream)
			: m_stream(stream), m_handle(handle), m_channel(0)
		{
			m_eNodeType = ENT_Sound;
		}

		// destructor
		BassSound2D::~BassSound2D()
		{
			BASS_ChannelStop(m_channel);
		}

		// SetPan
		void BassSound2D::SetPan(float pan)
		{
			m_pan = pan;
			BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_PAN, m_pan);
		}

		// SetVolume
		void BassSound2D::SetVolume(float vol)
		{
			m_volume = vol;
			BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_volume);
		}

		void BassSound2D::SetLoopingState(bool loop)
		{
			m_looping = loop;

			if (m_looping)
			{
				BASS_ChannelFlags(m_channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);	
			}
			else
			{
				BASS_ChannelFlags(m_channel, 0, BASS_SAMPLE_LOOP);
			}
		}

		// GetState
		av::SourceStates BassSound2D::GetState() const
		{
			const DWORD result = BASS_ChannelIsActive(m_channel);

			if (result == BASS_ACTIVE_PLAYING)
			{
				return av::SSS_PLAYING;
			}
			else if (result == BASS_ACTIVE_PAUSED)
			{
				return av::SSS_PAUSED;
			}

			return av::SSS_STOPPED;
		}

		// GetSourceType
		av::SourceType BassSound2D::GetSoundType() const
		{
			return m_stream ? av::SST_STREAMING : av::SST_STATIC;
		}

		// Play
		void BassSound2D::Play()
		{
			if (!m_stream)
			{
				m_channel = BASS_SampleGetChannel(m_handle, FALSE);
			}
			else
			{
				m_channel = m_handle;
			}
			
			if (m_looping)
			{
				BASS_ChannelFlags(m_channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);	
			}
			else
			{
				BASS_ChannelFlags(m_channel, 0, BASS_SAMPLE_LOOP);
			}

			BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_PAN, m_pan);
			BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_volume);

			BASS_ChannelPlay(m_channel, FALSE);
		}

		// Pause
		void BassSound2D::Pause()
		{
			BASS_ChannelPause(m_channel);
		}

		// Stop
		void BassSound2D::Stop()
		{
			BASS_ChannelStop(m_channel);
			m_channel = 0;
		}

		//
		void BassSound2D::SetFading(float value, unsigned int time)
		{
			// value: 0 (silent) to 1 (full)
			// time: in milliseconds
			BASS_ChannelSlideAttribute(m_channel, BASS_ATTRIB_VOL, value, time);
		}

	} // sn

} // kge

#endif // KGE_USE_BASS

