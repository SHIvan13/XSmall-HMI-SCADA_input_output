#include "scada_app.hpp"
#include <SFML/Graphics.hpp>
#include "app.hpp"

void App::setXMLConfig(const std::string& filename){
    XMLfile = filename;
}

bool App::initialize(){
    sf::RenderWindow window(sf::VideoMode(1900, 1000), L"SCADA");
    window.setFramerateLimit(60);

    // Загрузка ресурсов
    if (!loadResources()) {
        return false;
    }

     // Загрузка XML конфигурации
    if (!parser.getData(XMLfile, data)) {
        return false;
    }

     // Создание виджетов
    createWidgets();

    return true;
}

bool App::loadResources() {
    // Загрузка шрифта
    if (!font.loadFromFile("../resources/arial.ttf")) {
        std::cerr << "Ошибка: Не удалось загрузить шрифт." << std::endl;
        return false;
    }
    return true;
}

void App::createWidgets() {
    // 1. Создание кнопок из XML данных
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
    
    // 2. Создание полей ввода из XML данных
    for (const auto& inpData : data.inputs) {
        auto input = std::make_unique<TextField>(inpData.name, font);
        input->setPosition(inpData.x, inpData.y);
        input->setSize(inpData.width, inpData.height);
        input->setVariableName(inpData.name);
        
        inputs.push_back(std::move(input));
    }
    
    // 3. Создание полей вывода из XML данных
    for (const auto& outData : data.outputs) {
        auto output = std::make_unique<TextDisplay>(outData.name, outData.value, font);
        output->setPosition(outData.x, outData.y);
        output->setSize(outData.width, outData.height);
        
        outputs.push_back(std::move(output));
    }
    
    std::cout << "Виджеты созданы:" << std::endl;
    std::cout << "  Кнопки: " << buttons.size() << std::endl;
    std::cout << "  Поля ввода: " << inputs.size() << std::endl;
    std::cout << "  Поля вывода: " << outputs.size() << std::endl;
}

void App::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}


void App::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
        
        // Получаем позицию мыши
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(mousePos.x, mousePos.y);
        
        // Передаем события виджетам
        for (auto& btn : buttons) {
            btn->handleEvent(event, mousePosF);
        }
        
        for (auto& inp : inputs) {
            inp->handleEvent(event, mousePosF);
        }
    }
}

void App::update() {
    // Получаем текущую позицию мыши
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
}

void App::render() {
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
    
    window.display();
}