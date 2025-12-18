#include "01_userInterfaceService/userInterface.hpp"
#include "mainControlUnit.hpp"

// ---------------------------------------------------------
// main
// ---------------------------------------------------------
int main() {
    MainControlUnit& mcu = MainControlUnit::instance();

    UserInterfaceService uiService;
    LogViewer logViewer;
    ConsoleUserInterface consoleUi(mcu, uiService, logViewer);

    consoleUi.run();
    return 0;
}
