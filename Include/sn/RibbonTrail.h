// File name: RibbonTrail.h
// Des: 
// Date: March 26, 2011
// Programmer: Nader Golbaz

#ifndef RIBBONTRAIL_H
#define RIBBONTRAIL_H

#include "SceneNode.h"

namespace kge
{

namespace gfx
{
	class HardwareBuffer;
}

namespace sn
{

	struct RBuffer;

	class KGE_API RibbonTrail : public SceneNode
	{
		//! Copy constructor
		RibbonTrail(const RibbonTrail& other); // non copyable

		//! Assignment operator
		RibbonTrail& operator =(const RibbonTrail& other); // non copyable

	public:
		//! Constructor
		RibbonTrail(
			SceneManager* sceneMan,
			const char* shaderName,
			const char* textureName,
			const math::Vector& textureRect,
			const gfx::Colorf& initialColor,
			const gfx::Colorf& finalColor,
			float width,
			float speed,
			float age,
			float chaosRatio);

		//! Destructor
		~RibbonTrail();

		//! 
		void    PreRender(float elapsedTime);

		//! 
		void    Render();

		//! 
		void    PostRender();

		//! 
		void    Draw(bool WithMaterial, bool WithTransform, bool bPosition, bool bNormalTexcoord, bool bTangentBinormal);

		//!
		void    Start(const math::Vector& target);

		//!
		void	Start();

		//!
		void	Finish() { m_running = false; }

		//!
		bool    Running() const { return m_running; }

		//!
		void    SetWidth(float width) { m_width = width; }

		//!
		void    SetSpeed(float speed) { m_speed = speed; }

		//!
		void    SetAge(float age) { m_totalAge = age; }

		//!
		void    SetInitialColor(const gfx::Colorf& color) { m_initialColor = color; }

		//!
		void    SetFinalColor(const gfx::Colorf& color) { m_finalColor = color; }

		//!
		void    SetChaosRatio(float chaosRatio) { m_chaosRatio = chaosRatio; }

		//!
		float   GetWidth() const { return m_width; }

		//!
		float   GetSpeed() const { return m_speed; }

		//!
		float   GetAge() const { return m_totalAge; }

		//!
		float   GetChaosRatio() const { return m_chaosRatio; }

		//!
		const char* GetTextureName() const { return m_textureName.c_str(); }

		//!
		const char* GetShaderName() const { return m_shaderName.c_str(); }

		//!
		const math::Vector& GetTextureRect() const { return m_rect; }

		//!
		const gfx::Colorf& GetInitialColor() const { return m_initialColor; }

		//!
		const gfx::Colorf& GetFinalColor() const { return m_finalColor; }

	private:
		std::string             m_shaderName;
		std::string             m_textureName;
		gfx::Material*          m_pMaterial;
		RBuffer*                m_pRB;
		unsigned int			m_sizeOfData;
		unsigned int			m_bufferOffset;
		math::Vector            m_rect;
		gfx::Colorf             m_initialColor;
		gfx::Colorf             m_finalColor;
		float                   m_width;
		float                   m_speed;
		float                   m_totalAge;
		float                   m_currentAge;
		float                   m_chaosRatio;
		float                   m_elapsed;
		bool                    m_running;

		unsigned int            m_maxCount;
		unsigned int            m_activeCount;
		math::Vector*           m_positions;
		gfx::Color*             m_colors;

		math::Vector            m_prevPos;
		unsigned int			m_index;
		bool					m_bolt;
	};

} // sn

} // kge

#endif // RIBBONTRAIL_H
