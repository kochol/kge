// File name: Animation.h
// Des: This class has information about skeletal animations.
// Date: 31/1/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef ANIMATION_H
#define ANIMATION_H

#include "Structs.h"
#include "../core/KgeMemory.h"

#include <algorithm>

namespace kge
{

namespace sn
{
 class SceneManager;
}
	
namespace gfx
{

int GetKey(float fTime, const kge::gfx::KeyFrame* p, int num);

struct AnimationData
{
	AnimationData():m_pRotKeys(NULL),
		m_pTransKeys(NULL) {}

	~AnimationData()
	{
		KGE_DELETE_ARRAY(m_pRotKeys);
		KGE_DELETE_ARRAY(m_pTransKeys);
	}

	int GetTransKey(float fTime)
	{
		return GetKey(fTime, m_pTransKeys, m_iNumTransKeys);
	}

	int GetRotateKey(float fTime)
	{
		return GetKey(fTime, m_pRotKeys, m_iNumRotKeys);
	}

	KeyFrame*	m_pRotKeys,			//! Rotation Key Frames.
			*	m_pTransKeys;		//! Translation Key Frames.

	u16			m_iNumTransKeys,	//! The count of Translation Key Frames.
				m_iNumRotKeys;		//! The count of Rotation Key Frames.

}; // AnimationData

class MeshBuffer;
class Renderer;

class Animation
{
	friend class MS3DLoader;
	friend MeshBuffer* LoadAnimatedMesh(const char* filename, Renderer* renderer, sn::SceneManager* sceneManager);
	friend void SaveAnimatedMesh(MeshBuffer* meshBuffer);

public:

	//! Constructor.
	Animation() : m_pAnis(0), m_fFPS(0.0f), m_fTotalTime(0.0f), m_iFramesCount(0), m_iAnimationCount(0) {}

	//! Destructor.
	~Animation();

	float GetTotalAnimationTime() { return m_fTotalTime; }

	AnimationData* GetAnimation(u32 index) { return &m_pAnis[index]; }

	float GetFPS() { return m_fFPS; }

protected:

	AnimationData*	m_pAnis;			//! The Animation Datas array.

	float			m_fFPS,				//! The frames per second for animation.
					m_fTotalTime;		//! The total animation time.

	int				m_iFramesCount;		//! The frames count.
	int				m_iAnimationCount;

}; // Animation

} // gfx

} // kge

#endif // ANIMATION_H