# cuicui_biometricAuth

Biometric authentication (fingerprint / face) for [cuicui](https://github.com/mkdotcom/cuicui_core) apps.

> [!WARNING]
> **Status: functional on Android, used in a shipping app — API not stable.**
> Other platforms compile but report "not implemented" at runtime.

| Android | iOS | UWP | Web |
|---|---|---|---|
| ✅ (androidx.biometric) | ❌ not yet | ❌ | ❌ |

## Usage

```cpp
#include "modules/cuicui_biometricAuth/cuicui_biometricAuth.h"

std::string errorCode;
if (cuicui_biometricAuth::GetInstance()->CheckReady(errorCode))
{
    cuicui_biometricAuth::GetInstance()->PromptUser([](int result)
    {
        // 1 = success, 0 = failed attempt (the prompt stays open), -1 = error / cancelled
    });
}
else
{
    // errorCode: "not_implemented" on unsupported platforms, "not_init" if the Java side was not
    // created (Android), or an Android status code
    // (-3 no hardware, -4 hardware unavailable, -5 nothing enrolled)
}
```

The callback is called on the UI thread. Note that a failed attempt (`0`) may be followed by
a success (`1`) while the system prompt is still open.

## Setup

Clone the module into your project's `modules/` folder:

```
cd modules
git clone https://github.com/mkdotcom/cuicui_biometricAuth.git
```

### Android

1. Permission in `AndroidManifest.xml`:
   ```xml
   <uses-permission android:name="android.permission.USE_BIOMETRIC" />
   ```
2. In the app's `build.gradle`:
   ```gradle
   android {
       sourceSets {
           main.java.srcDirs += '../../../modules/cuicui_biometricAuth/android/'
       }
   }
   dependencies {
       implementation 'androidx.biometric:biometric:1.0.1'
   }
   ```
3. In `CMakeLists.txt`, add the C++ file to your native library:
   ```cmake
   ../../../modules/cuicui_biometricAuth/cuicui_biometricAuth.cpp
   ```
4. In your `MainActivity` (which extends `CuicuiActivity`), create the Java side once in
   `onCreate()`:
   ```java
   import io.github.mkdotcom.cuicui.biometricauth.CuicuiBiometricAuth;

   public CuicuiBiometricAuth cuicui_biometricAuth_instance = null;

   @Override
   protected void onCreate(Bundle savedInstanceState) {
       super.onCreate(savedInstanceState);
       if (cuicui_biometricAuth_instance == null)
           cuicui_biometricAuth_instance = new CuicuiBiometricAuth(this, this);
   }
   ```

### Other platforms
Add `cuicui_biometricAuth.cpp` to the project (Xcode, Visual Studio, `your_app_cpp_files.txt`
for the web) so that the app links; `CheckReady()` returns `false` with `"not_implemented"`.

## Known limitations
- The prompt texts are fixed (English) in `CuicuiBiometricAuth.java`.
- The result codes are plain integers / strings (a proper enum is planned).
- iOS (LocalAuthentication) and Windows Hello are not implemented yet.

## License
MIT — see [LICENSE](LICENSE).
