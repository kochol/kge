#include "../../include/kge/en/SystemMesh.h"
#include "../../include/kge/en/CmpMesh.h"
#include "../../include/kge/en/CmpPosition.h"
#include "../../include/kge/en/Entity.h"
#include "../../include/kge/en/ComponentType.h"
#include "../../include/kge/Device.h"
#include "../../include/kge/sn/StaticMesh.h"

namespace kge
{
namespace en
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	SystemMesh::SystemMesh()
	{
		m_iComMeshID = ComponentType<CmpMesh>::GetID();
		m_iComPosID	 = ComponentType<CmpPosition>::GetID();

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	SystemMesh::~SystemMesh()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// Updates the system and it will update the entities.
	//------------------------------------------------------------------------------------
	void SystemMesh::Update( float fElaspedTime )
	{
		CmpMesh		* pComMesh;
		CmpPosition	* pComPos;
		for (int i = 0; i < m_vEntities.size(); i++)
		{
			pComMesh = (CmpMesh*)m_vEntities[i]->GetComponent(m_iComMeshID);
			if (pComMesh->m_pStaticMesh)
			{
				pComPos	 = (CmpPosition*)m_vEntities[i]->GetComponent(m_iComPosID);
				pComMesh->m_pStaticMesh->SetPosition(math::Vector(pComPos->x, pComPos->y, pComPos->z));
			}
		}

	} // Update

	//------------------------------------------------------------------------------------
	// When a new entity created or new component attached to entity.
	//------------------------------------------------------------------------------------
	void SystemMesh::NotifySystem( Entity* pEn )
	{
		CmpMesh		* pCom		= (CmpMesh*)pEn->GetComponent(m_iComMeshID);
		CmpPosition	* pComPos	= (CmpPosition*)pEn->GetComponent(m_iComPosID);
		if (pCom && pComPos)
		{
			AddEntity(pEn);
			pCom->m_pStaticMesh = Device::GetSingletonPtr()->GetSceneManager()
				->AddStaticMeshNode(pCom->strMeshName.c_str());
			if (pCom->m_pStaticMesh)
				pCom->m_pStaticMesh->SetPosition(math::Vector(pComPos->x, pComPos->y, pComPos->z));
		}

	} // NotifySystem

	//------------------------------------------------------------------------------------
	// Updates the system in asynchronism mode. 
	//------------------------------------------------------------------------------------
	void SystemMesh::AsyncUpdate()
	{

	} // AsyncUpdate

	//------------------------------------------------------------------------------------
	// Receive a message to process
	//------------------------------------------------------------------------------------
	bool SystemMesh::OnMessage( Message* msg )
	{
		return false;

	} // OnMessage

} // en

} // kge
