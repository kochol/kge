#ifndef KGE_STREAM_H
#define KGE_STREAM_H

#include "KgeUnknown.h"

namespace kge
{
	namespace io
	{
		//! Stream class is for working with streams
		class Stream: public KgeUnknown
		{
		public:

			//! Constructor
			Stream(void* pData, uint DataSize);

			//! Destructor
			virtual ~Stream();

			//! Reads an amount of bytes from the Stream buffer.
			/*! 
				\param buffer Pointer to buffer where read bytes are written to.
				\param sizeToRead Amount of bytes to read from the Stream.
				\return How many bytes were read. 
			*/
			virtual uint Read(void* buffer, s32 sizeToRead);

		protected:

			void						*	m_pStart;			//!< The start point of data
			u8							*	m_pCurrent;			//!< The current positoin of data
			uint							m_iSize,			//!< The data size
											m_iCursur;			//!< The cursur position

		}; // Stream

	} // io

} // kge

#endif // KGE_STREAM_H
