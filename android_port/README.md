# Battle Brothers Android Port

This directory contains the Android NDK + SDL2 + Squirrel 3 C++ project structure for porting Battle Brothers to Android.

## Project Structure
- `app/src/main/cpp/`: C++ native source files (`native-lib.cpp`, `squirrel_bridge.cpp`, `squirrel_bridge.h`) bridging SDL2, OpenGL ES 3.0, and the Squirrel 3 scripting virtual machine.
- `app/src/main/java/`: Android Java/Kotlin wrapper (`MainActivity.java`) loading native `.so` libraries and setting up immersive fullscreen mode.
- `app/src/main/AndroidManifest.xml`: Android manifest configuring permissions, screen orientation (sensorLandscape), and OpenGL ES 3 requirements.
- `CMakeLists.txt`: CMake build configuration linking SDL2, OpenGL ES, EGL, and Android system libraries.

## Building Locally
1. Ensure Android Studio or Android NDK and SDK are installed.
2. Set `ANDROID_HOME` environment variable.
3. Build the project using Gradle:
   ```bash
   cd android_port
   ./gradlew assembleDebug
   ```
