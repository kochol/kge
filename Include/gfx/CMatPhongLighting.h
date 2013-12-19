// File name: CMatPhongLighting.h
// Des: The phong lighting model
// Date: 5/9/1390
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_CMATPHONGLIGHTING_H
#define KGE_CMATPHONGLIGHTING_H

#include "CustomMaterial.h"

namespace kge
{
	namespace gfx
	{
		class KGE_API CMatPhong: public CustomMaterial
		{
		public:

			//! Constructor
			CMatPhong();

			//! Destructor
			~CMatPhong();

			//! Returns the singleton pointer
			static CustomMaterial* GetSingletonPtr();

			//! Adds Vertex shader code
			bool CreateVertexShaderCode(MaterialParams* pMP, VertexShaderEntry ese);

			//! Adds pixel shader code
			bool CreatePixelShaderCode(MaterialParams* pMP, PixelShaderEntry ese);

			//! Set the constants
			void SetShaderConstants( ShaderInstance* pSI );

			//! Get a copy
			//CustomMaterial* GetCopy();

		protected:

		}; // CMatPhong

	} // gfx

} // kge

#endif // KGE_CMATPHONGLIGHTING_H
