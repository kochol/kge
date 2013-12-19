// File name: String.h
// Des: This is a small string class.
// Date: 9/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_STRING_H
#define KGE_STRING_H

#include <string>
#include "../KgeUnknown.h"
#include "../kgedef.h"

namespace kge
{
namespace core
{
	class KgeArray;
	typedef std::string stringc;
	typedef std::wstring stringw;

class KGE_API String: public KgeUnknown
{
public:

	//! Constructor
	String();

	//! Constructor
	String(const char* str);

	//! Destructor
	~String();

	//! operator = with char*
	void operator = (const char* str);

	//! copy operator
	void operator = (const String& str);
	
	//! Append a char* to string
	void operator += (const char* str);

	//! Append another string
	void operator += (const String& str);

	//! Add an integer to string
	void operator += (int i);

	//! Add a float to string
	void operator += (float f);

	//! Check with char*.
	bool operator == (const char* str);

	//! Check with kge::core::String.
	bool operator == (String& str);

	//! Get the char from string at specific position.
	char operator [] (int index);

	//! Find other string inside this string
	int Find(const char* str, unsigned int StartIndex = 0);

	//! Adds a character at the end of the string
	void Append(char c);

	//! Find other string inside this string
	int Find(const String* str, unsigned int StartIndex = 0);

	//! Find another string start at end of the string
	int FindLast(const char* str);

	//! Select a part of string
	bool Select(int StartIndex, int EndIndex, String& out);

	//! Splits this string into many strings separated by char.
	String* Split(char ch, int& StringCount);

	//! Insert another string to this string at the specific location
	bool Insert(const String* str, u32 at);

	// Convert to char pointer
	const char* ToCharPointer();

	//! Delete char* array
	void DelCharPointer();

	//! Get string lenght
	inline u32 GetLenght() const;

	//! A static function that copy from another char*
	static char* StringCopy(const char* Src);

	//! Unavailable TODO:
	void ToLower();

protected:
	KgeArray		*	m_vArray;
	KgeArray		*	m_nullTerminated;
	char			*	m_pChar;

}; // String

} // core

} // kge

#endif // STRING_H
