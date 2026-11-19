#ifndef SQUIRREL_BRIDGE_H
#define SQUIRREL_BRIDGE_H

#include <string>

class SquirrelBridge {
public:
    SquirrelBridge();
    ~SquirrelBridge();

    bool initialize();
    bool executeScript(const std::string& scriptPath);
    void shutdown();

private:
    void* m_v; // HSQUIRRELVM
};

#endif // SQUIRREL_BRIDGE_H
