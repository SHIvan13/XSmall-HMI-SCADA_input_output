// main.cpp - простой пример использования GUI виджетов
#include "scada_app.hpp"
#include <SFML/Graphics.hpp>

int main() {
    
    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(1900, 1000), L"SCADA");
    window.setFramerateLimit(60);
    
    // Загружаем шрифт с поддержкой кириллицы
    sf::Font font;
    if (!font.loadFromFile("../resources/arial.ttf")) {
        std::cerr << "Make sure the font file is in the same directory as the executable" << std::endl;
        return 1;
    }
    
    // Загружаем данные из XML
    XMLParcer parser;
    XMLData data;
    std::string xmlFile = "config1.xml";
    bool loaded = parser.getData(xmlFile, data);

    if (!loaded) {
        std::cerr << "Error: Failed to load XML file '" << xmlFile << "'" << std::endl;
        return 1;
    }
    
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
        
        // Настройка callback для кнопки
        std::string btnName = btnData.name;
        button->setCallback([btnName]() {
            std::cout << "Кнопка '" << btnName << "' нажата!" << std::endl;
        });
        
        buttons.push_back(std::move(button));
    }
    
    // 2. СОЗДАЕМ ПОЛЯ ВВОДА из XML данных
    for (const auto& inpData : data.inputs) {
        auto input = std::make_unique<TextField>(inpData.name, font);
        input->setPosition(inpData.x, inpData.y);
        input->setSize(inpData.width, inpData.height);
        input->setVariableName(inpData.name);
        
        inputs.push_back(std::move(input));
    }
    
    // 3. СОЗДАЕМ ПОЛЯ ВЫВОДА из XML данных (ДИСПЛЕИ)
    for (const auto& outData : data.outputs) {
        auto output = std::make_unique<TextDisplay>(outData.name, outData.value, font);
        output->setPosition(outData.x, outData.y);
        output->setSize(outData.width, outData.height);

        outputs.push_back(std::move(output));
    }
    
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
        
        // Показываем на экране
        window.display();
    }
    
    return 0;
}