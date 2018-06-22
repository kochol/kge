// File name: String.cpp
// Date: 9/5/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../include/kge/core/String.h"
#include "../../include/kge/core/KgeMemory.h"
#include "../../include/kge/core/KgeArray.h"
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <vector>

#ifndef NULL
#define NULL 0
#endif

namespace kge
{
namespace core
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	String::String(): m_pChar(NULL)
	{
		m_vArray = KGE_NEW(KgeArray)(sizeof(char), 10, 10);

	} // String()

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	String::String( const char* str ): m_pChar(NULL)
	{
		m_vArray = KGE_NEW(KgeArray)(sizeof(char), 10, 10);
		m_vArray->Clear();

		if (str)
		{
			u32 len = (u32)strlen(str);
			m_vArray->Add((void*)str, len);

		} // if (str)

	} // String( const char* str )

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	String::~String()
	{
		KGE_DELETE(m_vArray, KgeArray);
		KGE_DELETE_ARRAY(m_pChar);

	} // ~String()

	// ******** * ** *
	// Amalgare = ba T
	// ******** * ** *
	void String::operator = (const char* str)
	{
		m_vArray->Clear();

		if (str)
		{
			u32 len = (u32)strlen(str);
			m_vArray->Add((void*)str, len);

		} // if (str)

	} // operator = (const T* str)

	void String::operator=( const String& str )
	{
		m_vArray->Clear();
		u32 len = str.GetLenght();
		for (u32 i = 0; i < len; i++)
			m_vArray->Add(str.m_vArray->Get(i));

	}

	//---------------------------------
	// ezafe kardane ye matn be string.
	//---------------------------------
	void String::operator += (const char* str)
	{
		if (!str)
			return;

		u32 len = (u32)strlen(str);
		m_vArray->Add((void*)str, len);

	} // operator += char*

	//--------------------------
	// Add an integer to string.
	//--------------------------
	void String::operator += (int i)
	{
		char ch[50];
		sprintf(ch, "%d", i);
		*this += ch;

	} // operator += int

	//------------------------------------------------------------------------------------------
	// Add a float to string
	//------------------------------------------------------------------------------------------
	void String::operator+=( float f )
	{
		char ch[32];
		sprintf(ch, "%f", f);
		*this += ch;

	} // operator += float

	void String::operator+=( const String& str )
	{
		for (u32 i = 0 ; i < str.GetLenght() ; i++)
			m_vArray->Add(str.m_vArray->Get(i));
	}

	//------------------
	// Check with char*.
	//------------------
	bool String::operator == (const char* str)
	{
		if (str)
		{
			const size_t len = m_vArray->Size();

			if (len != strlen(str))
				return false;

			for (u32 i = 0; i < len; i++)
			{
				if (str[i] != *(char*)m_vArray->Get(i)) return false;
			}

			return true;
		}

		return false;

	} // operator == char*

	//------------------------------------------------------------------------------------------
	// Check with kge::core::String.
	//------------------------------------------------------------------------------------------
	bool String::operator == (String& str)
	{
		return *this == str.ToCharPointer();

	} // operator == String

//------------------------------------------------------------------------------------------
	// Adds a character at the end of the string
	//------------------------------------------------------------------------------------------
	void String::Append(char c)
	{
		m_vArray->Add(&c);

	} // Append

	//------------------------------------------------------------------------------------------
	// Find other string inside this string
	//------------------------------------------------------------------------------------------
	int String::Find(const char* str, unsigned int StartIndex)
	{
		if (str)
		{
			u32 len  = (u32)strlen(str);        // str length
			u32 len2 = (u32)m_vArray->Size();    // This length

			if (StartIndex > len2 - 1)
				return -1;

			for (u32 i = StartIndex; i<len2; i++)
			{
				if (*(char*)m_vArray->Get(i) == str[0])
				{
					u32 j;
					for (j=0; j<len; j++)
					{
						if (i+j >= len2)
							break;

						if (*(char*)m_vArray->Get(i+j) != str[j])
						{
							break;
						} // if (m_vArray[i+j] != str[j])

					} // for j

					if (j == len)
					{
						return i;
					} // if (j == len - 1)

				} // if (m_vArray[i] == str[0])

			} // for i

		} // if str == something

		return -1;

	} // Find (char*)

	//------------------------------------------------------------------------------------------
	// Find other string inside this string
	//------------------------------------------------------------------------------------------
	int String::Find( const String* str, unsigned int StartIndex /*= 0*/ )
	{
		u32 len,	// str length
			len2;	// This length

		if (str)
		{
			len  = str->GetLenght();
			len2 = (u32)m_vArray->Size();

			if (StartIndex > len2 - 1)
				return -1;

			for (u32 i = StartIndex; i<len2; i++)
			{
				if (*(char*)m_vArray->Get(i) == *(char*)str->m_vArray->Get(0))
				{
					u32 j;
					for (j=0; j<len; j++)
					{
						if (i+j >= len2)
							break;

						if (*(char*)m_vArray->Get(i+j) != *(char*)str->m_vArray->Get(j))
						{
							break;
						} // if (m_vArray[i+j] != str[j])

					} // for j

					if (j == len)
					{
						return i;
					} // if (j == len - 1)

				} // if (m_vArray[i] == str[0])

			} // for i

		} // if str == something

		return -1;

	} // Find (String)

	//-----------------------------------
	// Peyda kardane ye matni dar string.
	//-----------------------------------
	int String::FindLast(const char* str)
	{
		u32 len,	// str length
			len2;	// This length

		if (str)
		{
			len  = (u32)strlen(str);
			len2 = (u32)m_vArray->Size();

			for (u32 i=len2 - 1; i<-1; i--)
			{
				if (*(char*)m_vArray->Get(i) == str[0])
				{
					u32 j;
					for (j=0; j<len; j++)
					{
						if (i+j >= len2)
							break;

						if (*(char*)m_vArray->Get(i+j) != str[j])
						{
							break;
						} // if (m_vArray[i+j] != str[j])

					} // for j

					if (j == len)
					{
						return i;
					} // if (j == len - 1)

				} // if (m_vArray[i] == str[0])

			} // for i

		} // if str == something

		return -1;

	} // Find Last

	//-----------------------------
	// Entekhabe ye tike az string.
	//-----------------------------
	bool String::Select(int StartIndex, int EndIndex, String& out)
	{
		// return if Indices not set
		u32 len = GetLenght();
		if (EndIndex < StartIndex
			|| StartIndex < 0
			|| EndIndex < 0
			|| StartIndex > (int)len - 1
			|| EndIndex > (int)len -1)
			return false;

		for (int i = StartIndex; i <= EndIndex; i++)
			out.m_vArray->Add(m_vArray->Get(i));

		return true;

	} // Select

	//------------------------------------------------------------------------------------
	// Splits this string into many strings separated by char.
	//------------------------------------------------------------------------------------
	String* String::Split( char ch, int &StringCount )
	{
		int i = 0, c = 0;
		std::vector<int> vPos;
		while (i >= 0)
		{
			i = Find(&ch, i + 1);
			if (i != -1)
			{
				vPos.push_back(i);
				printf("pos = %d\n", i);
			}
			c++;
		}
		StringCount = c;
		printf("count = %d\n", c);

		String* str = KGE_NEW_ARRAY(String, c);
		int s		= 0;
		const size_t size = vPos.size();
		for (i = 0; i < size; i++)
		{
			Select(s, vPos[i], str[i]);
			s = vPos[i] + 1;
		}
		if (s < GetLenght() - 1)
			Select(s, GetLenght() - 1, str[c - 1]);

		return str;

	} // Split

	//------------------------------------------------------------------------------------
	// Insert another string to this string at the specific location
	//------------------------------------------------------------------------------------
	bool String::Insert( const String* str, u32 at )
	{
// 		if (str)
// 		{
// 			if (at > GetLenght())
// 				return false;
// 			for (u32 i = 0; i < str->GetLenght(); ++i)
// 			{
// 				m_vArray.insert(m_vArray.begin() + (at + i), str->m_vArray[i]);
// 			}
// 			return true;
// 		}

		return false;

	} // Insert

	//------------------------------------------------------------------------------------
	// Copy the char* to char*
	//------------------------------------------------------------------------------------
	char* String::StringCopy( const char* Src )
	{
		if (!Src)
			return 0;

		size_t len = strlen(Src);
		char* ch = KGE_NEW_ARRAY(char ,len + 1);
		strcpy(ch, Src);
		ch[len] = 0;

		return ch;

	} // StringCopy

	//------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------
	void String::ToLower()
	{
//		std::transform(m_vArray.begin(), m_vArray.end(), m_vArray.begin(), ::tolower);
	}

	kge::u32 String::GetLenght() const
	{
		return m_vArray->Size();

	} // GetLenght

	//------------------------------------------------------------------------------------
	// Convert to char pointer
	//------------------------------------------------------------------------------------
	const char* String::ToCharPointer()
	{
		KGE_DELETE_ARRAY(m_pChar);
		m_pChar = KGE_NEW_ARRAY(char, m_vArray->Size() + 1);
		memcpy(m_pChar, m_vArray->GetData(), m_vArray->Size());
		m_pChar[m_vArray->Size()] = 0;

		return m_pChar;

	} // ToCharPointer

	//------------------------------------------------------------------------------------
	// Delete char* array
	//------------------------------------------------------------------------------------
	void String::DelCharPointer()
	{
		KGE_DELETE_ARRAY(m_pChar);

	} // DelCharPointer

	//------------------------------------------------------------------------------------
	// Get the char from string at specific position.
	//------------------------------------------------------------------------------------
	char String::operator[]( int index )
	{
		char* ch = (char*)m_vArray->Get(index);

		return *ch;

	} // operator[]

} // core

} // kge
