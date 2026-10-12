#pragma once

#include "cuicui_core/cuicui.h"



namespace cui {
class cuicui_biometricAuth
{
public: // private:

#ifdef CUICUI_ANDROID
	jobject biometricAuth_obj=nullptr; // move in  class cuicui_biometricAuth ?
	JNIEnv* biometricAuth_env=nullptr;
#endif

	std::function<void(int)> promptEvent = nullptr;

public:
	static cuicui_biometricAuth* GetInstance()
	{
		if (cuicui_biometricAuth_instance == nullptr)
			cuicui_biometricAuth_instance = new cuicui_biometricAuth();
		return cuicui_biometricAuth_instance;
	}

private:
	static cuicui_biometricAuth* cuicui_biometricAuth_instance;
	cuicui_biometricAuth()
	{
	}

public:
	bool PromptUser(std::function<void(int)> f);
	bool CheckReady(std::string& errorCode);
};


} // namespace cui