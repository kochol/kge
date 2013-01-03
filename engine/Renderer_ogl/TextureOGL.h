#ifndef KGE_TEXTUREOGL_H
#define KGE_TEXTUREOGL_H

#include "../include/Texture.h"

namespace kge
{
    namespace gfx
    {
        class TextureOGL : public Texture
        {
            public:

                //! Constructor
                TextureOGL(Image* pImg);

                //! Destructor
                ~TextureOGL();

				uint		m_iTexID;		//!< The texture ID in opengl

        }; // TextureOGL

    } // gfx

} // kge

#endif // KGE_TEXTUREOGL_H
