
#include "../../../Headers/av/bass/BassSoundSystem.h"
#include "../../../Headers/av/bass/BassSoundBuffer.h"
#include "../../../Headers/sn/bass/BassSound3D.h"
#include "../../../Headers/sn/bass/BassSound2D.h"
#include "../../../include/kge/io/Logger.h"

#if KGE_USE_BASS == 1

#if KGE_COMPILER == KGE_COMPILER_MSVC
#pragma comment(lib, "bass.lib")
#endif // KGE_COMPILER_MSVC

#if (KGE_PLATFORM == KGE_PLATFORM_WINDOWS)
#include <algorithm>
#endif // KGE_PLATFORM_WINDOWS

namespace kge
{
	namespace av
	{

		// Constructor
		BassSoundSystem::BassSoundSystem(float musicVolume, float effectsVolume)
			: SoundSystem(ESST_BASS, musicVolume, effectsVolume)
		{

		}

		// Destructor
		BassSoundSystem::~BassSoundSystem()
		{
			for (BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); ++it)
			{
				delete it->second;
			}

			BASS_Free();
		}

		// Initialization
		bool BassSoundSystem::Init(void* hwnd)
		{
			if (HIWORD(BASS_GetVersion()) != BASSVERSION) { return false; }

#if (KGE_PLATFORM == KGE_PLATFORM_WINDOWS)
			if (!BASS_Init(-1, 44100, BASS_DEVICE_3D, (HWND)hwnd, NULL)) { return false; }
#else
			if (!BASS_Init(-1, 44100, BASS_DEVICE_3D, NULL, NULL)) { return false; }
#endif // KGE_PLATFORM_WINDOWS

			if (!BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 0)) { return false; }

			if (!BASS_Set3DFactors(1.0f, 1.0f, 1.0f)) { return false; }

			m_listener.position.x = 0.0f;
			m_listener.position.y = 0.0f;
			m_listener.position.z = 0.0f;

			m_listener.velocity.x = 0.0f;
			m_listener.velocity.y = 0.0f;
			m_listener.velocity.z = 0.0f;

			m_listener.front.x = 0.0f;
			m_listener.front.y = 0.0f;
			m_listener.front.z = 1.0f;

			m_listener.top.x = 0.0f;
			m_listener.top.y = 1.0f;
			m_listener.top.z = 0.0f;

			if (!BASS_Set3DPosition(&m_listener.position, &m_listener.velocity,
				&m_listener.front, &m_listener.top))
			{
				return false;
			}

			BASS_Apply3D();

			io::Logger::Log(io::ELM_Information, "Using BASS for sound system.");

			return true;
		}

		// CreateSound3D
		sn::Sound3D* BassSoundSystem::CreateSound3D(const char* filename)
		{
			std::string name(filename);

#if (KGE_PLATFORM == KGE_PLATFORM_WINDOWS)
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
#endif // KGE_PLATFORM_WINDOWS

			BufferMap::const_iterator it = m_buffers.find(name);

			if (it != m_buffers.end())
			{
				return KGE_NEW(sn::BassSound3D)(it->second->GetHandle());
			}
			else
			{
				av::BassSoundBuffer *pBuf = KGE_NEW(BassSoundBuffer)(filename, true, false);
				m_buffers[name] = pBuf;
				return KGE_NEW(sn::BassSound3D)(pBuf->GetHandle());
			}
		}

		// CreateSound2D
		sn::Sound2D* BassSoundSystem::CreateSound2D(const char* filename, bool streaming)
		{
			std::string name(filename);

#if (KGE_PLATFORM == KGE_PLATFORM_WINDOWS)
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
#endif // KGE_PLATFORM_WINDOWS

			BufferMap::const_iterator it = m_buffers.find(name);

			if (it != m_buffers.end())
			{
				return KGE_NEW(sn::BassSound2D)(it->second->GetHandle(), streaming);
			}
			else
			{
				av::BassSoundBuffer *pBuf = KGE_NEW(av::BassSoundBuffer)(filename, false, streaming);
				m_buffers[name] = pBuf;
				return KGE_NEW(sn::BassSound2D)(pBuf->GetHandle(), streaming);
			}
		}

		//
		void BassSoundSystem::SetListenerPosition(const math::Vector& pos)
		{
			m_listener.position.x = pos.x;
			m_listener.position.y = pos.y;
			m_listener.position.z = pos.z;

			BASS_Set3DPosition(&m_listener.position, NULL, NULL, NULL);
			BASS_Apply3D();
		}

		//
		void BassSoundSystem::SetListenerVelocity(const math::Vector& vel)
		{
			m_listener.velocity.x = vel.x;
			m_listener.velocity.y = vel.y;
			m_listener.velocity.z = vel.z;

			BASS_Set3DPosition(NULL, &m_listener.velocity, NULL, NULL);
			BASS_Apply3D();
		}

		//
		void BassSoundSystem::SetListenerOrientation(const math::Vector& front, const math::Vector& top)
		{
			m_listener.front.x = front.x;
			m_listener.front.y = front.y;
			m_listener.front.z = front.z;

			m_listener.top.x = top.x;
			m_listener.top.y = top.y;
			m_listener.top.z = top.z;

			BASS_Set3DPosition(NULL, NULL, &m_listener.front, &m_listener.top);
			BASS_Apply3D();
		}

		// Update
		void BassSoundSystem::Update()
		{
			BASS_Update(100);
		}

		//
		void BassSoundSystem::SetMusicVolume(float volume)
		{
			if (volume < 0.0f)
			{
				volume = 0.0f;
			}
			else if (volume > 1000.0f)
			{
				volume = 1000.0f;
			}

			m_musicVolume = volume * 10.0f;

			DWORD vol = static_cast<DWORD>(m_musicVolume);

			if (vol > 9999) { vol = 9999; }
			else if (vol < 1) { vol = 1; }

			BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol);
		}

		//
		void BassSoundSystem::SetEffectsVolume(float volume)
		{
			if (volume < 0.0f)
			{
				volume = 0.0f;
			}
			else if (volume > 1000.0f)
			{
				volume = 1000.0f;
			}

			m_effectsVolume = volume * 10.0f;

			DWORD vol = static_cast<DWORD>(m_effectsVolume);

			if (vol > 9999) { vol = 9999; }
			else if (vol < 1) { vol = 1; }

			BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, vol);
		}

		//
		void BassSoundSystem::Pause(bool pause)
		{
			if (pause)
			{
				BASS_Pause();
			}
			else
			{
				BASS_Start();
			}
		}

		//
		void BassSoundSystem::Stop()
		{
			BASS_Stop();
		}

	} // av

} // kge

#endif // KGE_USE_BASS

