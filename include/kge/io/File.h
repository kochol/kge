// File name: Keyboard.h
// Des: In class baraye kar ba file ha hast va mitone file ha ro az dakhele zip ham 
//		bekhone.
// Date: 26/6/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "../KgeUnknown.h"
#include "../kgedef.h"
#include "../core/String.h"

namespace kge
{
namespace io
{
class KGE_API File: public KgeUnknown
{
public:

	//! Sazande.
	File();

	//! Mokhareb.
	~File();

	//! Ye file ro baz mikone baraye khondan ya neveshtan age oon file vojod nadashte bashe misazash.
	//! \param FileName Esme file ee ke mikhahid baz shavad.
	//! \param CreateFile agra true bashad vaghti file vojod nadashte bashe misazash.
	//! \return agar movafagh shod true bar migardone.
	bool Open(const char* FileName, bool CreateFile = false);

	// TODO: badan age ehtiaj shod in ro ham ezafe mikonim.
	// Ye file ro az dakhel ye file zip mikkhone. Vali nemitone to file zip benevise.
	// \param ZipName Esme file zipi ke mikhahid baz shavad.
	// \param FileName Esme file ee ke mikhahid az dakhel file zip baz shavad.
	// \return agar movafagh shod true bar migardone.
	// bool Open(char* ZipName, char* FileName);

	//! Khondane ghesmati az file. pas az harbar khondan baghie file ro az jaee ke ta dafe ghabl khonde edame mide magar anke az seek estefade konid.
	//! \param Buffer Meghdari ke khande mishe dar inja zakhire mishe. 
	//! \param ReadSize Tedade byte hayee ke mikhahid az to file khande shavad.
	//! \return Tedad byte hayee ro ke khonde bar migardonad.
	int Read(void* Buffer, u32 ReadSize);

	int write(void* Buffer, u32 ReadSize);

	//! Check mikone ta bebine file amade estefade hast.
	//! \return Agar file ghablan baz shode bashad true mide.
	bool IsOpen();

	//! Hrekat kardan be noghte ye digari az file.
	//! \param pos Mahali ke bayad jabeja beshe.
	//! \param Current Agar flase bashe az aval file mire be noghte morde nazar va age ham ke true bashe az onjaee ke bode jabeja mishe.
	int Seek(u32 pos, bool Current = false);

	//! Bedast avadane size file e ke baz karde ed.
	//! \return Size file ra bar migardand.
	u32 GetSize();

	void Close();
	
	//! Check if the file exist
	static bool Exist(const char* cstrFileName);

protected:

	FILE* m_pFile;				// File ee ke baz shode.
	char* m_cFileName;			// File Name.

}; // File

} // io

} // kge
#endif // FILE_H