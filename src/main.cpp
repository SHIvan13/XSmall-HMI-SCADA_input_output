#include "scada_app.hpp"
#include <SFML/Graphics.hpp>
#include <windows.h>
#include "app.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);  // UTF-8 для консоли
    SetConsoleCP(CP_UTF8);        // Для ввода тоже
    
    App app;

    app.setXMLConfig("config1.xml");
    
    if (!app.initialize()) {
        std::cerr << "Не удалось инициализировать приложение SCADA." << std::endl;
        return 1;
    }
    
    app.run();
    
    return 0;
}