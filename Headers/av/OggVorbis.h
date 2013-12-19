// File name: OggVorbis.h
// Des: OggVorbis file loader
// Date: February 08, 2011 (19/11/1389)
// Programmer: Nader Golbaz

#ifndef OGGVORBIS_H
#define OGGVORBIS_H
#include "../../include/kgedef.h"

#if (KGE_USE_OGG == 1) && (KGE_USE_OPENAL == 1)
#include "../../Libs/al/al.h"

namespace kge
{

namespace av
{
    //!
    class OggLoader
    {
        //! Copy constructor
        OggLoader(const OggLoader& other); // non copyable

        //! Assignment operator
        OggLoader& operator =(const OggLoader& other); // non copyable

    public:
        //!
        OggLoader(const char* filename);

        //!
        ~OggLoader();

        //!
        int             GetFormat() const { return m_format; }

        //!
        size_t          GetSize() const { return m_size; }

        //!
        int             GetFrequency() const { return m_frequency; }

        //!
        const char*     GetData() const { return m_buffer; }

    private:
        int             m_format;
        size_t          m_size;
        int             m_frequency;
        char*           m_buffer;
    };

} // av

} // kge

#endif // KGE_USE_OGG && KGE_USE_OPENAL
#endif // OGGVORBIS_H