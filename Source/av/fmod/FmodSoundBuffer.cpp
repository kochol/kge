// File name: FmodSoundBuffer.cpp
// Des: interface for the sound systems
// Date: 15/12/1389 (jalali), Mar-06-2011
// Programmer: Pooya Shahinfar(Pswin)

#include "../../../Headers/av/fmod/FmodSoundBuffer.h"

#if KGE_USE_FMOD == 1

#include "../../../include/io/Logger.h"
#include "../../../Libs/physfs/physfs.h"

namespace kge
{
	namespace av
	{

		//////////////////////////////////////////////////////////////////////////

		FMOD_RESULT F_CALLBACK custom_open(const char *name, int unicode, unsigned int *filesize, void **handle, void **userdata)
		{
			if (!PHYSFS_exists(name)) { return FMOD_ERR_FILE_NOTFOUND; }
			PHYSFS_File* file = PHYSFS_openRead(name);
			const unsigned int length = PHYSFS_fileLength(file);
			*filesize = length;
			*handle = file;
			return FMOD_OK;
		}

		FMOD_RESULT F_CALLBACK custom_close(void *handle, void *userdata)
		{
			PHYSFS_close(static_cast<PHYSFS_File*>(handle));
			return FMOD_OK;
		}

		FMOD_RESULT F_CALLBACK custom_read(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata)
		{
			const unsigned int readLen = PHYSFS_read(static_cast<PHYSFS_File*>(handle), buffer, sizeof(unsigned char), sizebytes);
			*bytesread = readLen;
			return FMOD_OK;
		}

		FMOD_RESULT F_CALLBACK custom_seek(void *handle, unsigned int pos, void *userdata)
		{
			PHYSFS_seek(static_cast<PHYSFS_File*>(handle), pos);
			return FMOD_OK;
		}

		//////////////////////////////////////////////////////////////////////////

		// Constructor
		FmodSoundBuffer::FmodSoundBuffer( FMOD::System* system, const char* filename, bool is3D, bool streaming )
			: m_system(system), m_sound(0)
		{	
			FMOD_MODE flags = 0;

			if (is3D) { flags |= FMOD_3D; }

			if (streaming) { flags |= FMOD_CREATESTREAM; }

			if (flags == 0) { flags = FMOD_DEFAULT; }

			////// load from archive

			//FMOD_CREATESOUNDEXINFO exinfo = {0};
			//exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);

			//exinfo.useropen = custom_open;
			//exinfo.userclose = custom_close;
			//exinfo.userread = custom_read;
			//exinfo.userseek = custom_seek;

			//FMOD_RESULT re = m_system->createSound(filename, flags, &exinfo, &m_sound);

			//////

			////// load from file

			FMOD_RESULT re = m_system->createSound(filename, flags, 0, &m_sound);

			//////

			if (re != FMOD_OK)
			{
				io::Logger::Log(io::ELM_Information,"loading Sound buffer '%s' failed.", filename);
				return;
			}

			io::Logger::Log(io::ELM_Information,"Sound Buffer '%s' loaded successfully.", filename);

		}

		// destructor
		FmodSoundBuffer::~FmodSoundBuffer()
		{
			m_sound->release();
		}

	} // av

} // kge

#endif // KGE_USE_FMOD

