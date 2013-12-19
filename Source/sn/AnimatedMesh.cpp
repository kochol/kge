// File name: AnimatedMesh.cpp
// Des: This is a node class for animated meshes.
// Date: 31/1/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/sn/AnimatedMesh.h"
#include "../../Include/sn/Light.h"
#include "../../Headers/gfx/MeshBuffer.h"
#include "../../Include/io/Logger.h"
#include "../../Include/math/Quat.h"
#include "../../Include/math/Plane.h"
#include "../../Include/core/mem_fun.h"
#include "../../Include/gfx/Shader.h"
#include "../../Headers/gfx/GeometryBatch.h"



namespace kge
{
namespace sn
{
	float AnimatedMesh::m_sAnimSpeed = 1.0f;

	//-------------
	// Constructor.
	//-------------
	AnimatedMesh::AnimatedMesh(gfx::MeshBuffer* mb):
		m_pMeshes(mb->m_pMeshes),  m_pAnimation(mb->m_pAnimation) ,
		m_iNumMeshes(mb->m_iMeshCount) , m_iNumBones(mb->m_iBoneCount) ,
		m_fSpeed(1.0f) , m_pEndAniFunc(NULL), m_pUData(NULL) , 
		m_pVHSshader(NULL) , m_pPHSshader(NULL), m_pFileName(NULL),
		m_pMeshBuffer(mb), m_pShaderInstance(NULL), m_bBlend(true),
		m_fBlendTime(1.0f), m_fActionFrame(0.0f), m_bActionFrameReached(false),
		m_bCheckForActionFrame(false),
		m_shBones(0), m_shDiffColor(0), m_shDir(0), m_shViewProj(0), m_shWorld(0),
		m_pTLastFrameData(0), m_pTCurFrameData(0), m_pRLastFrameData(0), m_pRCurFrameData(0),
		m_pMaterials(0)
	{
		mb->AddRef();
		m_eNodeType  = ENT_AnimatedMesh;
		//m_pTimer     = new core::Timer();
		m_pBB		 = KGE_NEW(math::Sphere)();
		m_pAABB		 = KGE_NEW(math::AABB)();
		m_pMaterials = KGE_NEW_CLASS_ARRAY(gfx::Material, m_iNumMeshes);
		m_pBones     = KGE_NEW_CLASS_ARRAY(Bone, m_iNumBones);
		m_pTLastFrameData = KGE_NEW_ARRAY(u32, m_iNumBones);
		m_pTCurFrameData = KGE_NEW_ARRAY(u32, m_iNumBones);
		memset(m_pTCurFrameData, 0, m_iNumBones * sizeof(u32));
		memset(m_pTLastFrameData, 0, m_iNumBones * sizeof(u32));
		m_pRLastFrameData = KGE_NEW_ARRAY(u32, m_iNumBones);
		m_pRCurFrameData = KGE_NEW_ARRAY(u32, m_iNumBones);
		memset(m_pRCurFrameData, 0, m_iNumBones * sizeof(u32));
		memset(m_pRLastFrameData, 0, m_iNumBones * sizeof(u32));
//		memcpy(m_pBones, mb->m_pBones, sizeof(Bone) * m_iNumBones);
		if (m_pMeshBuffer->m_pVertexShader)
		{
			m_pVHSshader						= m_pMeshBuffer->m_pVertexShader;
			m_pPHSshader						= m_pMeshBuffer->m_pPixelShader;
			m_shBones							= m_pVHSshader->GetConstatnt("matBones");
			m_shViewProj						= m_pVHSshader->GetConstatnt("ViewProjMatrix");
			m_shDiffColor						= m_pPHSshader->GetConstatnt("DiffColor");
			m_shWorld							= m_pVHSshader->GetConstatnt("ViewMatrix");
			m_shDir								= m_pVHSshader->GetConstatnt("Dir");
			m_pShaderInstance					= KGE_NEW(gfx::ShaderInstance);
			m_pShaderInstance->m_pVertexShader	= m_pVHSshader;
			m_pShaderInstance->m_pPixelShader	= m_pPHSshader;
			m_pShaderInstance->m_pUserData		= this;
			m_pShaderInstance->m_pFun			= kge::core::mem_fun(this, &AnimatedMesh::SetShaderConstsForHS);
		}
		for (unsigned int i = 0; i < m_iNumMeshes; i++)
		{
			m_pAABB->AddAABB(m_pMeshes[i].GetAxisAlignedBoundingBox());
			m_pMaterials[i]					 = m_pMeshes[i].m_Mat;
			m_pMaterials[i].shader->m_pOwner = this;
		}
		for (unsigned int i = 0; i < m_iNumBones; i++)
		{
			m_pBones[i].m_sName			 = mb->m_pBones[i].m_sName;
			m_pBones[i].m_vStartPosition = mb->m_pBones[i].m_vStartPosition;
			m_pBones[i].m_vStartRotation = mb->m_pBones[i].m_vStartRotation;
			m_pBones[i].m_matLocal		 = mb->m_pBones[i].m_matLocal;
			m_pBones[i].m_matAbs		 = mb->m_pBones[i].m_matAbs;
			m_pBones[i].m_matFinal		 = mb->m_pBones[i].m_matFinal;
			m_pBones[i].m_iParent		 = mb->m_pBones[i].m_iParent;
			m_pBones[i].SetParent((SceneNode*)this);
		}
		m_Scale.set(1,1,1);
		m_Rotation.set(0,0,0);
		SetAnimationTime();

	} // Constructor

	//------------
	// Destructor.
	//------------
	AnimatedMesh::~AnimatedMesh()
	{
		KGE_DELETE(m_pAABB, AABB);
		KGE_DELETE(m_pBB, Sphere);
		//delete m_pTimer;
		KGE_DELETE(m_pShaderInstance, ShaderInstance);
		KGE_DELETE_CLASS_ARRAY(m_pMaterials, Material, m_iNumMeshes);

		if ( m_pMeshes )
		{
			//delete[] m_pMeshes;
			//m_pMeshes = NULL;
		}
		if ( m_pBones )
		{
			for (u32 i = 0; i < m_iNumBones; i++)
			{
				m_pSnMan->RemoveSceneNode(&m_pBones[i]);
			}
			KGE_DELETE_CLASS_ARRAY(m_pBones, Bone, m_iNumBones);
		}

		KGE_DELETE_ARRAY(m_pTLastFrameData);
		KGE_DELETE_ARRAY(m_pTCurFrameData);
		KGE_DELETE_ARRAY(m_pRLastFrameData);
		KGE_DELETE_ARRAY(m_pRCurFrameData);

		m_pMeshBuffer->DecRef();

	} // Destructor

	//-------------------------------------------------------------------------
	// Karhayee ke Ghabl az render bayad anjam shvad. Mesle colision detection.
	//-------------------------------------------------------------------------
	void AnimatedMesh::PreRender(float elapsedTime)
	{
		if (m_pParent)
		{
			*m_pFinalMat = (*m_pParent->GetFinalMatrix()) * (*m_pAbsMat);
		}
		else
		{
			*m_pFinalMat = *m_pAbsMat;
		}

		if (m_bVis)
		{
			if ( !m_bAutoCulling || m_pSnMan->IsVisible(this) == math::ECT_In )
			{
				if (m_pMeshBuffer->m_bInstanced)
				{
					m_bVisFrame = true;
					m_pMeshBuffer->m_pBatch->AddInstance(this);
				}
				else
				{
					m_pSnMan->RegNode(this);
				}
			}
			if ( m_iNumBones > 0)
			{
				Animate(elapsedTime);
			}
		}

	} // PreRender

	//-----------------------
	// Render kardane Object.
	//-----------------------
	void AnimatedMesh::Render()
	{	
		m_pRenderer->SetTransForm(m_pFinalMat);
		if ( m_iNumBones > 0)
		{
			if ( m_pMaterials[0].shader->m_MaterialParams.BoneCount > 0 )
			{
				m_pMeshBuffer->m_bShaderSet = false;
				for ( u32 x = 0 ; x < m_iNumMeshes; x++ )
				{
					m_pRenderer->SetMaterial(&m_pMaterials[x]);
					m_pMeshes[x].Render();
				}
			}
			else
				RenderMesh();
		}
		else
		{
			for ( u32 x = 0 ; x < m_iNumMeshes; x++ )
			{
				m_pRenderer->SetMaterial(&m_pMaterials[x]);
				m_pMeshes[x].Render();
			}
		}

	} // Render

	//--------------------------------------------
	// Karhaee ke bad az render bayad anjam beshe.
	//--------------------------------------------
	void AnimatedMesh::PostRender()
	{
	} // PostRender

	//-----------------------------
	// Set the mesh animation time.
	//-----------------------------
	void AnimatedMesh::SetAnimationTime(float StartTime ,
										float EndTime   ,
										bool  Loop	    ,
										bool  Blend     )
	{
		if (!m_pAnimation)
			return;

		if (Blend && !m_bBlend && m_fStartTime != StartTime && m_fEndTime != EndTime)
		{
			m_bBlend = true;
			m_fBlendTime = 0.0f;
			// find the start time animations
			for (u32 x = 0; x < m_iNumBones; x++)
			{
				// Current Animation Data
				gfx::AnimationData* pAni = m_pAnimation->GetAnimation(x);

				// if there are no key frames, don't do any transformations
				if (pAni->m_iNumRotKeys == 0 && pAni->m_iNumTransKeys == 0)
				{
					continue;
				}
				
				// Current frame
				u32 uiFrame = 0;
				// Calculate the current frame
				// Translation
				while (uiFrame < pAni->m_iNumTransKeys && pAni->m_pTransKeys[uiFrame].m_fTime < StartTime)
				{
					uiFrame++;
				}

				// If its at the extremes
				if (uiFrame == 0)
				{
					m_pTCurFrameData[x] = 0;
				}
				else
				{
					m_pTCurFrameData[x] = uiFrame-1;
				}

				// Calculate the current rotation
				uiFrame = 0;
				while (uiFrame < pAni->m_iNumRotKeys && pAni->m_pRotKeys[uiFrame].m_fTime < StartTime)
				{
					uiFrame++;
				}

				// If its at the extremes
				if (uiFrame == 0)
				{
					m_pRCurFrameData[x] = 0;
				}
				else
				{
					m_pRCurFrameData[x] = uiFrame-1;
				}		
			}
		}
		else
		{
			m_bBlend = false;
		}
		m_fStartTime = m_fLastTime = StartTime;
		m_fEndTime   = EndTime;
		m_bLoop		 = Loop;
		if ( EndTime < 0.0f )
		{
			m_fEndTime = m_pAnimation->GetTotalAnimationTime();
		}

		// Reset the timer too.
		//m_pTimer->GetSeconds();
	
	} // SetAnimationTime

	//------------------------------
	// Set the mesh animation frame.
	//------------------------------
	void AnimatedMesh::SetAnimationFrame(int  StartTime ,
										 int  EndTime   ,
										 bool Loop	    ,
										 bool Blend     )
	{
		float s = (float)StartTime / m_pAnimation->GetFPS();
		float e = (float)EndTime  / m_pAnimation->GetFPS();

		SetAnimationTime(s, e, Loop, Blend);

	} // SetAnimationFrame

	//------------------------------------------------------------------------------------
	// Sets the action frame
	//------------------------------------------------------------------------------------
	void AnimatedMesh::SetActionFrame( int ActionFrame )
	{
		m_bCheckForActionFrame = true;
		m_bActionFrameReached = false;
		m_fActionFrame = (float)ActionFrame / m_pAnimation->GetFPS();

	} // SetActionFrame

	//-------------------
	// Animates the mesh.
	//-------------------
	void AnimatedMesh::Animate(float elapsedTime)
	{
		bool Loop = false;
		float fs = elapsedTime;//m_pTimer->GetSeconds();
		float fTime = fs * m_fSpeed * m_sAnimSpeed;
		m_fBlendTime += fs;

		if ( m_fEndTime < m_fStartTime )
		{
			fTime = m_fLastTime - fTime;
			// Check for action frame
			if (m_bCheckForActionFrame)
			{
				if (m_fActionFrame >= fTime)
				{
					m_bCheckForActionFrame = false;
					m_bActionFrameReached = true;
				}
			}
			if ( fTime < m_fEndTime )
			{
				Loop = true;
				if ( m_pEndAniFunc )
				{
					m_pEndAniFunc->Call(m_pUData);
				}
			}
		}
		else
		{
			fTime += m_fLastTime;
			// Check for action frame
			if (m_bCheckForActionFrame)
			{
				if (m_fActionFrame <= fTime)
				{
					m_bCheckForActionFrame = false;
					m_bActionFrameReached = true;
				}
			}
			if ( fTime > m_fEndTime )
			{
				Loop = true;
				if ( m_pEndAniFunc )
				{
					m_pEndAniFunc->Call(m_pUData);
				}
			}
		}
		m_fLastTime = fTime;


		// looping
		if ( Loop )
		{
			if (m_bLoop)
			{
				//m_pTimer->GetSeconds();
				m_fLastTime = m_fStartTime;
				fTime		= m_fStartTime;
			}
			else
			{
				fTime = m_fEndTime;
			}
		}

		if (!IsVisibleInFrame())
		{
			return;
		}

		if (m_bBlend)
		{
			if (m_fBlendTime > 0.2f)
			{
				m_bBlend = false;
				m_fLastTime = m_fStartTime;
				return;
			}
			for (u32 x = 0; x < m_iNumBones; x++)
			{
				// Transformation matrix
				math::Matrix matTmp;
				// Current joint
				Bone * pJoint = &m_pBones[x];
				// Current Animation Data
				gfx::AnimationData* pAni = m_pAnimation->GetAnimation(x);

				// if there are no key frames, don't do any transformations
				if (pAni->m_iNumRotKeys == 0 && pAni->m_iNumTransKeys == 0)
				{
					continue;
				}

				float fTranslation[3];
				//float fDeltaT = 1.0f;
				float fInterp = 0.0f;
				gfx::KeyFrame * pkCur = &pAni->m_pTransKeys[m_pTCurFrameData[x]];
				gfx::KeyFrame * pkPrev = &pAni->m_pTransKeys[m_pTLastFrameData[x]];

				fInterp = m_fBlendTime * 5.0f;

				//Interpolate between the translations
				fTranslation[0] = pkPrev->m_fParam[0] + (pkCur->m_fParam[0] - pkPrev->m_fParam[0]) * fInterp;
				fTranslation[1] = pkPrev->m_fParam[1] + (pkCur->m_fParam[1] - pkPrev->m_fParam[1]) * fInterp;
				fTranslation[2] = pkPrev->m_fParam[2] + (pkCur->m_fParam[2] - pkPrev->m_fParam[2]) * fInterp;

				pkCur = &pAni->m_pRotKeys[m_pRCurFrameData[x]];
				pkPrev = &pAni->m_pRotKeys[m_pRLastFrameData[x]];

				// Create a rotation quaternion for each frame
				math::Quat qCur;
				math::Quat qPrev;
				qCur.FromEulers(pkCur->m_fParam);
				qPrev.FromEulers(pkPrev->m_fParam);
				// SLERP between the two frames
				math::Quat qFinal = math::Quat::SLERP(qPrev, qCur, fInterp);

				// Convert the quaternion to a rotation matrix
				matTmp = qFinal.ToMatrix();

				// Set the translation part of the matrix
				matTmp.SetTranslation(fTranslation);

				// Calculate the joints final transformation
				math::Matrix matFinal = pJoint->m_matLocal * matTmp;

				// if there is no parent, just use the matrix you just made
				if(pJoint->m_iParent == -1)
				{
					pJoint->m_matFinal = matFinal;
				}
				// otherwise the final matrix is the parents final matrix * the new matrix
				else
				{
					pJoint->m_matFinal = m_pBones[pJoint->m_iParent].m_matFinal * matFinal;
				}
			}

			return;
		
		}

		for (u32 x = 0; x < m_iNumBones; x++)
		{
			// Transformation matrix
			math::Matrix matTmp;
			// Current joint
			Bone * pJoint = &m_pBones[x];
			// Current Animation Data
			gfx::AnimationData* pAni = m_pAnimation->GetAnimation(x);

			// if there are no key frames, don't do any transformations
			if (pAni->m_iNumRotKeys == 0 && pAni->m_iNumTransKeys == 0)
			{
				pJoint->m_matFinal = pJoint->m_matAbs;
				continue;
			}
			
			// Current frame
			u32 uiFrame = 0;

			// Calculate the current frame
			// Translation
// 			while (uiFrame < pAni->m_iNumTransKeys && 
// 				pAni->m_pTransKeys[uiFrame].m_fTime < fTime)
// 				++uiFrame;
			uiFrame = pAni->GetTransKey(fTime);

			float fTranslation[3];
			float fDeltaT = 1.0f;
			float fInterp = 0.0f;

			// If its at the extremes
			if (uiFrame == 0)
			{
				memcpy(fTranslation, pAni->m_pTransKeys[0].m_fParam, sizeof(float[3]));
				m_pTLastFrameData[x] = 0;
			}
			else if (uiFrame == pAni->m_iNumTransKeys)
			{
				memcpy(fTranslation, pAni->m_pTransKeys[uiFrame-1].m_fParam, sizeof(float[3]));
				m_pTLastFrameData[x] = uiFrame-1;
			}
			//If its in the middle of two frames
			else
			{
				m_pTLastFrameData[x] = uiFrame;
				gfx::KeyFrame * pkCur = &pAni->m_pTransKeys[uiFrame];
				gfx::KeyFrame * pkPrev = &pAni->m_pTransKeys[uiFrame-1];

				fDeltaT = pkCur->m_fTime - pkPrev->m_fTime;
				fInterp = (fTime - pkPrev->m_fTime) / fDeltaT;

				//Interpolate between the translations
				fTranslation[0] = pkPrev->m_fParam[0] + (pkCur->m_fParam[0] - pkPrev->m_fParam[0]) * fInterp;
				fTranslation[1] = pkPrev->m_fParam[1] + (pkCur->m_fParam[1] - pkPrev->m_fParam[1]) * fInterp;
				fTranslation[2] = pkPrev->m_fParam[2] + (pkCur->m_fParam[2] - pkPrev->m_fParam[2]) * fInterp;
			}

			// Calculate the current rotation
// 			uiFrame = 0;
// 			while (uiFrame < pAni->m_iNumRotKeys && pAni->m_pRotKeys[uiFrame].m_fTime < fTime)
// 				++uiFrame;
			uiFrame = pAni->GetRotateKey(fTime);

			// If its at the extremes
			if (uiFrame == 0)
			{
				matTmp.SetRotation(pAni->m_pRotKeys[0].m_fParam);
				m_pRLastFrameData[x] = 0;
			}
			else if (uiFrame == pAni->m_iNumRotKeys)
			{
				matTmp.SetRotation(pAni->m_pRotKeys[uiFrame-1].m_fParam);
				m_pRLastFrameData[x] = uiFrame-1;
			}
			// If its in the middle of two frames, use a quaternion SLERP operation to calculate a new position
			else
			{
				m_pRLastFrameData[x] = uiFrame;
				gfx::KeyFrame * pkCur = &pAni->m_pRotKeys[uiFrame];
				gfx::KeyFrame * pkPrev = &pAni->m_pRotKeys[uiFrame-1];

				fDeltaT = pkCur->m_fTime - pkPrev->m_fTime;
				fInterp = (fTime - pkPrev->m_fTime) / fDeltaT;
			
				// Create a rotation quaternion for each frame
				math::Quat qCur;
				math::Quat qPrev;
				qCur.FromEulers(pkCur->m_fParam);
				qPrev.FromEulers(pkPrev->m_fParam);
				// SLERP between the two frames
				math::Quat qFinal = math::Quat::SLERP(qPrev, qCur, fInterp);

				// Convert the quaternion to a rotation matrix
				matTmp = qFinal.ToMatrix();
			}

			// Set the translation part of the matrix
			matTmp.SetTranslation(fTranslation);

			// Calculate the joints final transformation
			math::Matrix matFinal = pJoint->m_matLocal * matTmp;

			// if there is no parent, just use the matrix you just made
			if(pJoint->m_iParent == -1)
			{
				pJoint->m_matFinal = matFinal;
			}
			// otherwise the final matrix is the parents final matrix * the new matrix
			else
			{
				pJoint->m_matFinal = m_pBones[pJoint->m_iParent].m_matFinal * matFinal;
			}
		}

	} // Animate

	//----------------------------------------
	// Renders the mesh depend on their Bones.
	//----------------------------------------
	void AnimatedMesh::RenderMesh()
	{
		for (u32 x = 0; x < m_iNumMeshes; x++)
		{
			gfx::Vertex3*	TempPos = KGE_NEW_ARRAY(gfx::Vertex3, m_pMeshes[x].m_NumVerts);
			gfx::Vertex3T*  TempNorTex = KGE_NEW_ARRAY(gfx::Vertex3T, m_pMeshes[x].m_NumVerts);
			memcpy(TempPos, m_pMeshes[x].m_pPos, m_pMeshes[x].m_NumVerts*sizeof(gfx::Vertex3));
			memcpy(TempNorTex, m_pMeshes[x].m_pNorTex, m_pMeshes[x].m_NumVerts*sizeof(gfx::Vertex3T));
			for (u32 z = 0; z < m_pMeshes[x].m_NumVerts; z++)
			{
				if (m_pMeshes[x].m_Bone[z] == -1)
				{
					continue;
				}

				math::Matrix * mat = &m_pBones[m_pMeshes[x].m_Bone[z]].m_matFinal;

				TempPos[z].Transform4(mat);
				TempNorTex[z].pos.Transform3(mat);

			} // for (Vertices)

			m_pRenderer->SetMaterial(&m_pMaterials[x]);
			if (!m_pMeshes[x].m_pvbPosition->SetData(TempPos, 0, m_pMeshes[x].m_NumVerts))
			{
				m_pMeshes[x].PrepareBuffers(false, true);
				m_pMeshes[x].m_pvbPosition->SetData(TempPos, 0, m_pMeshes[x].m_NumVerts);
			}
			m_pMeshes[x].m_pvbNormalTexcoord->SetData(TempNorTex, 0, m_pMeshes[x].m_NumVerts);
			m_pMeshes[x].Render();
//			m_pRenderer->DrawTriangleList(Temp, m_pMeshes[x].m_NumVerts, m_pMeshes[x].m_pIndices , m_pMeshes[x].m_NumIndices);
			
			KGE_DELETE_ARRAY(TempPos);
			KGE_DELETE_ARRAY(TempNorTex);

		} // for (Meshes)

	} // Render Mesh

	bool AnimatedMesh::AddEffect( efx::EffectType p, efx::Effect** ppOut)
	{
		if ( this->HasThisEffect(p) )
		{
			return true;
		}
	
		switch ( p )
		{
			// Shadow Plane
			case efx::EET_ShadowPlane:
				CreateShadowPlane( ppOut );
				break;
			
			// Mirror Plane
			case efx::EET_MirrorPlane:
				CreateMirrorPlane( ppOut );
				break;
	
			// Hardware Skinning.
			case efx::EET_HardWareSkinning:
				CreateHardwareSkin();
				m_vEffectType.push_back(p);
				return true;

			// Default
			default:
				return false;
		} // switch ( p )
	
		m_vEffectType.push_back(p);
		m_pSnMan->AddEffect(*ppOut, p);

		return true;
	} // AddEffect

	//---------------------
	// Create Mirror Plane.
	//---------------------
	void AnimatedMesh::CreateMirrorPlane( efx::Effect** ppCreatedMirror )
	{
		math::Vector v0 = m_pMeshes[0].m_pVerts[0].pos.ToVector();
		math::Vector v1 = m_pMeshes[0].m_pVerts[1].pos.ToVector();
		math::Vector v2 = m_pMeshes[0].m_pVerts[2].pos.ToVector();

		math::Plane MeshPlane;
		MeshPlane.Set( v0 , v1 , v2 );

		*ppCreatedMirror = new efx::MirrorPlane( m_pRenderer , this );
		((efx::MirrorPlane*)(*ppCreatedMirror))->Init( MeshPlane.GetReflectMatrix() );

		m_vEffect.push_back(*ppCreatedMirror);

	} // CreateMirrorPlane

	//---------------------
	// Create Shadow Plane.
	//---------------------
	void AnimatedMesh::CreateShadowPlane( efx::Effect** ppCreatedShadow )
	{
		sn::Light* lit = m_pSnMan->GetNearestLight(this->GetPosition());
		if (!lit)
		{
			io::Logger::Log("Can't create the shadow plane. No light found.", io::ELM_Warning);
			return;
		}
		math::Vector v;
		if (lit->GetLightData()->Type == gfx::ELT_Directional)
		{
			v   = lit->GetLightData()->Direction;
			v.y *= -1;
			v.w = 0.0f;
		}
		else
		{
			v   = lit->GetLightData()->Position;
			v.w = 1.0f;
		}
		math::Vector v0 = m_pMeshes[0].m_pVerts[0].pos.ToVector();
		math::Vector v1 = m_pMeshes[0].m_pVerts[1].pos.ToVector();
		math::Vector v2 = m_pMeshes[0].m_pVerts[2].pos.ToVector();

		math::Plane MeshPlane;
		MeshPlane.Set( 0 , 1 ,0,0 );

		*ppCreatedShadow = new kge::efx::ShadowPlane( m_pRenderer , this );
		((efx::ShadowPlane*)(*ppCreatedShadow))->Init( MeshPlane.GetShadowMatrix(v) );

		m_vEffect.push_back(*ppCreatedShadow);
	} // CreateShadowPlane

	//---------------------------------------------------------
	// Draw the node with/without its Materials and Transforms.
	//---------------------------------------------------------
	void AnimatedMesh::Draw( bool WithMaterial , bool WithTransform , bool bPosition , bool bNormalTexcoord , bool bTangentBinormal )
	{
		if ( WithTransform )
			m_pRenderer->SetTransForm(m_pFinalMat);
		if (   m_pMaterials[0].shader->m_MaterialParams.BoneCount > 0 
			&& m_pMaterials[0].shader->m_pVertexShader)
		{
			for ( u32 x = 0 ; x < m_iNumMeshes; x++ )
			{
				m_pMeshBuffer->m_bShaderSet = false;
				if (WithMaterial)
					m_pRenderer->SetMaterial(&m_pMaterials[x]);
				else
				{
					if (!WithTransform)
						m_pRenderer->SetTransForm(m_pAbsMat);
					m_pMaterials[x].shader->m_pFun->Call(m_pMaterials[x].shader);
					m_pMaterials[x].shader->m_pVertexShader->PreRender();
					m_pMaterials[x].shader->m_pVertexShader->Render();
				}
				m_pMeshes[x].Render(bPosition, bNormalTexcoord, bTangentBinormal);
			}
			return;
		}

		if ( m_iNumBones > 0)
		{
			for (u32 x = 0; x < m_iNumMeshes; x++)
			{
				if ( WithMaterial )
					m_pRenderer->SetMaterial(&m_pMeshes[x].m_Mat);
				m_pMeshes[x].Draw(WithMaterial, WithTransform, bPosition, bNormalTexcoord, bTangentBinormal);
					

			} // for (Meshes)
		}
		else
		{
			if ( WithTransform )
				m_pRenderer->SetTransForm(m_pAbsMat);
			for ( u32 x = 0 ; x < m_iNumMeshes; x++ )
			{
				if ( WithMaterial )
					m_pRenderer->SetMaterial(&m_pMeshes[x].m_Mat);
				m_pMeshes[x].Draw(WithMaterial, WithTransform, bPosition, bNormalTexcoord, bTangentBinormal);
			}
		}
	} // Draw

	//------------------------------
	//
	//------------------------------
	void AnimatedMesh::SetShader(gfx::ShaderInstance* pSh)
	{
		for ( u32 x = 0 ; x < m_iNumMeshes; x++ )
		{
			m_pMeshes[x].m_Mat.shader = pSh;
		}

	} // SetShader

	//------------------------------------------
	// Get the count of materials this node has.
	//------------------------------------------
	u32 AnimatedMesh::GetMaterialCount()
	{
		return m_iNumMeshes;

	} // GetMaterialCount

	//------------------------------------
	// Get the node material by its index.
	//------------------------------------
	gfx::Material* AnimatedMesh::GetMaterial(u32 Index)
	{
		return &m_pMaterials[Index];

	} // GetMaterial

	//------------------------
	// Get a bone by its name.
	//------------------------
	Bone* AnimatedMesh::GetBoneByName(const char* Name)
	{
		for ( u32 i = 0; i < m_iNumBones; i++ )
		{
			if ( m_pBones[i].m_sName == Name )
				return &m_pBones[i];

		} // for i

		io::Logger::Log(io::ELM_Error, "The %s bone not found.", Name);
		
		return NULL;

	} // GetBoneByName

	//--------------------------------------------------------------
	// Get a pointer to function to call it at the end of animation.
	//--------------------------------------------------------------
	void AnimatedMesh::ConnectOnAnimationEnd(kge::core::Functor1<void, void*>* pFun)
	{
		m_pEndAniFunc = pFun;

	} // ConnectOnAnimationEnd

	//---------------------------------
	// Stop from calling user function.
	//---------------------------------
	void AnimatedMesh::DisconnectOnAnimationEnd()
	{
		m_pEndAniFunc = NULL;

	} // DisconnectOnAnimationEnd

	//---------------------------------
	// Create hardware skinning effect.
	//---------------------------------
	void AnimatedMesh::CreateHardwareSkin()
	{
		if (m_pMaterials[0].shader->m_MaterialParams.BoneCount == m_iNumBones)
			return;

		for (u32 i = 0; i < m_iNumMeshes; i++)
		{
			m_pMeshes[i].m_Mat.shader->m_MaterialParams.BoneCount	= m_iNumBones;
			m_pMaterials[i].shader->m_MaterialParams.BoneCount		= m_iNumBones;
			m_pMaterials[i].shader->m_bMatParamsChanged				= true;
		}

		if (m_pMeshes[0].m_pVertexDec  != m_pRenderer->GetVertexDec(gfx::EVT_PJ0NT1BT2I3))
		{
			for (u32 x = 0; x < m_iNumMeshes; x++)
			{
				gfx::Vertex3I* Temp = new gfx::Vertex3I[m_pMeshes[x].m_NumVerts];
				for (u32 z = 0; z < m_pMeshes[x].m_NumVerts; z++)
				{
					Temp[z].pos    = m_pMeshes[x].m_pPos[z];
					Temp[z].BoneID = m_pMeshes[x].m_Bone[z];

				} // for (Vertices)

				m_pMeshes[x].m_pvbPosition->DecRef();
				m_pMeshes[x].m_pvbPosition = m_pRenderer->CreateVertexBuffer(Temp, m_pMeshes[x].m_NumVerts, sizeof(gfx::Vertex3I));
				m_pMeshes[x].m_pVertexDec  = m_pRenderer->GetVertexDec(gfx::EVT_PJ0NT1BT2I3);

				delete[] Temp;

			} // for (Meshes)

		}
		
		return;

		if (m_pMeshBuffer->m_pVertexShader)
		{
			m_pVHSshader						= m_pMeshBuffer->m_pVertexShader;
			m_pPHSshader						= m_pMeshBuffer->m_pPixelShader;
			m_shBones							= m_pVHSshader->GetConstatnt("matBones");
			m_shWorld							= m_pVHSshader->GetConstatnt("ViewMatrix");
			m_shDir								= m_pVHSshader->GetConstatnt("Dir");
			m_shViewProj						= m_pVHSshader->GetConstatnt("ViewProjMatrix");
			m_shDiffColor						= m_pPHSshader->GetConstatnt("DiffColor");
			m_pShaderInstance					= KGE_NEW(gfx::ShaderInstance)();
			m_pShaderInstance->m_pVertexShader	= m_pVHSshader;
			m_pShaderInstance->m_pPixelShader	= m_pPHSshader;
			m_pShaderInstance->m_pUserData		= this;
			m_pShaderInstance->m_pFun			= kge::core::mem_fun(this, &AnimatedMesh::SetShaderConstsForHS);
			return;
		}
		if ( m_iNumBones > 0)
		{
			const char VS[] = 
				"matrix ViewMatrix;\n"\
				"matrix ViewProjMatrix;\n"\
				"float3 Dir;\n"\
				"matrix matBones[";
			core::String str;
			str = VS;
			str += (int)m_iNumBones;
			const char VS2[] = 
				"];\n"\
				"struct VS_INPUT\n"\
				"{\n"\
				"    float4 position : POSITION;\n"\
				"    int    index	 : COLOR;\n"\
				"    float3 normal   : NORMAL;\n"\
				"    float2 base 	 : TEXCOORD0;\n"\
				"};\n"\
				"struct VS_OUTPUT\n"\
				"{\n"\
				"    vector position : POSITION;\n"\
				"    float2 uv : TEXCOORD;\n"\
				"	 float fNdotL : TEXCOORD1;\n"\
				"};\n"\
				"VS_OUTPUT VSMain(VS_INPUT input)\n"\
				"{\n"\
				"	VS_OUTPUT o = (VS_OUTPUT)0;\n"\
				"	o.position  = mul(input.position, mul(matBones[(int)input.index], ViewProjMatrix) );\n"\
				"	o.uv        = input.base;\n"\
				"   float3 Normal = mul(float4(input.normal, 0), mul(matBones[(int)input.index], ViewMatrix) ).xyz;\n"\
				"	o.fNdotL    = max(0.2, dot(Normal, Dir)) + 0.2;\n"\
				"	return o;\n"\
				"}\n";

			str += VS2; 
			const char PS[] = "sampler2D baseMap;\n"\
				"float4 DiffColor;\n"\
				"float4 PSMain(float2 texcoo : TEXCOORD0, float fNdotL : TEXCOORD1) : COLOR0\n"\
				"{\n"\
				"	float4 v = tex2D( baseMap, texcoo );\n"\
				"	float  i = 1.0 - v.a;\n"\
				"	float4 c = float4(v.rgb * i + (DiffColor.rgb * v.a), 1.0);\n"\
				"	c *= fNdotL;\n"\
				"	c.a = DiffColor.a;\n"\
				"	return c;\n"\
				"}";
			m_pVHSshader = m_pRenderer->CreateVertexShaderFromString(str.ToCharPointer(),"VSMain", gfx::ESV_VS2);
			if (!m_pVHSshader)
				return;
			m_pPHSshader = m_pRenderer->CreatePixelShaderFromString(PS,"PSMain", gfx::ESV_PS2);
			m_shBones = m_pVHSshader->GetConstatnt("matBones");
			m_shViewProj = m_pVHSshader->GetConstatnt("ViewProjMatrix");
			m_shWorld							= m_pVHSshader->GetConstatnt("ViewMatrix");
			m_shDir								= m_pVHSshader->GetConstatnt("Dir");
			m_shDiffColor						= m_pPHSshader->GetConstatnt("DiffColor");
			m_pMeshBuffer->m_pVertexShader		= m_pVHSshader;
			m_pMeshBuffer->m_pPixelShader		= m_pPHSshader;
			m_pShaderInstance					= KGE_NEW(gfx::ShaderInstance)();
			m_pShaderInstance->m_pVertexShader	= m_pVHSshader;
			m_pShaderInstance->m_pPixelShader	= m_pPHSshader;
			m_pShaderInstance->m_pUserData		= this;
			m_pShaderInstance->m_pFun			= kge::core::mem_fun(this, &AnimatedMesh::SetShaderConstsForHS);

			for (u32 x = 0; x < m_iNumMeshes; x++)
			{
				m_pMaterials[x].shader = m_pShaderInstance;
			}
		}

	} // CreateHardwareSkin

	//-----------------------------------------------
	// Set the shader variables for HardwareSkinning.
	//-----------------------------------------------
	void AnimatedMesh::SetShaderConstsForHS(gfx::ShaderInstance* si)
	{
		if (m_pMeshBuffer->m_bShaderSet)
			return;
		m_pMeshBuffer->m_bShaderSet = true;
		math::Matrix* mat = KGE_NEW_CLASS_ARRAY(math::Matrix, m_iNumBones);
		kge::gfx::Colorf diff = m_pRenderer->GetLastMaterial()->Diffuse;
		for ( u32 i = 0; i < m_iNumBones; i++ )
		{
			mat[i] = m_pBones[i].m_matFinal;
		}
		m_pVHSshader->SetConstant(m_shBones, &mat[0].m_fMat[0], m_iNumBones * 16);
		kge::math::Matrix vm, vpm, wm;
		vm   = m_pRenderer->GetTransForm(kge::gfx::ETM_View);
		vpm  = m_pRenderer->GetTransForm(kge::gfx::ETM_Projection);
		wm   = *m_pFinalMat;
		vpm *= vm;
		vpm *= wm;
		m_pVHSshader->SetConstant(m_shViewProj, vpm.m_fMat, 16);
		m_pPHSshader->SetConstant(m_shDiffColor, diff.c, 4);
		wm.Identity();
		wm.SetTranslation(&m_Position.x);
		wm.SetRotation(&m_Rotation.x);
		m_pVHSshader->SetConstant(m_shWorld, wm.m_fMat, 16);
		m_pVHSshader->SetConstant(m_shDir, &m_pSnMan->GetNearestLight(m_Position)->GetLightData()->Direction.x, 3);

		delete[] mat;

	} // SetShaderConstsForHS


	void AnimatedMesh::RemoveChildrenFromBones()
	{
		if ( m_pBones )
		{
			for (u32 i = 0; i < m_iNumBones; i++)
			{
				m_pBones[i].RemoveChildren();
			}
		}
	}

} // sn

} // kge
