// File name: Image.cpp
// Des: This file is the image control.
// Date: 8/1/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/gui/Image.h"

namespace kge
{
namespace gui
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Image::Image(u16* pIndices, core::RectI rect, gfx::Renderer* pRenderer)
	{
		m_pIndices		  = pIndices;
		m_Rect			  = rect;
		m_pRenderer		  = pRenderer;
		m_pVerts		  = new gfx::Vertex3T[4];
		m_bMouseOver	  = false;
		m_pVerts[0].tex.X = 0;
		m_pVerts[0].tex.Y = 0;
		m_pVerts[1].tex.X = 1;
		m_pVerts[1].tex.Y = 0;
		m_pVerts[2].tex.X = 0;
		m_pVerts[2].tex.Y = 1;
		m_pVerts[3].tex.X = 1;
		m_pVerts[3].tex.Y = 1;
		m_pVerts[0].pos.Z = 0;
		m_pVerts[1].pos.Z = 0;
		m_pVerts[2].pos.Z = 0;
		m_pVerts[3].pos.Z = 0;
		Update();

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Image::~Image()
	{
		delete[] m_pVerts;

	} // Destructor

	//------------------------------------------------------------------------------------
	// Renders the control
	//------------------------------------------------------------------------------------
	void Image::Render()
	{
		if (m_BackImage)
			m_pRenderer->SetTexture(m_BackImage);
		// TODO:
//		m_pRenderer->DrawTriangleList(m_pVerts, 4, m_pIndices, 6);

	} // Render

	//------------------------------------------------------------------------------------
	// Updates the control properties.
	//------------------------------------------------------------------------------------
	void Image::Update()
	{
		m_pVerts[0].pos.X = (float)m_Rect.X;
		m_pVerts[0].pos.Y = (float)m_Rect.Y;
		m_pVerts[1].pos.X = (float)m_Rect.X + m_Rect.Width;
		m_pVerts[1].pos.Y = (float)m_Rect.Y;
		m_pVerts[2].pos.X = (float)m_Rect.X;
		m_pVerts[2].pos.Y = (float)m_Rect.Y + m_Rect.Height;
		m_pVerts[3].pos.X = (float)m_Rect.X + m_Rect.Width;
		m_pVerts[3].pos.Y = (float)m_Rect.Y + m_Rect.Height;

	} // Update

	//------------------------------------------------------------------------------------
	// Set custom UV
	//------------------------------------------------------------------------------------
	void Image::SetUV( float u1, float v1, float u2, float v2 )
	{
		m_pVerts[0].tex.X = u1;
		m_pVerts[0].tex.Y = v1;
		m_pVerts[1].tex.X = u2;
		m_pVerts[1].tex.Y = v1;
		m_pVerts[2].tex.X = u1;
		m_pVerts[2].tex.Y = v2;
		m_pVerts[3].tex.X = u2;
		m_pVerts[3].tex.Y = v2;

	} // SetUV

} // gui

} // kge
