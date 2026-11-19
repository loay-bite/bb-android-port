#include <jni.h>
#include <SDL.h>
#include <GLES3/gl3.h>
#include <android/log.h>
#include "squirrel_bridge.h"

#define LOG_TAG "BattleBrothersNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static SquirrelBridge* g_squirrel = nullptr;
static SDL_Window* g_window = nullptr;
static SDL_GLContext g_glContext = nullptr;
static bool g_isRunning = false;

extern "C" {

JNIEXPORT jint JNICALL
Java_com_battlebrothers_MainActivity_nativeInit(JNIEnv* env, jobject thiz) {
    LOGI("Initializing Battle Brothers Engine on Android via SDL2...");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
        LOGE("SDL_Init failed: %s", SDL_GetError());
        return -1;
    }

    // Use OpenGL ES 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    g_window = SDL_CreateWindow("Battle Brothers Android",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                1280, 720,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);
    if (!g_window) {
        LOGE("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    g_glContext = SDL_GL_CreateContext(g_window);
    if (!g_glContext) {
        LOGE("SDL_GL_CreateContext failed: %s", SDL_GetError());
        SDL_DestroyWindow(g_window);
        SDL_Quit();
        return -1;
    }

    LOGI("OpenGL Vendor: %s", glGetString(GL_VENDOR));
    LOGI("OpenGL Renderer: %s", glGetString(GL_RENDERER));
    LOGI("OpenGL Version: %s", glGetString(GL_VERSION));

    // Initialize Squirrel VM
    g_squirrel = new SquirrelBridge();
    if (!g_squirrel->initialize()) {
        LOGE("Failed to initialize Squirrel VM");
    } else {
        g_squirrel->executeScript("scripts/root_state.cnut");
    }

    g_isRunning = true;
    return 0;
}

JNIEXPORT void JNICALL
Java_com_battlebrothers_MainActivity_nativeRender(JNIEnv* env, jobject thiz) {
    if (!g_isRunning) return;

    // Clear screen (dark tactical background)
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_SwapWindow(g_window);
}

JNIEXPORT void JNICALL
Java_com_battlebrothers_MainActivity_nativeResize(JNIEnv* env, jobject thiz, jint width, jint height) {
    LOGI("Window resized to %dx%d", width, height);
    glViewport(0, 0, width, height);
}

JNIEXPORT void JNICALL
Java_com_battlebrothers_MainActivity_nativeDestroy(JNIEnv* env, jobject thiz) {
    LOGI("Shutting down Battle Brothers Engine...");
    g_isRunning = false;

    if (g_squirrel) {
        delete g_squirrel;
        g_squirrel = nullptr;
    }

    if (g_glContext) {
        SDL_GL_DeleteContext(g_glContext);
        g_glContext = nullptr;
    }

    if (g_window) {
        SDL_DestroyWindow(g_window);
        g_window = nullptr;
    }

    SDL_Quit();
}

} // extern "C"
