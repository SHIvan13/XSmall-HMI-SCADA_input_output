#include "scada_app.hpp"  

using std::endl;

// Функция загрузки данных из XML
XMLData loadInterfaceData(const std::string& filename = "config.xml") {
    XMLParcer parser;  
    XMLData data;      // Структура для хранения данных

    bool success = parser.getData(filename, data); // Парсинг структуры
    
    if (!success) {
        std::cerr << "Error loading file: " << filename << endl;
        return data;  // Возврат пустой структуру
    }
    std::cout << "=== Loaded from XML ===" << std::endl;
    std::cout << "Buttons: " << data.buttons.size() << std::endl;
    std::cout << "Inputs: " << data.inputs.size() << std::endl;
    std::cout << "Outputs: " << data.outputs.size() << std::endl;
    
    // Посмотреть, что именно загрузилось (для отладки?)
    // for (const auto& button : data.buttons) {
    //     std::cout << "Button: " << button.name 
    //               << " (" << button.text << ")"
    //               << " position: " << button.x << "," << button.y 
    //               << " size: " << button.width << "x" << button.height << std::endl;
    // }
    
    // for (const auto& input : data.inputs) {
    //     std::cout << "Input: " << input.name
    //               << " position: " << input.x << "," << input.y 
    //               << " size: " << input.width << "x" << input.height << std::endl;
    // }
    
    // for (const auto& output : data.outputs) {
    //     std::cout << "Output: " << output.name
    //               << " value: " << output.value << output.unit
    //               << " position: " << output.x << "," << output.y 
    //               << " size: " << output.width << "x" << output.height << std::endl;
    // }
    return data;  
}