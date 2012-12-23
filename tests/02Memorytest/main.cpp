#include "../../engine/include/KgeMemory.h"
#include "../../engine/include/Logger.h"
#include "../../engine/KGEmain/KgeMemoryTrack.h"
#include <stdio.h>

#ifdef _DEBUG
#pragma comment(lib, "../../bin/debug/kge.lib")
#else
#pragma comment(lib, "../../bin/release/kge.lib")
#endif

class test
{
public:
	test(int i) {kge::io::Logger::Debug("test constructor %d", i);}
	~test() {kge::io::Logger::Debug("test destructor");}
	int a,b,c,d;
};

int main()
{
	kge::io::Logger log;
	test* t = KGE_NEW(test)(5);
	test* r = KGE_NEW(test)(57);
	kge::core::KgeMemoryTrack::LogMemoryLeaks();
	KGE_DELETE(t, test);
	KGE_DELETE(r, test);
	kge::core::KgeMemoryTrack::LogMemoryLeaks();

	getchar();

	return 0;
}

extern "C"
{
void KGEmain()
{
	main();
}
}