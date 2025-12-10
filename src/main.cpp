#include "scada_app.hpp"
#include <memory>
#include <vector>

int main() {
    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(1200, 800), "SCADA System from XML");
    window.setFramerateLimit(60);
    
    // Загружаем шрифт
    sf::Font font;
    if (!font.loadFromFile("../src/arial.ttf")) {
        std::cerr << "Error: Failed to load font 'arial.ttf'" << std::endl;
        std::cerr << "Make sure the font file is in the same directory as the executable" << std::endl;
        return 1;
    }
    
    // Создаем парсер и загружаем данные из XML
    XMLParcer parser;
    XMLData data;
    
    std::string xmlFile = "config1.xml";  // Имя XML файла
    bool loaded = parser.getData(xmlFile, data);
    
    if (!loaded) {
        std::cerr << "Error: Failed to load XML file '" << xmlFile << "'" << std::endl;
        std::cerr << "Creating default interface instead..." << std::endl;
        
        // Создаем тестовые данные если XML не загрузился
        data.buttons.push_back({"start_btn", 50, 50, 150, 60, "START"});
        data.buttons.push_back({"stop_btn", 50, 250, 150, 60, "STOP"});
        data.buttons.push_back({"reset_btn", 50, 450, 150, 60, "RESET"});
        
        data.inputs.push_back({"temp_input", 150, 50, 250, 40});
        data.inputs.push_back({"pressure_input", 150, 150, 250, 40});
        data.inputs.push_back({"speed_input", 150, 250, 250, 40});
        
        data.outputs.push_back({"temp_output", 300, 50, 300, 40, "25.5", "°C"});
        data.outputs.push_back({"pressure_output", 300, 150, 300, 40, "101.3", "kPa"});
        data.outputs.push_back({"speed_output", 300, 250, 300, 40, "1200", "RPM"});
    }
    
    // Выводим информацию о загруженных данных
    std::cout << "=== Loaded from XML ===" << std::endl;
    std::cout << "Buttons: " << data.buttons.size() << std::endl;
    std::cout << "Inputs: " << data.inputs.size() << std::endl;
    std::cout << "Outputs: " << data.outputs.size() << std::endl;
    
    // Векторы для хранения виджетов
    std::vector<std::unique_ptr<Button>> buttons;
    std::vector<std::unique_ptr<TextField>> inputs;
    std::vector<std::unique_ptr<TextDisplay>> outputs;
    
    // 1. СОЗДАЕМ КНОПКИ из XML данных
    for (const auto& btnData : data.buttons) {
        auto button = std::make_unique<Button>(btnData.text, font);
        button->setPosition(btnData.x, btnData.y);
        button->setSize(btnData.width, btnData.height);
        button->setVariableName(btnData.name);
        
        // Устанавливаем красный цвет для кнопок
        button->setColors(
            sf::Color(200, 60, 60),    // Красный (нормальный)
            sf::Color(230, 80, 80),    // Светлее при наведении
            sf::Color(170, 40, 40)     // Темнее при нажатии
        );
        
        // Устанавливаем callback для каждой кнопки
        std::string btnName = btnData.name;
        button->setCallback([btnName]() {
            std::cout << "Button '" << btnName << "' clicked!" << std::endl;
            
            // Разные действия для разных кнопок
            if (btnName == "start_btn") {
                std::cout << "Starting process..." << std::endl;
            }
            else if (btnName == "stop_btn") {
                std::cout << "Stopping process..." << std::endl;
            }
            else if (btnName == "reset_btn") {
                std::cout << "Resetting system..." << std::endl;
            }
        });
        
        buttons.push_back(std::move(button));
        std::cout << "Created button: " << btnData.name << std::endl;
    }
    
    // 2. СОЗДАЕМ ПОЛЯ ВВОДА из XML данных
    for (const auto& inpData : data.inputs) {
        auto input = std::make_unique<TextField>(inpData.name, font);
        input->setPosition(inpData.x, inpData.y);
        input->setSize(inpData.width, inpData.height);
        input->setVariableName(inpData.name);
        
        // Устанавливаем callback при изменении текста
        std::string inpName = inpData.name;
        input->setOnChange([inpName](const std::string& text) {
            std::cout << "Input '" << inpName << "' changed to: " << text << std::endl;
        });
        
        inputs.push_back(std::move(input));
        std::cout << "Created input: " << inpData.name << std::endl;
    }
    
    // 3. СОЗДАЕМ ПОЛЯ ВЫВОДА из XML данных
    for (const auto& outData : data.outputs) {
        auto output = std::make_unique<TextDisplay>(outData.name, font);   ///////// (outData.name, "{...}", font); ???
        output->setPosition(outData.x, outData.y);
        output->setSize(outData.width, outData.height);
        output->setVariableName(outData.name);
        output->updateDisplay(outData.value + " " + outData.unit);
        
        outputs.push_back(std::move(output));
        std::cout << "Created output: " << outData.name 
                  << " = " << outData.value << outData.unit << std::endl;
    }
    
    std::cout << "=== Interface created successfully ===" << std::endl;
    std::cout << "Total widgets: " 
              << (buttons.size() + inputs.size() + outputs.size()) 
              << std::endl;
    
    // Главный цикл
    while (window.isOpen()) {
        sf::Event event;
        
        // Обработка событий
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Получаем позицию мыши
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF(mousePos.x, mousePos.y);
            
            // Передаем события всем виджетам
            for (auto& btn : buttons) {
                btn->handleEvent(event, mousePosF);
            }
            
            for (auto& inp : inputs) {
                inp->handleEvent(event, mousePosF);
            }
            
            // Обновляем поля вывода значениями из полей ввода
            // (если количество совпадает)
            for (size_t i = 0; i < std::min(inputs.size(), outputs.size()); i++) {
                std::string inputText = inputs[i]->getText();
                if (!inputText.empty()) {
                    // Добавляем единицу измерения если она есть в XML
                    std::string unit = (i < data.outputs.size()) ? 
                                      " " + data.outputs[i].unit : "";
                    outputs[i]->updateDisplay(inputText + unit);
                }
            }
        }
        
        // Получаем текущую позицию мыши для обновления
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        // Обновляем все виджеты
        for (auto& btn : buttons) {
            btn->update(mousePos.x, mousePos.y);
        }
        
        for (auto& inp : inputs) {
            inp->update(mousePos.x, mousePos.y);
        }
        
        for (auto& out : outputs) {
            out->update(mousePos.x, mousePos.y);
        }
        
        // Отрисовка
        window.clear(sf::Color(240, 242, 245));  // Светло-серый фон
        
        // Рисуем все виджеты
        for (auto& btn : buttons) {
            btn->draw(window);
        }
        
        for (auto& inp : inputs) {
            inp->draw(window);
        }
        
        for (auto& out : outputs) {
            out->draw(window);
        }
        
        // Рисуем информационный текст
        sf::Text infoText;
        infoText.setFont(font);
        infoText.setCharacterSize(16);
        infoText.setFillColor(sf::Color(100, 100, 100));
        infoText.setString("SCADA System - Loaded from: " + xmlFile + 
                          " | Widgets: " + 
                          std::to_string(buttons.size() + inputs.size() + outputs.size()));
        infoText.setPosition(10, window.getSize().y - 30);
        window.draw(infoText);
        
        // Показываем на экране
        window.display();
    }
    
    return 0;
}