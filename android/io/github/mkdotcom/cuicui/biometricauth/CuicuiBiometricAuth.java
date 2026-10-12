package io.github.mkdotcom.cuicui.biometricauth;


import android.Manifest;
import android.annotation.TargetApi;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.os.Build;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.biometric.BiometricManager;
import androidx.biometric.BiometricPrompt;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import androidx.fragment.app.FragmentActivity;

import java.util.concurrent.Executor;


@RequiresApi(api = Build.VERSION_CODES.P)
public class CuicuiBiometricAuth  {

    private Context context;
    private FragmentActivity ma; // the host activity (any FragmentActivity, e.g. CuicuiActivity)
    private Executor executor;
    private BiometricPrompt biometricPrompt;
    private BiometricPrompt.PromptInfo promptInfo;

    public int displayBiometricPrompt() {

        executor = ContextCompat.getMainExecutor(context);


        biometricPrompt = new BiometricPrompt(ma, executor, new BiometricPrompt.AuthenticationCallback() {
            @Override
            public void onAuthenticationError(int errorCode,
                                              @NonNull CharSequence errString) {
                super.onAuthenticationError(errorCode, errString);
                Log.d("CuicuiBiometricAuth", "Authentication error: " + errString);
                answer(-1);
            }

            @Override
            public void onAuthenticationSucceeded(
                    @NonNull BiometricPrompt.AuthenticationResult result) {
                super.onAuthenticationSucceeded(result);
                Log.d("CuicuiBiometricAuth", "Authentication succeeded!");
                answer(1);
            }

            @Override
            public void onAuthenticationFailed() {
                super.onAuthenticationFailed();
                Log.d("CuicuiBiometricAuth", "Authentication failed");
                answer(0);
            }
        });

        promptInfo = new BiometricPrompt.PromptInfo.Builder()
                .setTitle("Biometric login for my app")
                .setSubtitle("Log in using your biometric credential")
                .setNegativeButtonText("Use account password")
                .build();


        biometricPrompt.authenticate(promptInfo);


        return -1; // useless
    }

    public CuicuiBiometricAuth(Context c, FragmentActivity m)
    {
        context = c;
        ma = m;

        init();


    }


    public int isAvailable() {
        BiometricManager biometricManager = BiometricManager.from(context);
        switch (biometricManager.canAuthenticate()) {
            case BiometricManager.BIOMETRIC_SUCCESS:
                return 1;
            case BiometricManager.BIOMETRIC_ERROR_NO_HARDWARE:
                return -3;
            case BiometricManager.BIOMETRIC_ERROR_HW_UNAVAILABLE:
                return -4;
            case BiometricManager.BIOMETRIC_ERROR_NONE_ENROLLED:
                return -5;
        }

        return -5;
    }

    public native void init();
    public native void answer(int code);
}