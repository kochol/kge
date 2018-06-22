// File name: Sound3D.h
// Des: interface for 3D sound sources
// Date: 6/12/1389 (jalali), Feb-26-2011
// Programmers: Pooya Shahinfar (Pswin)

#ifndef KGE_SOUND3D_H
#define KGE_SOUND3D_H

#include "../kgedef.h"
#include "SceneNode.h"
#include "../av/Structs.h"

namespace kge
{
namespace sn
{
//! 3D Sound node.
class KGE_API Sound3D : public SceneNode
{
public:

	//! Constructor
	Sound3D()
		: m_velocity(0.0f, 0.0f, 0.0f), m_volume(1.0f), m_looping(false)
	{
	}

	//! Destructor
	virtual ~Sound3D() {}

	//! Recalculate the sound position.
	virtual void PreRender(float elapsedTime = 0.0f) { /* Nothing to do */ }

	//! Nothing to do.
	virtual void Render() { /* Nothing to do */ }

	//! Nothing to do.
	virtual void PostRender() { /* Nothing to do */ }

	//! Nothing to do 
	virtual bool AddEffect ( efx::EffectType p, efx::Effect** ppOut ) { return false; }

	//! Nothing to do
	virtual void Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal ) { /* Nothing to do */ }

	//! Set the position of the sound.
	virtual void SetPosition(const math::Vector& v) = 0;

	//! Set sound source velocity
	virtual void SetVelocity(const math::Vector& velocity) = 0;

	//! Returns sound source velocity
	virtual math::Vector GetVelocity() const { return m_velocity; }

	//! Set the sound source volume 
	virtual void SetVolume (float value) = 0;

	//! Get the sound source volume
	virtual float GetVolume () const { return m_volume; }

	//! Sets looping of sound
	virtual void SetLoopingState(bool looping) = 0;

	//! Returns Sound lopping state
	virtual bool GetLoopingState() const { return m_looping; }

	//! sound source state
	virtual av::SourceStates GetState() const = 0; 

	//! sound source type (static,streaming...)
	virtual av::SourceType GetSoundType() const = 0;

	//! play source (sound)
	virtual void Play() = 0;

	//! pause playing source (sound)
	virtual void Pause() = 0;

	//! stop playing source (sound)
	virtual void Stop() =0;

	//! set max and min distance
	virtual void SetMinMaxDistance(float _min, float _max = 10000.0f) = 0;

	//!
	virtual void SetFading(float value, unsigned int time) = 0;

protected:
	math::Vector	m_velocity;
	float			m_volume;
	bool			m_looping;
	//u32 Source;

}; // Sound3D

} // sn

} // kge

#endif // KGE_SOUND3D_H

