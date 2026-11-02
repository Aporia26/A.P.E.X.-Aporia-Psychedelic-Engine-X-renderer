#include "Engine/Engine.hpp"
#include <iostream>

// Standard main function for custom monolithic engine assembly
int main(int argc, char* argv[]) {
    std::cout << "APEX ENGINE [Subsystem Status]: Ignition key turned.\n";
    
    Apex::Engine app;

    if (app.Init()) {
        app.Run();
    }

    app.Shutdown();
    return 0;
}
