# Battle Brothers Engine Technical Breakdown

## 1. Executive Summary & Engine Architecture
Battle Brothers is built on a proprietary, high-performance C++ game engine designed specifically for turn-based tactical RPGs. The architecture is split into three main layers:
- **Core C++ Engine**: Handles platform abstraction, windowing, file I/O (custom `.dat` archives containing graphics, audio, and scripts), memory management, math libraries, entity management, tactical grid calculations, and rendering loops.
- **Squirrel 3 Scripting VM (`sq`)**: Houses nearly all gameplay logic, AI behavior trees, UI event handling, state machine transitions (`root_state.cnut`, state stack), entity attributes, skills, contracts, and item definitions. The compiled bytecode scripts (`.cnut`) are executed dynamically by the embedded Squirrel virtual machine.
- **Coherent UI / HTML5 UI Bridge (Awesomium/Chromium derived)**: Renders the user interface (tactical HUD, inventory screens, towns, company management) using web technologies (HTML, CSS, JavaScript) running inside a browser viewport embedded in the engine.

---

## 2. DLL Dependencies & Runtime Libraries
Analysis of the Windows binary (`BattleBrothers.exe`) and shipped DLLs reveals the following dependencies:

### Core Game & UI Framework
- **`CoherentUIGT.dll` / `CoherentGTCore.dll` / `CoherentGTJS.dll`**: The core Coherent UI libraries providing HTML5/CSS UI rendering via Chromium/Blink architecture.
- **`RenoirCore.WindowsDesktop.dll`**: The hardware-accelerated rendering backend for Coherent UI (drawing DOM elements via DirectX/OpenGL).
- **`coherenticuin.dll` / `coherenticuuc.dll` / `icudt.dll` / `icudtcoherent53.dll`**: Internationalization and Unicode text layout engines used by Coherent UI.
- **`WTF.dll`**: WebKit Template Library / base utility library supporting Chromium/Coherent UI runtime.

### Audio & Services
- **`irrKlang.dll`**: 3D audio engine providing sound effect playback, positional audio in tactical combat, and ambient music (`.ogg` files).
- **`Galaxy.dll`**: GOG Galaxy integration for achievements, cloud saves, and DRM initialization.

### C++ Runtime & OS Stubs
- **`msvcp140.dll` / `vcruntime140.dll` / `ucrtbase.dll` / `concrt140.dll`**: Microsoft Visual C++ 2017 runtime libraries.
- **`api-ms-win-*` stubs**: Universal Windows CRT API redirection DLLs for Windows 7/8 compatibility.
- **`xinput9_1_0.dll`**: Controller / gamepad input support.

---

## 3. Squirrel (`sq_`) C++ Bindings & Scripting Architecture
The game heavily leverages **Squirrel 3** for game logic and data definition. 

### VM Initialization & Lifecycle
- The engine initializes the Squirrel VM (`sq_open(1024)`) during startup.
- Standard libraries (`sqstd_blib`, `sqstd_iolib`, `sqstd_mathlib`, `sqstd_strlib`, `sqstd_syslib`) are registered.
- Custom C++ class bindings and native functions are bound to the VM root table using `sq_registerclib` or manual stack operations (`sq_pushstring`, `sq_newclosure`, `sq_newslot`).

### Script Execution Flow
1. **Preload**: Executes `scripts/preload.cnut` to set up globals, constants, and core utility classes.
2. **Root State**: Initializes `scripts/root_state.cnut`, establishing the main game state machine (Title State, World State, Tactical Combat State, Town State).
3. **Hot-reload / Modding Support**: Scripts are loaded via virtual file system mounts from `.dat` archives or extracted directories (`extracted/scripts/`).

### Native Bindings (`sq_`)
- **Entity System**: C++ classes (`Entity`, `Actor`, `TacticalEntity`) expose methods to Squirrel for health, action points, armor, fatigue, and inventory.
- **Math & Grid**: Hexagonal grid coordinate systems (`createVec2`, `createVec3`) are bound for fast pathfinding and distance calculations.
- **UI Communication**: Native functions allow Squirrel to push events to the Coherent UI JavaScript bridge (`uimgr.call()`) and receive asynchronous callbacks from UI actions (e.g., clicking "End Turn" or buying an item).

---

## 4. OpenGL Calls & Rendering Pipeline
The native renderer utilizes hardware-accelerated graphics (OpenGL on cross-platform/Linux, DirectX on Windows) with custom shaders located in `extracted/shaders/`:

### Shader Ecosystem (`extracted/shaders/`)
- **`sprite.vs` / `sprite.fs`**: 2D sprite batching shader with color tinting and texture mapping for characters, tactical items, and map tiles.
- **`solid.vs` / `solid.fs`**: Solid color / primitive rendering for UI highlights, grid lines, and selection boxes.
- **`fow.vs` / `fow.fs`**: Fog of War rendering pipeline handling visibility masks and shadow overlays.
- **`water.vs` / `water.fs`**: Animated water shader with wave distortion and reflections.
- **`background.vs` / `background.fs`**: Parallax background and menu rendering.
- **`ui.vs` / `ui.fs`**: UI overlay rendering.
- **`sprite_dof.vs` / `sprite_dof.fs`**: Depth-of-field post-processing effects.

### Rendering Architecture
- Uses dynamic Vertex Buffer Objects (VBOs) and Vertex Array Objects (VAOs) for 2D batched sprite rendering.
- Framebuffer Objects (FBOs) are used for render-to-texture effects, Fog of War masking, and offscreen UI rendering.

---

## 5. Awesomium / Coherent UI Bridge & Android Port Strategy
The desktop version relies on Coherent UI (Chromium-based), which is heavy and difficult to run natively on Android with direct hardware acceleration without significant overhead.

### Porting Strategy for Android
1. **Headless UI / Embedded WebView or WebGL Canvas Renderer**:
   - Replace Coherent UI bindings with an embedded lightweight HTML5 renderer or embed Android WebView / Chromium WebView communicating via Javascript Bridge (`addJavascriptInterface`), OR render HTML5 UI via an offscreen WebGL canvas mapped to an OpenGL ES texture in SDL2.
2. **SDL2 Framework**:
   - Use SDL2 as the cross-platform application shell (`SDL_Window`, `SDL_Renderer`/OpenGL ES context, `SDL_Event` handling for touch and controller input).
3. **Squirrel 3 Embedding**:
   - Compile Squirrel 3 source code directly into the Android native library (`libbattlebrothers.so`) via CMake.
4. **Asset Loading**:
   - Mount extracted assets (`scripts/`, `gfx/`, `ui/`, `sounds/`, `music/`, `shaders/`) into the app's internal storage or APK asset manager for virtual file system access.
