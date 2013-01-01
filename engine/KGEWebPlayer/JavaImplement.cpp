#include "InputListener.h"
#include "Launcher.h"

#include "../include/Logger.h"
#include "../include/WebPlayer.h"
#include "InputMgrJava.h"

using namespace kge;

/*
* Global web player instance
*/
extern kge::web::WebPlayer	*	g_pWebPlayer;
extern kge::io::InputMgrJava*	g_pJavaInputMgr;

JNIEXPORT void JNICALL Java_Launcher_WebMain
  (JNIEnv *env, jobject obj)
{
		// RegisterPlugin function pointer
		typedef void (*WebMain) ();

		bool bLoadFailed	= false;
		WebMain pFn	= NULL;

		// Load KGEWebApp.dll files under windows OS
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS

		// Load dll and check for success
		HMODULE hMod = LoadLibrary(L"KGEWebApp.dll");

		if (hMod)
		{
			pFn = (WebMain)GetProcAddress(hMod, "WebMain");
			if (!pFn)
			{
				io::Logger::Error("WebMain function dose not find in KGEWebApp.dll.");
				return;
			}
		}
		else
		{
				io::Logger::Error("can't load the KGEWebApp.dll. (file not found)");
				return;
		}

		// Call the RegisterPlugin function
		(*pFn)();

#endif // KGE_PLATFORM == KGE_PLATFORM_WINDOWS
	
}

JNIEXPORT void JNICALL Java_Launcher_initialise
  (JNIEnv *env, jobject obj)
{
	g_pWebPlayer->Initialise();
}

JNIEXPORT jboolean JNICALL Java_Launcher_update
  (JNIEnv *env, jobject obj)
{
	return g_pWebPlayer->Update();
}

JNIEXPORT void JNICALL Java_Launcher_shutdown
  (JNIEnv *env, jobject obj)
{
	g_pWebPlayer->ShutDown();
}

JNIEXPORT void JNICALL Java_Launcher_setCanvas
  (JNIEnv *env, jobject obj, jint hwnd, jint width, jint height)
{
	g_pWebPlayer->SetCanvas(hwnd, width, height);
}

JNIEXPORT void JNICALL Java_Launcher_setResourcePath
  (JNIEnv *env, jobject obj, jstring jStr)
{
	jboolean iscopy;
	const char *mfile = env->GetStringUTFChars(jStr, &iscopy);
	g_pWebPlayer->SetResourcePath(mfile);
}

/*
 * Class:     InputListener
 * Method:    sendKeyPressed
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_InputListener_sendKeyPressed
  (JNIEnv *env, jobject obj, jint key)
{
	g_pJavaInputMgr->sendKeyPressed(key);
}

/*
 * Class:     InputListener
 * Method:    sendKeyReleased
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_InputListener_sendKeyReleased
  (JNIEnv *env, jobject obj, jint key)
{
	g_pJavaInputMgr->sendKeyReleased(key);
}

/*
 * Class:     InputListener
 * Method:    sendMouseMoved
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_InputListener_sendMouseMoved
  (JNIEnv *env, jobject obj, jint x, jint y)
{
	g_pJavaInputMgr->sendMouseMoved(x, y);
}

/*
 * Class:     InputListener
 * Method:    sendMouseScrolled
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_InputListener_sendMouseScrolled
  (JNIEnv *env, jobject obj, jint z)
{
}

/*
 * Class:     InputListener
 * Method:    sendMousePressed
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_InputListener_sendMousePressed
  (JNIEnv *env, jobject obj, jint id)
{
	g_pJavaInputMgr->sendMousePressed(id);
}

/*
 * Class:     InputListener
 * Method:    sendMouseReleased
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_InputListener_sendMouseReleased
  (JNIEnv *env, jobject obj, jint id)
{
	g_pJavaInputMgr->sendMouseReleased(id);
}
