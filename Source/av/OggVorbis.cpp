// File name: OggVorbis.cpp
// Des: OggVorbis file loader
// Date: February 08, 2011 (19/11/1389)
// Programmer: Nader Golbaz

//#include <Windows.h>
//#include <MMSystem.h>
#if (KGE_USE_OGG == 1) && (KGE_USE_OPENAL == 1)
#include <memory.h>
#include "../../Libs/OggVorbis/vorbisfile.h"
#include "../../Headers/av/OggVorbis.h"

namespace kge
{

namespace av
{

    //----------------------------------------
    //
    //----------------------------------------
    OggLoader::OggLoader(const char* filename)
        : m_format(0), m_size(0), m_frequency(0), m_buffer(0)
    {
        FILE* f = fopen(filename, "rb");

        if (NULL == f) return;

        OggVorbis_File ogg = {0};
        ov_open(f, &ogg, NULL, 0);
        vorbis_info *info = ov_info(&ogg, -1);

        m_format = (1 == info->channels) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        m_frequency = info->rate;
        m_size = static_cast<size_t>(ov_pcm_total(&ogg, -1) * 2 * info->channels);

        /*
        WAVEFORMATEX wfx	= {0};
        wfx.wFormatTag		= WAVE_FORMAT_PCM;
        wfx.nChannels		= info->channels;
        wfx.nSamplesPerSec	= info->rate;
        wfx.wBitsPerSample	= 16;
        wfx.nBlockAlign		= wfx.wBitsPerSample / 8 * wfx.nChannels;
        wfx.nAvgBytesPerSec	= wfx.nSamplesPerSec * wfx.nBlockAlign;
        wfx.cbSize          = 0;
        */

        int i = 0;
        int sum = 0;
        bool eof = false;
        int currentSection = 0;
        char pcm[4096];

        m_buffer = KGE_NEW_ARRAY(char, m_size);

        while (!eof)
        {
            i = ov_read(&ogg, pcm, sizeof(pcm), 0, 2, 1, &currentSection);

            if (i > 0)
            {
                //std::copy(pcm , pcm + i, m_buffer + sum);
                memcpy(m_buffer + sum, pcm, i);
                sum += i;
            }
            else
            {
                eof = true;
            }
        }

        ov_clear(&ogg);
    }

    //----------------------------------------
    //
    //----------------------------------------
    OggLoader::~OggLoader()
    {
        delete m_buffer;
    }

} // av

} // kge

#endif // KGE_USE_OGG && KGE_USE_OPENAL
