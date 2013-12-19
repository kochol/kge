// File name: AnimatedTexture.h
// Des: 
// Date: February 08, 2011 (19/11/1389)
// Programmer: Nader Golbaz

#ifndef ANIMATEDTEXTURE_H
#define ANIMATEDTEXTURE_H

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

struct AnimatedTextureParameters
{
	const char*                 textureName;        //
	const char*                 shaderName;         //
	math::Vector                textureRect;        //
	unsigned int                horizontalImages;   //
	unsigned int                verticalImages;     //
	math::Vector                size;               //
	gfx::Colorf                 initialColor;       //
	gfx::Colorf                 finalColor;         //
	float                       fDuration;          //
	float                       fAngularVelocity;   //
	gfx::Colorf                 middleColor;        //
	math::Vector                middleSize;         //
	bool                        fixedAxis;          //
	bool						positionOnly;		//

	AnimatedTextureParameters() :
		textureName(0),
		shaderName(0),
		textureRect(0.0f, 0.0f, 1.0f, 1.0f),
		horizontalImages(1),
		verticalImages(1),
		size(1.0f, 1.0f, 1.0f, 1.0f),
		initialColor(1.0f, 1.0f, 1.0f, 1.0f),
		finalColor(1.0f, 1.0f, 1.0f, 1.0f),
		fDuration(1.0f),
		fAngularVelocity(0.0f),
		middleColor(1.0f, 1.0f, 1.0f, 1.0f),
		middleSize(1.0f, 1.0f, 1.0f, 1.0f),
		fixedAxis(false),
		positionOnly(true)
   {}
};

//!
class KGE_API AnimatedTexture : public SceneNode
{
	//! Copy constructor
	AnimatedTexture(const AnimatedTexture& other); // non copyable

	//! Assignment operator
	AnimatedTexture& operator =(const AnimatedTexture& other); // non copyable

public:
	//! Constructor
	AnimatedTexture(const AnimatedTextureParameters& p);

	//! Destructor
	~AnimatedTexture();

	//!
	void                    PreRender(float elapsedTime);

	//!
	void                    Render();

	//!
	void                    PostRender();

	//!
	void                    Draw( bool WithMaterial , bool WithTransform, bool bPosition /* = true */, bool bNormalTexcoord /* = true */, bool bTangentBinormal /* = true */ );

	//! 
	void                    Play(bool repeat = false);

	//! 
	void                    Stop();

	//! 
	void                    SetTexture(const char* filename);

	//! 
	void                    SetShader(const char* filename);

	//! This function called by SceneManager only.
	void                    SetVertexBuffer();

	//! rect.x = left, rect.y = top, rect.z = right, rect.w = bottom
	void                    SetTextureRect(const math::Vector& rect, unsigned int horizontalFrames, unsigned int verticalFrames);

	//! Sets size of billboard
	void                    SetSize(const math::Vector& size);

	//!
	void                    SetMiddleSize(const math::Vector& size);

	//! 
	void                    SetDuration(float duration) { m_duration = duration; }

	//! Sets rotation speed
	void                    SetAngularVelocity(float angularVelocity) { m_angularVelocity = angularVelocity; }

	//!
	void                    SetInitialColor(const gfx::Colorf& initialColor) { m_initialColor = initialColor; }

	//!
	void                    SetMiddleColor(const gfx::Colorf& middleColor) { m_middleColor = middleColor; }

	//!
	void                    SetFinalColor(const gfx::Colorf& finalColor) { m_finalColor = finalColor; }

	//!
	void                    SetFixedAxis(bool fixedAxis) { m_fixedAxis = fixedAxis; }

	//!
	void					SetPositionOnly(bool positionOnly) { m_positionOnly = positionOnly; }

	//!
	const char*             GetTextureName() const { return m_textureName.c_str(); }

	//!
	const char*             GetShaderName() const { return m_shaderName.c_str(); }

	//!
	const math::Vector&     GetTextureRect() const { return m_rect; }

	//!
	unsigned int            GetHorizontalImages() const { return m_horizontalImages; }

	//!
	unsigned int            GetVerticalImages() const { return m_verticalImages; }

	//!
	math::Vector            GetSize() const;

	//!
	math::Vector            GetMiddleSize() const;

	//!
	float                   GetDuration() const { return m_duration; }

	//!
	float                   GetAngularVelocity() const { return m_angularVelocity; }

	//!
	const gfx::Colorf&      GetInitialColor() const { return m_initialColor; }

	//!
	const gfx::Colorf&      GetMiddleColor() const { return m_middleColor; }

	//!
	const gfx::Colorf&      GetFinalColor() const { return m_finalColor; }

	//!
	bool                    IsFixedAxis() const { return m_fixedAxis; }

	//!
	bool					PositionOnly() const { return m_positionOnly; }

private:
	std::string             m_textureName;
	std::string             m_shaderName;
	gfx::Material*          m_pMaterial;
	//gfx::HardwareBuffer*    m_pVB;
	RBuffer*                m_pRB;
	unsigned int            m_bufferOffset;
	const kge::u32			m_sizeOfData;
	math::Vector            m_rect;
	unsigned int            m_horizontalImages;
	unsigned int            m_verticalImages;
	math::Vector            m_size;
	math::Vector            m_middleSize;
	float                   m_duration;
	float                   m_angularVelocity;
	float                   m_fRot;
	gfx::Colorf             m_initialColor;
	gfx::Colorf             m_middleColor;
	gfx::Colorf             m_finalColor;
	bool                    m_repeat;
	float                   m_elapsed;
	bool                    m_running;
	bool                    m_fixedAxis;
	bool					m_positionOnly;
};

} // sn

} // kge

#endif // ANIMATEDTEXTURE_H
