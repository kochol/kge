#include <stdio.h>
#include "../../engine/include/Logger.h"
#include "../../engine/include/FileSystemManager.h"
#include "../../engine/include/FileSystem.h"

#ifdef _DEBUG
#pragma comment(lib, "../../bin/debug/kge.lib")
#else
#pragma comment(lib, "../../bin/release/kge.lib")
#endif

int main()
{
	kge::io::Logger log;

	kge::io::FileSystem fs;
	kge::io::FileSystemManager* pFSM = kge::io::FileSystemManager::getSingletonPtr();
	pFSM->RegisterFileSystem(&fs);
	pFSM->AddSearchPath(L"e:\\");

	fs.CanLoad(L"ter1ps.txt");

	getchar();

} // main
