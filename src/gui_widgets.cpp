#include "scada_app.hpp"  

using std::endl;
using std::cout;

// БЛОК 1 - Функция загрузки данных из XML
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

// БЛОК 2 - Widget
void Widget::setPosition(float x, float y){
    position.x = x;
    position.y = y;
}
void Widget::setSize(float w, float h) {
    size.x = w;
    size.y = h;
}
void Widget::setVariableName(const std::string& name) {
    variableName = name;
}
std::string Widget::getVariableName() const {
    return variableName;
}

// БЛОК 3 - Button 
// Переопределенные методы === СКРЫТЫЕ ВНУТРЕННИЕ ФУНКЦИИ ===
// Создание кнопки
Button::Button(const std::string& label, const sf::Font& font){  
    // Настройка метки имени (прямоугольничек сверху)
    nameLabel.setFillColor(sf::Color(60, 60, 60));        // Серый цвет
    nameLabel.setOutlineColor(sf::Color(100, 100, 100));  // Темно-серый обводка
    nameLabel.setOutlineThickness(1.0f);
    // Настройка текста метки
    nameText.setFont(font);
    nameText.setCharacterSize(14);
    nameText.setFillColor(sf::Color::White);    
    // Методы класса sf::RectangleShape
    shape.setFillColor(sf::Color(255, 165, 0)); 
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
    // Методы класса sf::Text
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    // Соб-ные переменные
    normalColor = sf::Color(255, 165, 0);     // Оранжевый
    hoverColor = sf::Color(255, 200, 100);    // Светло-оранжевый
    pressColor = sf::Color(255, 140, 0);      // Темно-оранжевый
}
// (1) Отрисовка виджета
void Button::draw(sf::RenderTarget& target) const {
    if (!visible) return;
    target.draw(shape);     // Рисовка прямоугольника
    target.draw(text);      // Текст на кнопке

    // Рисуем прямоугольничек с именем сверху
    target.draw(nameLabel);
    target.draw(nameText);
}
// (2) Наведение мыши
void Button::update(float mouseX, float mouseY){
    isHovered = contains(mouseX, mouseY); // Наведение мыши - проверка
    // Цвет в зависимости от состояния
    if (isPressed) {                      // При нажатии
        shape.setFillColor(pressColor);
    } 
    else if (isHovered) {                 // При наведении
        shape.setFillColor(hoverColor);
    } 
    else {
        shape.setFillColor(normalColor);
    }
}
// (3) Обработка событий кнопки (Клик, нажатие клавиши)
void Button::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) {
    if (!visible) return; // Видна ли кнопка
    // Если нажата левая кнопка мыши
    if (event.type == sf::Event::MouseButtonPressed) {       // sf::Event::MouseButtonPressed - нажатие
        if (event.mouseButton.button == sf::Mouse::Left) {  
            if (contains(mousePos.x, mousePos.y)) {          // Проверка попадания клика на кнопку
                isPressed = true;                            // Проверка нажатия школы
                cout << "Button " << variableName << " pressed" << endl;
            }
        }
    }
    // Если не нажата левая кнопка (отпустили)
    else if (event.type == sf::Event::MouseButtonReleased) { //sf::Event::MouseButtonReleased - закончена сессия
        if (event.mouseButton.button == sf::Mouse::Left) {   // Левая кнопка отпущена
            // Если кнопка была нажата и отпускание произошло над кнопкой
            if (isPressed && contains(mousePos.x, mousePos.y)) {
                // Вызывается callback, тк произошел клик
                if (onClick) {                               // Проверяем задан ли callback
                    onClick();                               // Вызываем функцию которую нам дали через setCallback()
                }
                cout << "Button " << variableName << " clicked" << endl;
            }
            isPressed = false;                               // Сбрасывание нажатия
        }
    }
}
// (4) Проверка попадения точки в границы кнопки
bool Button::contains(float x, float y) const {
    return shape.getGlobalBounds().contains(x, y);
}
// Свои методы Button === ЧТО МОЖНО НАСТРОИТЬ СНАРУЖИ ===
// Цвета для разных состояний
void Button::setColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& pressed) {
    normalColor = normal;
    hoverColor = hover;
    pressColor = pressed;
}
// Привязка функции, которая выполняется при клике
void Button::setCallback(std::function<void()> callback) {
    onClick = callback;
}
// Смена текста
void Button::setText(const std::string& newText) {
    text.setString(newText);
}


// БЛОК 4 - TextField
TextField::TextField(const std::string& varName, const sf::Font& font) {
    variableName = varName;
    
    // Настройка метки имени (прямоугольничек сверху)
    nameLabel.setFillColor(sf::Color(60, 60, 60));        // Серый цвет
    nameLabel.setOutlineColor(sf::Color(100, 100, 100));  // Темно-серый обводка
    nameLabel.setOutlineThickness(1.0f);
    // Настройка текста метки
    nameText.setFont(font);
    nameText.setCharacterSize(14);
    nameText.setFillColor(sf::Color::White);

    background.setFillColor(sf::Color::White);
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(1.0f);
    
    text.setFont(font);
    text.setString("");
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
}

void TextField::draw(sf::RenderTarget& target) const {
    if (!visible) return;
    target.draw(background);
    target.draw(text);
}

void TextField::update(float mouseX, float mouseY) {
    // Можно добавить мигание курсора если поле активно
}

void TextField::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) {
    if (!visible) return;
    
    if (event.type == sf::Event::MouseButtonPressed) {
        // Если кликнули по полю - фокусируем его
        if (contains(mousePos.x, mousePos.y)) {
            isFocused = true;
            background.setOutlineColor(sf::Color::Blue);  // Синяя рамка при фокусе
            std::cout << "TextField '" << variableName << "' focused" << std::endl;
        } else {
            isFocused = false;
            background.setOutlineColor(sf::Color::Black);  // Черная рамка без фокуса
        }
    }
    else if (event.type == sf::Event::TextEntered && isFocused) {
        // Обработка ввода текста
        if (event.text.unicode == 8) {  // Backspace
            std::string current = text.getString();
            if (!current.empty()) {
                current.pop_back();
                text.setString(current);
            }
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 128) {  // Печатные символы
            std::string current = text.getString();
            current += static_cast<char>(event.text.unicode);
            text.setString(current);
            
            // Вызываем callback если есть
            if (onChange) {
                onChange(current);
            }
        }
    }
}

bool TextField::contains(float x, float y) const {
    return background.getGlobalBounds().contains(x, y);
}

void TextField::setOnChange(std::function<void(const std::string&)> callback) {
    onChange = callback;
}

void TextField::setText(const std::string& newText) {
    text.setString(newText);
}

std::string TextField::getText() const {
    return text.getString();
}

// БЛОК 5 - TextDisplay 

TextDisplay::TextDisplay(const std::string& varName, const sf::Font& font) {
    variableName = varName;
    currentValue = "";
     // Настройка метки имени (прямоугольничек сверху)
    nameLabel.setFillColor(sf::Color(60, 60, 60));        // Серый цвет
    nameLabel.setOutlineColor(sf::Color(100, 100, 100));  // Темно-серый обводка
    nameLabel.setOutlineThickness(1.0f);
    
    // Настройка текста метки
    nameText.setFont(font);
    nameText.setCharacterSize(14);
    nameText.setFillColor(sf::Color::White);

    background.setFillColor(sf::Color(230, 230, 230));  // Светло-серый фон
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(1.0f);
    
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
    
    // Устанавливаем начальный текст
    updateDisplay("");
}

void TextDisplay::draw(sf::RenderTarget& target) const {
    if (!visible) return;
    target.draw(background);
    target.draw(text);
}

void TextDisplay::update(float mouseX, float mouseY) {
    // Центрируем текст
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        position.x + (size.x - textBounds.width) / 2,
        position.y + (size.y - textBounds.height) / 2 - textBounds.top
    );
}

void TextDisplay::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) {
    // Дисплей обычно не обрабатывает события, но метод нужен
}

bool TextDisplay::contains(float x, float y) const {
    return background.getGlobalBounds().contains(x, y);
}

void TextDisplay::updateDisplay(const std::string& value) {
    currentValue = value;
    
    // Форматируем текст: заменяем {...} на значение
    std::string displayText = format;
    size_t pos = displayText.find("{...}");
    if (pos != std::string::npos) {
        displayText.replace(pos, 5, value);
    }
    
    text.setString(displayText);
}

void TextDisplay::setFormat(const std::string& fmt) {
    format = fmt;
    updateDisplay(currentValue);  // Обновляем с новым форматом
}