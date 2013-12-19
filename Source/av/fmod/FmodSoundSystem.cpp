// File name: FmodSoundSystem.h
// Des: FMOD
// Date: 7/12/1389 (jalali), Feb-27-2011
// Programmers: Pooya Shahinfar (Pswin)

#include "../../../Headers/av/fmod/FmodSoundSystem.h"

#if KGE_USE_FMOD == 1

#include "../../../Headers/av/fmod/FmodSoundBuffer.h"
#include "../../../Headers/sn/fmod/FmodSound3D.h"
#include "../../../Headers/sn/fmod/FmodSound2D.h"
#include "../../../include/io/Logger.h"

#if KGE_COMPILER == KGE_COMPILER_MSVC
#pragma comment(lib,"fmodex_vc.lib")
#endif

#if (KGE_PLATFORM == KGE_PLATFORM_WINDOWS)
#include <algorithm>
#endif


namespace kge
{
	namespace av
	{

		// constructor
		FmodSoundSystem::FmodSoundSystem(float musicVolume, float effectsVolume)
			: SoundSystem(ESST_FMOD, musicVolume, effectsVolume),
			m_system(0), m_musicGroup(0), m_effectsGroup(0)
		{

		}

		// destructor
		FmodSoundSystem::~FmodSoundSystem()
		{
			for (BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); ++it)
			{
				delete it->second;
			}

			m_effectsGroup->release();
			m_musicGroup->release();
			m_system->close();
			m_system->release();
		}

		bool FmodSoundSystem::Init(void* hwnd)
		{
			FMOD_RESULT re = FMOD::System_Create(&m_system);

			if (re != FMOD_OK)
			{
				io::Logger::Log(io::ELM_Error, "FMOD system creation failed");
				return false;
			}

			re = m_system->init(100, FMOD_INIT_NORMAL, NULL/*hwnd*/);

			if (re != FMOD_OK)
			{
				io::Logger::Log(io::ELM_Error, "Initialization FMOD failed.");
				return false;
			}

			// setting up 3D settings
			re = m_system->set3DSettings(1.0f, 1.0f, 1.0f);

			if (re != FMOD_OK)
			{
				io::Logger::Log(io::ELM_Error, "Sound system can't setup 3D settings.");
				return false;
			}

			re = m_system->createChannelGroup("MusicGroup", &m_musicGroup);

			if (re != FMOD_OK)
			{
				io::Logger::Log(io::ELM_Error, "Sound system can't create channel group.");
				return false;
			}

			re = m_system->createChannelGroup("EffectsGroup", &m_effectsGroup);

			if (re != FMOD_OK)
			{
				io::Logger::Log(io::ELM_Error, "Sound system can't create channel group.");
				return false;
			}

			m_listener.position.x = 0.0f;
			m_listener.position.y = 0.0f;
			m_listener.position.z = 0.0f;

			m_listener.velocity.x = 0.0f;
			m_listener.velocity.y = 0.0f;
			m_listener.velocity.z = 0.0f;

			m_listener.forward.x = 0.0f;
			m_listener.forward.y = 0.0f;
			m_listener.forward.z = 1.0f;

			m_listener.up.x = 0.0f;
			m_listener.up.y = 1.0f;
			m_listener.up.z = 0.0f;

			re = m_system->set3DListenerAttributes(0,
				&m_listener.position,
				&m_listener.velocity,
				&m_listener.forward,
				&m_listener.up);

			if (re != FMOD_OK)
			{
				io::Logger::Log(io::ELM_Error, "Sound system can't set listener attributes.");
				return false;
			}

			io::Logger::Log(io::ELM_Information, "Using FMOD for sound system.");

			return true;
		}

		// CreateSound3D
		sn::Sound3D* FmodSoundSystem::CreateSound3D( const char* filename )
		{
			std::string name(filename);

#if (KGE_PLATFORM == KGE_PLATFORM_WINDOWS)
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
#endif

			BufferMap::const_iterator it = m_buffers.find(name);

			if (it != m_buffers.end())
			{
				return KGE_NEW(sn::FmodSound3D)(m_system, m_effectsGroup, it->second->GetBuffer());
			}
			else
			{
				av::FmodSoundBuffer *pBuf = KGE_NEW(FmodSoundBuffer)(m_system, filename, true, false);
				m_buffers[name] = pBuf;
				return KGE_NEW(sn::FmodSound3D)(m_system, m_effectsGroup, pBuf->GetBuffer());
			}
		}

		// CreateSound2D
		sn::Sound2D* FmodSoundSystem::CreateSound2D( const char* filename, bool streaming /*= false*/ )
		{
			std::string name(filename);

#if (KGE_PLATFORM == KGE_PLATFORM_WINDOWS)
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
#endif

			BufferMap::const_iterator it = m_buffers.find(name);

			if (it != m_buffers.end())
			{
				return KGE_NEW(sn::FmodSound2D)(m_system, streaming ? m_musicGroup : m_effectsGroup,
					it->second->GetBuffer(), streaming);
			}
			else
			{
				av::FmodSoundBuffer *pBuf = KGE_NEW(av::FmodSoundBuffer)(m_system, filename, false, streaming);
				m_buffers[name] = pBuf;
				return KGE_NEW(sn::FmodSound2D)(m_system, streaming ? m_musicGroup : m_effectsGroup,
					pBuf->GetBuffer(), streaming);
			}
		}

		//
		void FmodSoundSystem::SetListenerPosition(const math::Vector& pos)
		{
			m_listener.position.x = pos.x;
			m_listener.position.y = pos.y;
			m_listener.position.z = pos.z;

			m_system->set3DListenerAttributes(0, &m_listener.position, NULL, NULL, NULL);
		}

		//
		void FmodSoundSystem::SetListenerVelocity(const math::Vector& vel)
		{
			m_listener.velocity.x = vel.x;
			m_listener.velocity.y = vel.y;
			m_listener.velocity.z = vel.z;

			m_system->set3DListenerAttributes(0, NULL, &m_listener.velocity, NULL, NULL);
		}

		//
		void FmodSoundSystem::SetListenerOrientation(const math::Vector& front, const math::Vector& top)
		{
			m_listener.forward.x = front.x;
			m_listener.forward.y = front.y;
			m_listener.forward.z = front.z;

			m_listener.up.x = top.x;
			m_listener.up.y = top.y;
			m_listener.up.z = top.z;

			m_system->set3DListenerAttributes(0, NULL, NULL, &m_listener.forward, &m_listener.up);
		}

		// Update
		void FmodSoundSystem::Update()
		{
			m_system->update();
		}

		//
		void FmodSoundSystem::SetMusicVolume(float volume)
		{
			if (volume > 1000.0f)
			{
				volume = 1000.0f;
			}
			else if (volume < 0.0f)
			{
				volume = 0.0f;
			}

			m_musicVolume = volume * 0.001f;

			//FMOD::ChannelGroup* group = 0;
			//
			//if (m_system->getMasterChannelGroup(&group) == FMOD_OK)
			//{
			//	group->setVolume(m_musicVolume);
			//}

			m_musicGroup->setVolume(m_musicVolume);
		}

		//
		void FmodSoundSystem::SetEffectsVolume(float volume)
		{
			if (volume > 1000.0f)
			{
				volume = 1000.0f;
			}
			else if (volume < 0.0f)
			{
				volume = 0.0f;
			}

			m_effectsVolume = volume * 0.001f;
			
			//FMOD::ChannelGroup* group = 0;

			//if (m_system->getMasterChannelGroup(&group) == FMOD_OK)
			//{
			//	group->setVolume(m_effectsVolume);
			//}

			m_effectsGroup->setVolume(m_effectsVolume);
		}

		//
		void FmodSoundSystem::Pause(bool pause)
		{
			//FMOD::ChannelGroup* group = 0;

			//if (m_system->getMasterChannelGroup(&group) == FMOD_OK)
			//{
			//	group->setPaused(pause);
			//}

			m_musicGroup->setPaused(pause);
			m_effectsGroup->setPaused(pause);
		}

		//
		void FmodSoundSystem::Stop()
		{
			//FMOD::ChannelGroup* group = 0;

			//if (m_system->getMasterChannelGroup(&group) == FMOD_OK)
			//{
			//	group->stop();
			//}

			m_musicGroup->stop();
			m_effectsGroup->stop();
		}

	} // av
} // kge

#endif // KGE_USE_FMOD

