#include <string.h>
#include "../../include/kge/io/Stream.h"

namespace kge
{
	namespace io
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		Stream::Stream( void* pData, uint DataSize, std::string name ): m_pStart(pData),
			m_pCurrent(static_cast<u8*>(pData)),	m_iSize(DataSize), m_iCursur(0),
			m_sName(name)
		{

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		Stream::~Stream()
		{
			KGE_DELETE_ARRAY(m_pStart);

		} // Destructor

		//------------------------------------------------------------------------------------
		// Reads an amount of bytes from the Stream buffer.
		//------------------------------------------------------------------------------------
		kge::uint Stream::Read( void* buffer, s32 sizeToRead )
		{
			// Check for out bounds
			if (m_iCursur + sizeToRead > m_iSize)
				sizeToRead -= m_iCursur + sizeToRead - m_iSize;

			if (sizeToRead <= 0)
				return 0;

			// copy the data
			memcpy(buffer, m_pCurrent + m_iCursur, sizeToRead);

			m_iCursur += sizeToRead;

			return sizeToRead;

		} // Read

	} // io

} // kge
