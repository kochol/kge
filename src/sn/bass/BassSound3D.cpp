
#include "../../../Headers/sn/bass/BassSound3D.h"

#if KGE_USE_BASS == 1

#include "../../../Libs/bass/bass.h"
#include "../../../include/kge/io/Logger.h"

namespace kge
{
	namespace sn
	{
		// Constructor
		BassSound3D::BassSound3D(DWORD handle)
			: m_handle(handle), m_minDistance(1.0f), m_maxDistance(10000.0f), m_channel(0)
		{
			m_eNodeType = ENT_Sound;
		}

		// destructor
		BassSound3D::~BassSound3D()
		{
			BASS_ChannelStop(m_channel);
		}

		// SetPosition
		void BassSound3D::SetPosition(const math::Vector &v)
		{
			m_Position = v;
			BASS_3DVECTOR pos(m_Position.x, m_Position.y, m_Position.z);
			BASS_ChannelSet3DPosition(m_channel, &pos, NULL, NULL);
			BASS_Apply3D();
		}

		// SetVelocity
		void BassSound3D::SetVelocity(const math::Vector& v)
		{
			m_velocity = v;
			BASS_3DVECTOR vel(m_velocity.x, m_velocity.y, m_velocity.z);
			BASS_ChannelSet3DPosition(m_channel, NULL, NULL, &vel);
			BASS_Apply3D();
		}

		// SetVolume
		void BassSound3D::SetVolume(float value)
		{
			m_volume = value;
			BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_volume);
		}

		// SetLoopingState
		void BassSound3D::SetLoopingState(bool loop)
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
		av::SourceStates BassSound3D::GetState() const
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
		av::SourceType BassSound3D::GetSoundType() const
		{
			return av::SST_STATIC;
		}

		// play
		void BassSound3D::Play()
		{
			m_channel = BASS_SampleGetChannel(m_handle, FALSE);

			if (m_looping)
			{
				BASS_ChannelFlags(m_channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
			}
			else
			{
				BASS_ChannelFlags(m_channel, 0, BASS_SAMPLE_LOOP);
			}

			BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_volume);

			BASS_ChannelSet3DAttributes(m_channel, -1, m_minDistance, m_maxDistance, -1, -1, -1);

			BASS_3DVECTOR pos(m_Position.x, m_Position.y, m_Position.z);
			BASS_3DVECTOR vel(m_velocity.x, m_velocity.y, m_velocity.z);
			BASS_ChannelSet3DPosition(m_channel, &pos, NULL, &vel);

			BASS_Apply3D();

			BASS_ChannelPlay(m_channel, FALSE);
		}

		// Pause
		void BassSound3D::Pause()
		{
			BASS_ChannelPause(m_channel);
		}

		// Stop
		void BassSound3D::Stop()
		{
			BASS_ChannelStop(m_channel);
			m_channel = 0;
		}

		// SetMinMaxDistance
		void BassSound3D::SetMinMaxDistance(float _min, float _max /*= 10000.0f*/)
		{
			m_minDistance = _min;
			m_maxDistance = _max;

			BASS_ChannelSet3DAttributes(m_channel, -1, m_minDistance, m_maxDistance, -1, -1, -1);
			BASS_Apply3D();
		}

		//
		void BassSound3D::SetFading(float value, unsigned int time)
		{
			// value: 0 (silent) to 1 (full)
			// time: in milliseconds
			BASS_ChannelSlideAttribute(m_channel, BASS_ATTRIB_VOL, value, time);
		}

	} // sn

} // kge

#endif // KGE_USE_BASS

