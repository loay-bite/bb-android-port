#include "squirrel_bridge.h"
#include <android/log.h>

#define LOG_TAG "BB_Squirrel"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Stub implementation for Squirrel 3 VM integration
SquirrelBridge::SquirrelBridge() : m_v(nullptr) {}

SquirrelBridge::~SquirrelBridge() {
    shutdown();
}

bool SquirrelBridge::initialize() {
    LOGI("Initializing Squirrel 3 Virtual Machine...");
    // In full implementation:
    // m_v = (void*)sq_open(1024);
    // sqstd_open((HSQUIRRELVM)m_v);
    return true;
}

bool SquirrelBridge::executeScript(const std::string& scriptPath) {
    LOGI("Executing script: %s", scriptPath.c_str());
    // In full implementation: load and compile .cnut bytecode or source via sq_compilebuffer
    return true;
}

void SquirrelBridge::shutdown() {
    if (m_v) {
        LOGI("Shutting down Squirrel 3 VM...");
        // sq_close((HSQUIRRELVM)m_v);
        m_v = nullptr;
    }
}
