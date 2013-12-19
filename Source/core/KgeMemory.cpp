/*
#include "../../Include/core/KgeMemory.h"
#include <list>

struct ALLOC_INFO 
{
	kge::ul32	address;
	kge::ul32	size;
	char	file[256];
	kge::ul32	line;
} ;

typedef std::list<ALLOC_INFO*> AllocList;

AllocList *allocList;

void AddTrack( kge::ul32 addr, kge::ul32 asize, const char *fname, kge::ul32 lnum )
{
	ALLOC_INFO *info;

	if(!allocList) {
		allocList = new(AllocList);
	}

	info = new(ALLOC_INFO);
	info->address = addr;
	strncpy(info->file, fname, 255);
	info->line = lnum;
	info->size = asize;
	allocList->insert(allocList->begin(), info);

}

void RemoveTrack( kge::ul32 addr )
{
	AllocList::iterator i;

	if(!allocList)
		return;
	for(i = allocList->begin(); i != allocList->end(); ++i)
	{
		if((*i)->address == addr)
		{
			allocList->remove((*i));
			break;
		}
	}

}

void DumpUnfreed()
{
	AllocList::iterator i;
	kge::ul32 totalSize = 0;
	char buf[1024];

	if(!allocList)
		return;

	FILE* f;
	fopen_s(&f, "mem.txt", "w");

	for(i = allocList->begin(); i != allocList->end(); ++i) {
		sprintf(buf, "%-50s:\t\tLINE %d,\t\tADDRESS %d\t%d unfreed\n",
			(*i)->file, (*i)->line, (*i)->address, (*i)->size);
		fprintf_s(f, buf);
		totalSize += (*i)->size;
	}
	sprintf(buf, "-----------------------------------------------------------\n");
	fprintf_s(f, buf);
	sprintf(buf, "Total Unfreed: %d bytes\n", totalSize);
	fprintf_s(f, buf);
	_fcloseall();

}
*/