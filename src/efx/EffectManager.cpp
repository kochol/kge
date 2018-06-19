// File name: EffectManager.cpp
// Des: Effect manager works with scene manager to manage shadows , Material shader effects and post process effects.
// Date: 16/12/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Include/efx/EffectManager.h"
#include "../../Include/efx/Bloom.h"
#include "../../Include/Device.h"
#include "../../Include/gfx/Renderer.h"

extern kge::efx::EffectManager	*	g_pEffectManager;

namespace kge
{

namespace efx
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	EffectManager::EffectManager()
		: m_pRefraction(NULL), m_bNeedColor(false), m_pColorMap(NULL), m_pBloom(NULL),
		m_pShadow(NULL)
	{
		for (int i = 0; i < 8; i++)
		{
			m_pTempRenderTarget[i] = NULL;
		}
		g_pEffectManager = this;

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	EffectManager::~EffectManager()
	{

		for (std::vector<Effect*>::iterator it = m_arrShadows.begin();
			it != m_arrShadows.end(); ++it)
		{
			delete *it;
		}

		delete m_pRefraction;
		delete m_pColorMap;

		g_pEffectManager = NULL;

	} // Destructor

	//------------------------------------------------------------------------------------
	// Returns the EffectManager class pointer.
	//------------------------------------------------------------------------------------
	EffectManager* EffectManager::GetSingletonPtr()
	{
		return g_pEffectManager;

	} // GetSingletonPtr

	//------------------------------------------------------------------------------------
	// The scene manager calls this function when it calls every node prerender function
	//------------------------------------------------------------------------------------
	void EffectManager::AfterSceneNodesPreRender(float elapsedTime)
	{
		// Shadows
		for (std::vector<Effect*>::iterator it = m_arrShadows.begin();
			it != m_arrShadows.end(); ++it)
		{
			(*it)->PreRender();
		}
		for (std::vector<Effect*>::iterator it = m_arrShadows.begin();
			it != m_arrShadows.end(); ++it)
		{
			(*it)->Render();
		}

		if (m_bNeedColor)
			gfx::Renderer::GetSingletonPtr()->SetRenderTarget(0, m_pColorMap);

	} // AfterSceneNodesPreRender

	void EffectManager::AfterSceneNodesRender(float elapsedTime)
	{
		if (m_bNeedColor)
		{
			// Stop rendering in color map
			gfx::Renderer::GetSingletonPtr()->EndRendering();
			if (!m_pRefraction)
			{
				gfx::Renderer::GetSingletonPtr()->SetRenderTarget();
			}
			else
			{
				gfx::Renderer::GetSingletonPtr()->SetRenderTarget(0, m_pColorMap);
			}
			gfx::Renderer::GetSingletonPtr()->SetTexture(m_pColorMap, 6);
			gfx::Renderer::GetSingletonPtr()->SetTextureParams(gfx::ETP_Clamp, 6);
			if (gfx::Renderer::GetSingletonPtr()->BeginRendering(false, false, false))
			{
				if (m_pRefraction)
				{
					m_pRefraction->PreRender(elapsedTime);
					m_pRefraction->Render();
					gfx::Renderer::GetSingletonPtr()->EndRendering();
					gfx::Renderer::GetSingletonPtr()->SetRenderTarget();
				}

				// Render bloom
				if (m_pBloom)
				{
					m_pBloom->PreRender();
					m_pBloom->Render();
				}
			}
		}

	} // AfterSceneNodesRender

	//------------------------------------------------------------------------------------
	// Creates a ShadowMapProjection class and returns its pointer
	//------------------------------------------------------------------------------------
	ShadowMapProjection* EffectManager::AddShadowMapProjection( int ShadowMapSize /*= 512*/ )
	{
		ShadowMapProjection* smp = KGE_NEW(ShadowMapProjection)(ShadowMapSize, Device::GetSingletonPtr()->GetSceneManager());
		smp->Init();
		m_arrShadows.push_back(smp);

		m_pShadow = smp;

		return smp;

	} // AddShadowMapProjection

	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	void EffectManager::RemoveShadowMapProjection(ShadowMapProjection* pShadowMapProj)
	{
		const size_t m_arrShadowsSize = m_arrShadows.size();
		for (size_t i = 0; i < m_arrShadowsSize; ++i)
		{
			if (m_arrShadows[i] == pShadowMapProj)
			{
				delete pShadowMapProj;
				m_arrShadows[i] = m_arrShadows[m_arrShadowsSize - 1];
				m_arrShadows.pop_back();
				m_pShadow = NULL;
				return;
			}
		}

	} //RemoveShadowMapProjection

	//------------------------------------------------------------------------------------
	// Gets the shadow map
	//------------------------------------------------------------------------------------
	ShadowMapProjection* EffectManager::GetShadowMapProjection()
	{
		return m_pShadow;

	} // GetShadowMapProjection

	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	void EffectManager::AddRefractionEffect()
	{
		if (!m_pRefraction)
		{
			m_pRefraction = KGE_NEW(Refraction)();
			NeedColorMap();
		}

	} // AddRefractionEffect

	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	void EffectManager::RemoveRefractionEffect()
	{
		delete m_pRefraction;
		m_pRefraction = 0;
		m_bNeedColor = false;
		delete m_pColorMap;
		m_pColorMap = NULL;

	} // RemoveRefractionEffect

	//------------------------------------------------------------------------------------
	// Returns the refraction effect then you can add nodes to it.
	//------------------------------------------------------------------------------------
	Refraction* EffectManager::GetRefractionEffect()
	{
		return m_pRefraction;

	} // GetRefractionEffect

	void EffectManager::NeedColorMap()
	{
		if (m_bNeedColor) { return; }

		gfx::Renderer*	ren = gfx::Renderer::GetSingletonPtr();

		if (m_pColorMap)
		{
			delete m_pColorMap;
			m_pColorMap = NULL;
		}

		if (!ren->CreateRenderableTexture( &m_pColorMap, 
											ren->GetWindowWidth(), 
											ren->GetWindowHeight(), 
											gfx::ETF_A8R8G8B8, 
											false, 
											true))
		{
			io::Logger::Log("Can't create render able texture for color map for post process effects", io::ELM_Error);
			return;
		}

		m_bNeedColor = true;

	} // NeedColorMap

	//------------------------------------------------------------------------------------
	// For internal use. Renderer calls it on window resize
	//------------------------------------------------------------------------------------
	void EffectManager::OnResize()
	{
		if (m_bNeedColor)
		{
			m_bNeedColor = false;
			NeedColorMap();
		}

	} // OnResize

	//------------------------------------------------------------------------------------
	// Returns a temp render target (size = screen size) for index 0 to 7
	//------------------------------------------------------------------------------------
	gfx::Texture* EffectManager::GetTempRenderTarget( int Index )
	{
		if (Index < 0 || Index > 7) { return NULL; }

		if (!m_pTempRenderTarget[Index])
		{
			gfx::Renderer* ren = gfx::Renderer::GetSingletonPtr();

			if (!ren->CreateRenderableTexture(&m_pTempRenderTarget[Index], 
				ren->GetWindowWidth(), 
				ren->GetWindowHeight(), 
				gfx::ETF_A8R8G8B8))
			{
				io::Logger::Log( io::ELM_Error, "Can't create render able texture for temp texture %d", Index );
				return NULL;
			}

		}

		return m_pTempRenderTarget[Index];

	} // GetTempRenderTarget

	//------------------------------------------------------------------------------------
	// Create the post bloom effect and returns its pointer
	//------------------------------------------------------------------------------------
	Bloom* EffectManager::GetBloomEffect()
	{
		if (!m_pBloom)
		{
			m_pBloom = KGE_NEW(Bloom)();
			NeedColorMap();
		}

		return m_pBloom;

	} // GetBloomEffect

	//------------------------------------------------------------------------------------
	// For internal use. Renderer calls it on device reset
	//------------------------------------------------------------------------------------
	void EffectManager::OnReset()
	{
		// Shadows
		for (std::vector<Effect*>::iterator it = m_arrShadows.begin();
			it != m_arrShadows.end(); ++it)
		{
			(*it)->OnReset();
		}

	} // OnReset

} // efx

} // kge
