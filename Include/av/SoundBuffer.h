// File name: SoundBuffer.h
// Des: This class is for storing sound buffer
// Date: 15/3/1387
// Programmer: Pooya Shahinfar (Pswin), Ali Akbar Mohammadi (Kochol)

#ifndef KGE_SOUNDBUFFER_H
#define KGE_SOUNDBUFFER_H

#include "../KgeUnknown.h"
#include "../kgedef.h"


namespace kge
{
namespace av
{
class KGE_API SoundBuffer: public KgeUnknown
{
public:

	//! Constructor 
	SoundBuffer() {}
	
	//! Destructor
	virtual ~SoundBuffer() { /* Nothing to do */ }

}; // SoundBuffer

} // av

} // kge

#endif // KGE_SOUNDBUFFER_H
