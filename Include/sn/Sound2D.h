// File name: Sound2D.h
// Des: interface for 2D sound
// Date: 15/12/1389 (jalali), Mar-06-2011
// Programmer: Pooya Shahinfar(Pswin)

#ifndef KGE_SOUND2D_H
#define KGE_SOUND2D_H

#include "../kgedef.h"
#include "SceneNode.h"
#include "../av/Structs.h"

namespace kge
{
	namespace sn
	{
		class Sound2D : public sn::SceneNode
		{
		public:
			//! Constructor
			Sound2D()
				: m_pan(0.0f), m_looping(false), m_volume(1.0f) {}

			// Destructor
			virtual ~Sound2D() {}

			//! Sets pan of sound in stereo speakers 
			virtual void SetPan(float pan) = 0 ;

			//! Returns pan
			virtual float GetPan() const { return m_pan; }

			//! Sets volume of the sound
			virtual void SetVolume(float vol) = 0;

			//! Returns volume of the sound
			virtual float GetVolume() const { return m_volume; }

			//! Sets lopping of the sound
			virtual void SetLoopingState(bool loop) = 0;

			//! Returns looping state of the sound
			virtual bool GetLoopingState() const { return m_looping; }

			//! sound source state
			virtual av::SourceStates GetState() const = 0; 

			//! sound source type (static,streaming...)
			virtual av::SourceType GetSoundType() const = 0;

			//! Plays sound 
			virtual void Play() = 0;

			//! Pause playing sound
			virtual void Pause() = 0;

			//! Stops playing sound
			virtual void Stop() = 0;

			//!
			virtual void SetFading(float value, unsigned int time) = 0;

		// Public members form scene manager
		public:
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

		protected:
			float	m_pan;
			float	m_volume;
			bool	m_looping;
		}; // Sound2D

	} // sn

} // kge

#endif // KGE_SOUND2D_H

