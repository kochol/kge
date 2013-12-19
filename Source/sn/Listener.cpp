// File name: Listener.cpp
// Des: This class is for working with sound listener in 3D space.
// Date: 16/4/1387
// Programmer: Pooya Shahinfar (Pswin), Ali Akbar Mohammadi (Kochol)

#include "../../Include/sn/Listener.h"
#include "../../Include/io/Logger.h"
#include "../../Libs/al/al.h"
#include "../../Libs/al/alut.h"
#include "../../Include/sn/Camera.h"

#include "../../Libs/mmgr/mmgr.h"



namespace kge
{
namespace sn
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Listener::Listener()
	{
		m_eNodeType		= ENT_Listener;

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Listener::~Listener()
	{
		if (m_pAbsMat)
			delete m_pAbsMat;
		if (m_pFinalMat)
			delete m_pFinalMat;

	} // Destructor

	//------------------------------------------------------------------------------------
	// Recalculate the sound listener position.
	//------------------------------------------------------------------------------------
	void Listener::PreRender(float elapsedTime)
	{
		if (m_pParent)
		{
			*m_pFinalMat = (*m_pParent->GetFinalMatrix()) * (*m_pAbsMat);
			if (m_pParent->GetType() == ENT_Camera)
			{
				if (((Camera*)m_pParent)->IsChanged())
				{
					SetOrientation(((Camera*)m_pParent)->GetUpDir(), ((Camera*)m_pParent)->GetTarget());

				} // if camera properties changed.

			} // if its parent is camera

		} // if has parent
		else
			*m_pFinalMat = *m_pAbsMat;

		SetPosition( m_pFinalMat->GetTranslation() );

	} // PreRender

	//------------------------------------------------------------------------------------
	// Set the position of the Listener.
	//------------------------------------------------------------------------------------
	void Listener::SetPosition(const math::Vector& pos)
	{
		SceneNode::SetPosition(pos);
		alListener3f(AL_POSITION,pos.x,pos.y,pos.z );
		if (alGetError() != AL_NO_ERROR) // checking for error
			kge::io::Logger::Log(io::ELM_Error , "Could not Set Listner Postion");	

	} // SetPosition

	//------------------------------------------------------------------------------------
	// Set the listener velocity.
	//------------------------------------------------------------------------------------
	void Listener::SetVelocity(const math::Vector& vel)
	{
		alListener3f(AL_VELOCITY,vel.x,vel.y,vel.z );
		if (alGetError() != AL_NO_ERROR) // checking for error
			kge::io::Logger::Log(io::ELM_Error , "Could not Set Listner Velocity");

	} // SetVelocity

	//------------------------------------------------------------------------------------
	// Get the listener velocity.
	//------------------------------------------------------------------------------------
	math::Vector Listener::GetVelocity()
	{
		kge::math::Vector vel;
		alGetListener3f(AL_VELOCITY,&vel.x,&vel.y,&vel.z);
		if (alGetError() != AL_NO_ERROR) // checking for error
			kge::io::Logger::Log(io::ELM_Error , "Could not Get Listner velocity");

		return vel;

	} // GetVelocity

	//------------------------------------------------------------------------------------
	// Set the Distance Model
	//------------------------------------------------------------------------------------
	void Listener::SetDistanceModel(av::DistanceGainType model)
	{
		switch(model)
		{
		case av::DGT_NO_ATTENUATION: alDistanceModel( AL_NONE ); break;
		case av::DGT_DEFAULT: alDistanceModel( AL_INVERSE_DISTANCE_CLAMPED ); break;
		case av::DGT_LINER: alDistanceModel( AL_LINEAR_DISTANCE_CLAMPED ); break;
		case av::DGT_INVERSE_CLAMPED: alDistanceModel( AL_INVERSE_DISTANCE_CLAMPED ); break;
		}
		if (alGetError() != AL_NO_ERROR) // checking for error
			kge::io::Logger::Log(io::ELM_Error , "Could not Set the Distance Model.");

	} // SetDistanceModel

	//------------------------------------------------------------------------------------
	// Set the listener orientation
	//------------------------------------------------------------------------------------
	void Listener::SetOrientation(const math::Vector& up, const math::Vector& at)
	{
		ALfloat ListenerOri[] = { up.x, up.y,up.z, at.x , at.y, at.z };
		alListenerfv(AL_ORIENTATION, ListenerOri);
		if (alGetError() != AL_NO_ERROR) // checking for error
			kge::io::Logger::Log(io::ELM_Error , "Could not Set the listener orientation.");

	} // SetOrientation

	//! Set the listener gain (default 1)
	void Listener::SetGain(float value)
	{
		alListenerf(AL_GAIN,value);
		if (alGetError() != AL_NO_ERROR) // checking for error
			kge::io::Logger::Log(io::ELM_Error , "Could not Set the listener gain.");

	} // SetGain

	//! Get the listener gain
	float Listener::GetGain()
	{
		float value;
		alGetListenerf(AL_GAIN,&value);

		return value;

	} // GetGain

} // sn

} // kge
