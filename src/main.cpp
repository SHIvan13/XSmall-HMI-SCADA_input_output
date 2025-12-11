// main.cpp - простой пример использования GUI виджетов
#include "scada_app.hpp"
#include <SFML/Graphics.hpp>

int main() {
    // Создание окна
    sf::RenderWindow window(sf::VideoMode(800, 600), "SCADA GUI Demo");
    window.setFramerateLimit(60);
    
    // Загрузка шрифта (создадим шрифт программно, если файл недоступен)
    sf::Font font;
    if (!font.loadFromFile("../src/arial.ttf")) {
        // Если шрифт не загружен, выводим предупреждение
        std::cerr << "Warning: Could not load arial.ttf" << std::endl;
        // Продолжаем выполнение
    }
    // if (!font.loadFromFile("../src/roboto.ttf")) {
    //     font.loadFromFile("../src/arial.ttf");  // запасной
    // }
    

    // Создание виджетов
    Button button1("Нажми меня", font);
    button1.setPosition(100, 100);
    button1.setSize(200, 60);
    button1.setVariableName("button1");
    
    // Настройка callback для кнопки
    button1.setCallback([]() {
        std::cout << "Кнопка нажата!" << std::endl;
    });
    
    TextField textField1("input1", font);
    textField1.setPosition(100, 200);
    textField1.setSize(200, 40);
    
    // Callback для текстового поля
    textField1.setOnChange([](const std::string& text) {
        std::cout << "Текст изменен: " << text << std::endl;
    });
    
    TextDisplay textDisplay1("display1", font);
    textDisplay1.setPosition(100, 300);
    textDisplay1.setSize(200, 40);
    textDisplay1.updateDisplay("25.5"); // Просто выводим значение
    
    
    // Основной цикл
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Получаем позицию мыши
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            
            // Обрабатываем события виджетов
            button1.handleEvent(event, mousePos);
            textField1.handleEvent(event, mousePos);
            textDisplay1.handleEvent(event, mousePos);
        }
        
        // Обновляем позицию мыши для виджетов
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        button1.update(mousePos.x, mousePos.y);
        textField1.update(mousePos.x, mousePos.y);
        textDisplay1.update(mousePos.x, mousePos.y);
        
        // Отрисовка
        window.clear(sf::Color(240, 240, 240)); // Светло-серый фон
        
        // Рисуем виджеты
        button1.draw(window);
        textField1.draw(window);
        textDisplay1.draw(window);
        
        window.display();
    }
    
    return 0;
}

// #include "scada_app.hpp"
// #include <memory>
// #include <vector>

// int main() {
    
//     // Создаем окно
//     sf::RenderWindow window(sf::VideoMode(1200, 800), "SCADA System from XML");
//     window.setFramerateLimit(60);
    
//     // Загружаем шрифт с поддержкой кириллицы
//     sf::Font font;
//     if (!font.loadFromFile("../src/roboto.ttf")) {
//         std::cerr << "Error: Failed to load font 'roboto.ttf'" << std::endl;
//         std::cerr << "Make sure the font file is in the same directory as the executable" << std::endl;
//         return 1;
//     }
    
//     // Загружаем данные из XML
//     XMLParcer parser;
//     XMLData data;
//     std::string xmlFile = "../config/config1.xml";
//     bool loaded = parser.getData(xmlFile, data);

//     // === ОТЛАДКА (что загрузилось?) ===
//     std::cout << "\n=== DEBUG ===" << std::endl;
//     std::cout << "XML loaded: " << (loaded ? "YES" : "NO") << std::endl;
//     std::cout << "Buttons count: " << data.buttons.size() << std::endl;
//     std::cout << "Inputs count: " << data.inputs.size() << std::endl;
//     std::cout << "Outputs count: " << data.outputs.size() << std::endl;

//     // Выводим детали
//     for (const auto& btn : data.buttons) {
//         std::cout << "Button: name='" << btn.name 
//                 << "' text='" << btn.text 
//                 << "' pos=" << btn.x << "," << btn.y << std::endl;
//     }
//     for (const auto& out : data.outputs) {
//         std::cout << "Display: name='" << out.name 
//                 << "' value='" << out.value 
//                 << "' unit='" << out.unit << "'" << std::endl;
//     }
//     std::cout << "=== END DEBUG ===\n" << std::endl;

//     if (!loaded) {
//         std::cerr << "Error: Failed to load XML file '" << xmlFile << "'" << std::endl;
//         return 1;
//     }
//     std::string xmlFile = "config1.xml";  // Используем наш XML файл
//     bool loaded = parser.getData(xmlFile, data);
    
//     if (!loaded) {
//         std::cerr << "Error: Failed to load XML file '" << xmlFile << "'" << std::endl;
//         return 1;
//     }
    
//     // Векторы для хранения виджетов
//     std::vector<std::unique_ptr<Button>> buttons;
//     std::vector<std::unique_ptr<TextField>> inputs;
//     std::vector<std::unique_ptr<TextDisplay>> outputs;
    
//     // 1. СОЗДАЕМ КНОПКИ из XML данных
//     for (const auto& btnData : data.buttons) {
//         auto button = std::make_unique<Button>(btnData.text, font);
//         button->setPosition(btnData.x, btnData.y);
//         button->setSize(btnData.width, btnData.height);
//         button->setVariableName(btnData.name);
        
//         // Настройка callback для кнопки
//         std::string btnName = btnData.name;
//         button->setCallback([btnName]() {
//             std::cout << "Кнопка '" << btnName << "' нажата!" << std::endl;
//         });
        
//         buttons.push_back(std::move(button));
//     }
    
//     // 2. СОЗДАЕМ ПОЛЯ ВВОДА из XML данных
//     for (const auto& inpData : data.inputs) {
//         auto input = std::make_unique<TextField>(inpData.name, font);
//         input->setPosition(inpData.x, inpData.y);
//         input->setSize(inpData.width, inpData.height);
//         input->setVariableName(inpData.name);
        
//         inputs.push_back(std::move(input));
//     }
    
//     // 3. СОЗДАЕМ ПОЛЯ ВЫВОДА из XML данных (ДИСПЛЕИ)
//     for (const auto& outData : data.outputs) {
//         auto output = std::make_unique<TextDisplay>(outData.name, font);
//         output->setPosition(outData.x, outData.y);
//         output->setSize(outData.width, outData.height);
//         output->setVariableName(outData.name);
        
//         // КЛЮЧЕВАЯ СТРОКА: устанавливаем значение из XML
//         output->setValue(outData.value + " " + outData.unit);
        
//         outputs.push_back(std::move(output));
//     }
    
//     // Главный цикл
//     while (window.isOpen()) {
//         sf::Event event;
        
//         // Обработка событий
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }
            
//             // Получаем позицию мыши
//             sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//             sf::Vector2f mousePosF(mousePos.x, mousePos.y);
            
//             // Передаем события всем виджетам
//             for (auto& btn : buttons) {
//                 btn->handleEvent(event, mousePosF);
//             }
            
//             for (auto& inp : inputs) {
//                 inp->handleEvent(event, mousePosF);
//             }
//         }
        
//         // Получаем текущую позицию мыши для обновления
//         sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
//         // Обновляем все виджеты
//         for (auto& btn : buttons) {
//             btn->update(mousePos.x, mousePos.y);
//         }
        
//         for (auto& inp : inputs) {
//             inp->update(mousePos.x, mousePos.y);
//         }
        
//         for (auto& out : outputs) {
//             out->update(mousePos.x, mousePos.y);
//         }
        
//         // Отрисовка
//         window.clear(sf::Color(240, 242, 245));  // Светло-серый фон
        
//         // Рисуем все виджеты
//         for (auto& btn : buttons) {
//             btn->draw(window);
//         }
        
//         for (auto& inp : inputs) {
//             inp->draw(window);
//         }
        
//         for (auto& out : outputs) {
//             out->draw(window);
//         }
        
//         // Показываем на экране
//         window.display();
//     }
    
//     return 0;
// }