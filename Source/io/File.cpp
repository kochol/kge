// File name: Keyboard.h
// Des: In class baraye kar ba file ha hast va mitone file ha ro az dakhele zip ham 
//		bekhone.
// Date: 26/6/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/io/File.h"
#include "../../Include/io/Logger.h"
#include <memory.h>
#include "../../Include/core/KgeMemory.h"
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
#	include <io.h>
#else
#	include <unistd.h>
#endif

namespace kge
{
namespace io
{
	// ********
	// Sazande.
	// ********
	File::File(): m_cFileName(NULL)
	{
		m_pFile = NULL;

	} // Sazande

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	File::~File()
	{
		if (m_pFile)
			fclose(m_pFile);	// Closing the file.
		KGE_DELETE_ARRAY(m_cFileName);

	} // Destructor

	// ** **** ** *** ****** ****** ******* ** **********
	// Ye file ro baz mikone baraye khondan ya neveshtan.
	// ** **** ** *** ****** ****** ******* ** **********
	bool File::Open(const char* FileName, bool CreateFile)
	{
		m_pFile = fopen(FileName, "rb");

		// agar file peyda nashod.
		if (!m_pFile)
		{
			if (CreateFile)
			{
				// aval misazimesh
				m_pFile = fopen(FileName, "a");
				
				// bad dobare amade khondan va neveshtanesh mikonimesh.
				m_pFile = fopen(FileName, "r+");

				// agar moshkele dige i bod khoroj.
				if (!m_pFile)
				{
					io::Logger::Log("Can't create file: ", FileName, ELM_Error);
					return false;
				}
			}
			else
			{
				io::Logger::Log("File not found: ", FileName, ELM_Error);
				return false;
			}
		}

		// Save the FileName.
		int i = sizeof FileName;
		m_cFileName = KGE_NEW_ARRAY(char, i);
		memcpy(m_cFileName, FileName, i);

		return true;
	} // Open(char* FileName)

	// ******** ******** ** *****
	// Khondane ghesmati az file.
	// ******** ******** ** *****
	int File::Read(void* Buffer, u32 ReadSize)
	{
		if (!IsOpen())
			return 0;

		int i = (int)fread(Buffer, 1, ReadSize, m_pFile);
		if (i==-1)
			io::Logger::Log("Could not read file: ", m_cFileName, io::ELM_Error);

		return i;
	} // Read

	// ***** ****** ** ****** **** ***** ******** *****
	// Check mikone ta bebine file amade estefade hast.
	// ***** ****** ** ****** **** ***** ******** *****
	bool File::IsOpen()
	{
		return m_pFile != NULL;
	} // IsOpen

	// ****** ****** ** ****** ** ****** ** *****
	// Hrekat kardan be noghte ye digari az file.
	// ****** ****** ** ****** ** ****** ** *****
	int File::Seek(u32 pos, bool Current)
	{
		if (Current)
			return fseek(m_pFile, pos, SEEK_CUR);
		else
			return fseek(m_pFile, pos, SEEK_SET);
	} // Seek

	// ****** ******* **** **** * ** *** ***** ***
	// Bedast avadane size file e ke baz karde ed.
	// ****** ******* **** **** * ** *** ***** ***
	u32 File::GetSize()
	{
		if (!IsOpen())
			return 0;

		fseek(m_pFile, 0, SEEK_END);
		int iEnd = ftell(m_pFile);
		fseek(m_pFile, 0, SEEK_SET);
		int iStart = ftell(m_pFile);
		return (iEnd - iStart);
	} // GetSize

	void File::Close()
	{
		if (m_pFile)
			fclose(m_pFile);	// bastane file.

	}

	int File::write( void* Buffer, u32 ReadSize )
	{
		if (!IsOpen())
			return 0;

		int i = (int)fwrite(Buffer, 1, ReadSize, m_pFile);
		if (i==-1)
			io::Logger::Log("Could not write to file: ", m_cFileName, io::ELM_Error);

		return i;

	}

	//------------------------------------------------------------------------------------
	// Check if the file exist
	//------------------------------------------------------------------------------------
	bool File::Exist( const char* cstrFileName )
	{
		if (access(cstrFileName, 0) != -1)
			return true;

		return false;

	} // Exist

} // io

} // kge