#include "../../engine/include/KgeMemory.h"
#include "../../engine/include/WebPlayer.h"
#include "Test web application.h"

extern "C"
{
	KGE_API void WebMain()
	{
		kge::web::TestWebApp* pWebApp = new kge::web::TestWebApp();

		kge::web::WebPlayer* pWebPlayer = new kge::web::WebPlayer(pWebApp);

	} // WebMain

} // extern "C"
