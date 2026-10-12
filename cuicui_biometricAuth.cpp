#include "cuicui_biometricAuth.h"
namespace cui {} using namespace cui; // for code outside the namespace (ObjC blocks, main)

namespace cui {
cuicui_biometricAuth* cuicui_biometricAuth::cuicui_biometricAuth_instance = nullptr;


bool cuicui_biometricAuth::PromptUser(std::function<void(int)> f)
{
	AssertOnUiThread();
	promptEvent = f;
#if defined(CUICUI_ANDROID)
	if (biometricAuth_obj == nullptr) return false;
	jclass clazz = globalCuiJniEnv->FindClass("io/github/mkdotcom/cuicui/biometricauth/CuicuiBiometricAuth");
	jmethodID method = globalCuiJniEnv->GetMethodID(clazz, "displayBiometricPrompt", "()I");
	globalCuiJniEnv->CallIntMethod(biometricAuth_obj, method);
	globalCuiJniEnv->DeleteLocalRef(clazz);
	return true;
#else
	// not available on this platform
	return false;
#endif
}

bool cuicui_biometricAuth::CheckReady(std::string& errorCode)
{
	AssertOnUiThread();

#if defined(CUICUI_ANDROID)
	if (biometricAuth_obj == nullptr) { errorCode = "not_init"; return false; }
	jclass clazz = globalCuiJniEnv->FindClass("io/github/mkdotcom/cuicui/biometricauth/CuicuiBiometricAuth");
	jmethodID method = globalCuiJniEnv->GetMethodID(clazz, "isAvailable", "()I");
	int ret = (int)globalCuiJniEnv->CallIntMethod(biometricAuth_obj, method);
	globalCuiJniEnv->DeleteLocalRef(clazz);
	if (ret == 1)
	{
		errorCode = "";
		return true;
	}
	else
	{
		errorCode = std::to_string(ret);
		return false;
	}
#elif defined(CUICUI_UWP)
	errorCode = "not_implemented";
	return false;
#elif defined(CUICUI_WEB)
	errorCode = "not_implemented";
	return false;
#elif defined(CUICUI_IOS)
	errorCode = "not_implemented";
	return false;
#elif defined(CUICUI_MACOS)
	errorCode = "not_implemented";
	return false;
#else
	non_implemented_platform
#endif
}



#ifdef CUICUI_ANDROID


extern "C"
JNIEXPORT void
JNICALL
Java_io_github_mkdotcom_cuicui_biometricauth_CuicuiBiometricAuth_init(
	JNIEnv * env,
	jobject obj)
{
	cuicui_biometricAuth::GetInstance()->biometricAuth_env = env; // useless if always==globalCuiJniEnv
	cuicui_biometricAuth::GetInstance()->biometricAuth_obj = env->NewGlobalRef(obj); // TODO: need to do "uninit" I presume (or no big deal, the user only does this once anyway?)

	return;
}

extern "C"
JNIEXPORT void
JNICALL
Java_io_github_mkdotcom_cuicui_biometricauth_CuicuiBiometricAuth_answer(
	JNIEnv * env,
	jobject obj,
	int code)
{
	if (cuicui_biometricAuth::GetInstance()->promptEvent != nullptr)
		cuicui_biometricAuth::GetInstance()->promptEvent(code);

	return;
}

#endif
} // namespace cui