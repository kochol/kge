#include "../include/kgedef.h"
#if KGE_PLATFORM == KGE_PLATFORM_ANDROID
#include <jni.h>
#include <dlfcn.h>
#include <unistd.h>
#include "../include/os.h"

#ifndef NULL
#define NULL 0
#endif

#	include <android/log.h>
#	define  LOG_TAG    "kge"
#	define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#	define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#	define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#	define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

namespace kge
{
	namespace os
	{
		// Sleep the thread
		void KGESleep(int miliSeconds)
		{
			usleep(miliSeconds * 1000);

		} // Sleep

	} // os

} // kge

extern "C"
{
void Java_com_kge_android_KGEAndroidActivity_AndroidMain
  (JNIEnv *env, jobject obj, jstring jStr)
{
	// Get the app name
	jboolean iscopy;
	const char *appname = env->GetStringUTFChars(jStr, &iscopy);

	// KGEmain function pointer
	typedef void (*KGEmain) ();

	bool bLoadFailed	= false;
	KGEmain pFn	= NULL;

	std::string strPluginName = "/data/data/com.kge.android/lib/lib";
	strPluginName += appname;
	strPluginName += ".so";

    void* handle = dlopen(strPluginName.c_str(), RTLD_LAZY);

    const char* error;

    if (!handle)
    {
			LOGE("can't load the %s plugin. (%s)", strPluginName.c_str(), dlerror());
			return;
    }
    // Clear any exiting error
    dlerror();

    // Load the KGEmain function
    pFn = (KGEmain)dlsym(handle, "KGEmain");
    if ((error = dlerror()) != NULL || !pFn)
    {
        LOGE("KGEmain function dose not find in %s plugin.\n%s", strPluginName.c_str(), error);
        return;
    }

	pFn();

} // Java_com_kge_android_KGEAndroidActivity_AndroidMain

} // extern "C"

#endif
