
#include "../../../Headers/av/bass/BassSoundBuffer.h"

#if KGE_USE_BASS == 1

#include "../../../Libs/physfs/physfs.h"
#include "../../../include/io/Logger.h"
#include "../../../Libs/bass/bass.h"
#include <vector>
#include <algorithm>

#define LOAD_FROM_FILE

namespace kge
{
	namespace av
	{

		//////////////////////////////////////////////////////////////////////////

		void CALLBACK custom_close(void *handle)
		{
			PHYSFS_close(static_cast<PHYSFS_File*>(handle));
		}

		QWORD CALLBACK custom_length(void *handle)
		{
			return PHYSFS_fileLength(static_cast<PHYSFS_File*>(handle));
		}

		DWORD CALLBACK custom_read(void *buffer, DWORD length, void *handle)
		{
			return PHYSFS_read(static_cast<PHYSFS_File*>(handle), buffer, sizeof(unsigned char), length);
		}

		BOOL CALLBACK custom_seek(QWORD offset, void *handle)
		{
			if (PHYSFS_seek(static_cast<PHYSFS_File*>(handle), offset))
			{
				return TRUE;
			}

			return FALSE;
		}

		//////////////////////////////////////////////////////////////////////////

		// Constructor
		BassSoundBuffer::BassSoundBuffer(const char* filename, bool is3D, bool stream)
			: m_handle(0), m_stream(stream)
		{
			DWORD flags = 0;

			if (is3D)
			{
				flags |= BASS_SAMPLE_MONO | BASS_SAMPLE_3D | BASS_SAMPLE_SOFTWARE;

				if (!stream)
				{
					flags |= BASS_SAMPLE_OVER_DIST;
				}
			}
			else if (!stream)
			{
				flags |= BASS_SAMPLE_OVER_POS | BASS_SAMPLE_SOFTWARE;
			}

#if defined(LOAD_FROM_FILE)

			////// load from file

			m_handle = stream ?
				BASS_StreamCreateFile(FALSE, filename, 0, 0, flags) :
				BASS_SampleLoad(FALSE, filename, 0, 0, 5, flags);

			//////

#else

			////// load from archive

			std::string name(filename);
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			std::string path("../../"); // 
			std::string::size_type pos = name.find(path);
			if (pos != std::string::npos)
			{
				name = name.substr(pos + path.size());
			}

			if (!PHYSFS_exists(name.c_str())) { return; }
			PHYSFS_File* file = PHYSFS_openRead(name.c_str());

			//if (!PHYSFS_exists(filename)) { return; }
			//PHYSFS_File* file = PHYSFS_openRead(filename);

			BASS_FILEPROCS fileProc = {0};
			fileProc.close = custom_close;
			fileProc.length = custom_length;
			fileProc.read = custom_read;
			fileProc.seek = custom_seek;

			if (!stream)
			{
				const unsigned int length = PHYSFS_fileLength(file);
				std::vector<unsigned char> buffer(length);
				const unsigned int readLen = PHYSFS_read(file, &buffer[0], sizeof(unsigned char), length);
				PHYSFS_close(file);

				if (readLen == length)
				{
					m_handle = BASS_SampleLoad(TRUE, &buffer[0], 0, length, 5, flags);
				}
			}
			else
			{
				m_handle = BASS_StreamCreateFileUser(STREAMFILE_NOBUFFER, flags, &fileProc, file);
			}

			//////

#endif // debug

			if (m_handle)
			{
				io::Logger::Log(io::ELM_Information,"Sound Buffer '%s' loaded successfully.", filename);			
			}
			else
			{
				io::Logger::Log(io::ELM_Information, "Loading Sound buffer '%s' failed.", filename);
			}
		}

		// Destructor
		BassSoundBuffer::~BassSoundBuffer()
		{
			if (m_stream)
			{
				BASS_StreamFree(m_handle);
			}
			else
			{
				BASS_SampleFree(m_handle);
			}
		}

	} // av

} // kge

#endif // KGE_USE_BASS

