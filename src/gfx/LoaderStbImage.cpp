#include "../../Headers/gfx/LoaderStbImage.h"
#include "stb_image.h"
#include "../../include/kge/io/Stream.h"
#include "../../include/kge/io/Logger.h"
#include "../../include/kge/gfx/Texture.h"
#include "../../include/kge/gfx/Renderer.h"
#include "../../include/kge/enums.h"
#include "../../include/kge/gfx/ImageData.h"

//! Renderer public pointer
KGE_IMPORT extern kge::gfx::Renderer*	g_pRenderer;

namespace kge
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	LoaderStbImage::LoaderStbImage()
	{
		m_aFileExtension.push_back("png");
		m_aFileExtension.push_back("jpg");
		m_aFileExtension.push_back("gif");
		m_aFileExtension.push_back("psd");
		m_aFileExtension.push_back("tga");
		m_aFileExtension.push_back("pic");

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	LoaderStbImage::~LoaderStbImage()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// Loads a resource from a FileSystem and return its pointer.
	//------------------------------------------------------------------------------------
	Resource* LoaderStbImage::LoadResource( io::Stream* pStream )
	{
		// Create the buffer
		std::vector<stbi_uc> buffer(pStream->GetSize());
		pStream->Read((void*)&buffer[0], pStream->GetSize());

		int x, y, n;
		stbi_uc* data = stbi_load_from_memory(&buffer[0], pStream->GetSize(), &x, &y, &n, 4);
		if (!data)
		{
			io::Logger::Error("Can't load texture %s", pStream->GetName().c_str());
			io::Logger::Debug("Reason: %s", stbi_failure_reason());
			return NULL;
		}

		gfx::TextureFormat format = gfx::ETF_A8R8G8B8;

		// Create image resource
		gfx::ImageData* pimg = KGE_NEW(gfx::ImageData)
			(0, pStream->GetName().c_str(), (u8*)data, x * y * 4, x, 
			y, 4, 32, format, 1);

		// Create texture from an Image
		gfx::Texture* pTex = g_pRenderer->CreateTexture(pimg);

		// Maybe we want to send it to image resource manager later but for now we simply delete it.
		KGE_DELETE(pimg, ImageData);

		io::Logger::Info("Texture loaded: %s", pStream->GetName().c_str());

		return pTex;

	} // LoadResource

} // kge