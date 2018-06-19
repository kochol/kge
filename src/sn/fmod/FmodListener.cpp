// File name: FmodListener.cpp
// Des: Listener implementation in FMOD
// Date: 7/12/1389 (jalali), Feb-27-2011
// Programmers: Pooya Shahinfar (Pswin)

#include "../../../Include/kgedef.h"
#include "../../../Headers/sn/fmod/FmodListener.h"

#if KGE_USE_FMOD == 1


#include <fmod.hpp>
#include "../../../include/io/Logger.h"

#if KGE_COMPILER == KGE_COMPILER_MSVC
	#pragma comment(lib,"fmodex_vc.lib")
#endif


namespace kge
{
	namespace sn
	{

		// constructor
		FmodListener::FmodListener( FMOD::System* system, u32 _index)
			: m_uIndex(_index), m_system(system)
		{
		}

		// destructor
		FmodListener::~FmodListener()
		{
		}

		// SetVelocity
		void FmodListener::SetVelocity( const math::Vector& v)
		{
			m_vVelocity = v;
			FMOD_VECTOR pos = {m_Position.x,m_Position.y,m_Position.z};
			FMOD_VECTOR vel = {m_vVelocity.x,m_vVelocity.y,m_vVelocity.z};
			FMOD_VECTOR forward = {m_vAt.x,m_vAt.y,m_vAt.z};
			FMOD_VECTOR up = {m_vUp.x,m_vUp.y,m_vUp.z};

			m_system->set3DListenerAttributes(m_uIndex,&pos,&vel,&forward,&up);
		}

		// SetOirentation
		void FmodListener::SetOrientation( const math::Vector& _up, const math::Vector& _at )
		{
			m_vAt = _at;
			m_vUp = _up;
			FMOD_VECTOR pos = {m_Position.x,m_Position.y,m_Position.z};
			FMOD_VECTOR vel = {m_vVelocity.x,m_vVelocity.y,m_vVelocity.z};
			FMOD_VECTOR forward = {m_vAt.x,m_vAt.y,m_vAt.z};
			FMOD_VECTOR up = {m_vUp.x,m_vUp.y,m_vUp.z};

			m_system->set3DListenerAttributes(m_uIndex,&pos,&vel,&forward,&up);
		}

		// SetGain
		void FmodListener::SetGain( float value )
		{
			m_fGain = value;
		}

		// SetPosition
		void FmodListener::SetPosition( const math::Vector& v )
		{
			m_Position = v;
			FMOD_VECTOR pos = {m_Position.x,m_Position.y,m_Position.z};
			FMOD_VECTOR vel = {m_vVelocity.x,m_vVelocity.y,m_vVelocity.z};
			FMOD_VECTOR forward = {m_vAt.x,m_vAt.y,m_vAt.z};
			FMOD_VECTOR up = {m_vUp.x,m_vUp.y,m_vUp.z};

			m_system->set3DListenerAttributes(m_uIndex,&pos,&vel,&forward,&up);
		}

	} // sn
} // Kochol Game Engine

#endif // KGE_USE_FMOD