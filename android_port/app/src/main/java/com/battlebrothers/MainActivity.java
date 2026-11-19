package com.battlebrothers;

import android.app.NativeActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class MainActivity extends android.app.Activity {
    private static final String TAG = "BattleBrothers";

    // Load native libraries
    static {
        System.loadLibrary("SDL2");
        System.loadLibrary("battlebrothers");
    }

    private native int nativeInit();
    private native void nativeRender();
    private native void nativeResize(int width, int height);
    private native void nativeDestroy();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(TAG, "MainActivity created.");
        
        // Hide system UI for immersive tactical experience
        getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);

        nativeInit();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        nativeDestroy();
        Log.i(TAG, "MainActivity destroyed.");
    }
}
