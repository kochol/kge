#ifndef KGE_STREAM_H
#define KGE_STREAM_H

#include "KgeUnknown.h"
#include "String.h"

namespace kge
{
	namespace io
	{
		//! Stream class is for working with streams
		class Stream: public KgeUnknown
		{
		public:

			//! Constructor
			Stream(void* pData, uint DataSize, core::stringc name);

			//! Destructor
			virtual ~Stream();

			//! Reads an amount of bytes from the Stream buffer.
			/*!
				\param buffer Pointer to buffer where read bytes are written to.
				\param sizeToRead Amount of bytes to read from the Stream.
				\return How many bytes were read.
			*/
			virtual uint Read(void* buffer, s32 sizeToRead);

			//! Returns the stream size
			virtual uint GetSize() {return m_iSize;}

			//! Returns the stream name
			virtual core::stringc GetName() {return m_sName;}

		protected:

			void						*	m_pStart;			//!< The start point of data
			u8							*	m_pCurrent;			//!< The current positoin of data
			uint							m_iSize,			//!< The data size
											m_iCursur;			//!< The cursur position
			core::stringc					m_sName;			//!< The stream name

		}; // Stream

	} // io

} // kge

#endif // KGE_STREAM_H
