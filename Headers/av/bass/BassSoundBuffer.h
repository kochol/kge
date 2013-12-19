
#ifndef BASS_SOUND_BUFFER_H
#define BASS_SOUND_BUFFER_H

#include "../../../Include/config.h"

#if KGE_USE_BASS == 1

#include "../../../Libs/bass/bass.h"

namespace kge
{
	namespace av
	{
		class BassSoundBuffer
		{
		public:
			//! constructor
			BassSoundBuffer(const char* filename, bool is3D, bool stream);

			//! destructor
			~BassSoundBuffer();

			//! 
			DWORD	GetHandle() const { return m_handle; }

		private:
			DWORD	m_handle;
			bool	m_stream;
		};

	} // av

} // kge

#endif // KGE_USE_BASS
#endif // BASS_SOUND_BUFFER_H

