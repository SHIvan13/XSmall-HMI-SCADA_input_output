#include "scada_app.hpp"  

using std::endl;
using std::cout;


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
    nameLabel.setOutlineColor(sf::Color::Black);  // Темно-серый обводка   НА ЧЕРНЫЙ поменял цвет
    nameLabel.setOutlineThickness(2.0f);
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
    text.setString(sf::String::fromUtf8(label.begin(), label.end()));
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
    // Позиция и размер прямоугольника кнопки
    shape.setPosition(position);          
    shape.setSize(size);

    //  Центрирование текста на кнопке
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width/2.0f,               // Привязка текста в центр
                   textBounds.top + textBounds.height/2.0f);
    text.setPosition(position.x + size.x/2.0f, position.y + size.y/2.0f); // Размещение текста в центре
    // Метка над кнопкой

    float nameHeight = 20.0f;
    nameLabel.setPosition(position.x, position.y - nameHeight);
    nameLabel.setSize(sf::Vector2f(size.x, nameHeight));

    // Центрирование текста в метке
    nameText.setString(sf::String::fromUtf8(variableName.begin(), variableName.end()));                                     // Имя виджета в текст
    sf::FloatRect nameBounds = nameText.getLocalBounds();                 // Размер текста
    nameText.setOrigin(nameBounds.left + nameBounds.width/2.0f,           // Центр по x  - Привязка точки в центр текста
                   nameBounds.top + nameBounds.height/2.0f);              // По y
    nameText.setPosition(position.x + size.x/2.0f,
                     position.y - nameHeight/2.0f);

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
    //  НГНННН
    while (text.getLocalBounds().width > size.x - 20 && text.getCharacterSize() > 10) {
        text.setCharacterSize(text.getCharacterSize() - 1);
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
    // return shape.getGlobalBounds().contains(x, y);
    // Учитывается метка имени
    float nameHeight = 20.0f;
    sf::FloatRect buttonBounds(position.x, position.y - nameHeight, 
                               size.x, size.y + nameHeight);
    return buttonBounds.contains(x, y);
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
    text.setString(sf::String::fromUtf8(newText.begin(), newText.end()));
}


// БЛОК 4 - TextField
TextField::TextField(const std::string& varName, const sf::Font& font) {
    variableName = varName;
    
    // Настройка метки имени (прямоугольничек сверху)
    nameLabel.setFillColor(sf::Color(60, 60, 60));        // Серый цвет
    nameLabel.setOutlineColor(sf::Color::Black);  // Темно-серый обводка
    nameLabel.setOutlineThickness(2.0f);
    // Настройка текста метки
    nameText.setFont(font);
    nameText.setCharacterSize(14);
    nameText.setFillColor(sf::Color::White);

    background.setFillColor(sf::Color::White);
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(2.0f);
    
    text.setFont(font);
    text.setString("");
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
}

void TextField::draw(sf::RenderTarget& target) const {
    if (!visible) return;
    target.draw(nameLabel);
    target.draw(nameText);

    target.draw(background);
    target.draw(text);
}

void TextField::update(float mouseX, float mouseY) {
    // Обновление позиции и размера
    background.setPosition(position);
    background.setSize(size);
    
    // Центрирование текста
    //text.setPosition(position.x + 5, position.y + (size.y - 20)/2);   // 5 - отступ слева, 20 - высота текста
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width/2.0f,
                   textBounds.top + textBounds.height/2.0f);
    text.setPosition(position.x + size.x/2.0f,
                     position.y + size.y/2.0f);
    // Обновление метки имени
    float nameHeight = 20.0f;
    nameLabel.setPosition(position.x, position.y - nameHeight);
    nameLabel.setSize(sf::Vector2f(size.x, nameHeight));
    
    nameText.setString(sf::String::fromUtf8(variableName.begin(), variableName.end()));
    sf::FloatRect nameBounds = nameText.getLocalBounds();
    nameText.setOrigin(nameBounds.left + nameBounds.width/2.0f,       // Точка привязки из sf::Transformable
                       nameBounds.top + nameBounds.height/2.0f);
    nameText.setPosition(position.x + size.x/2.0f,
                         position.y - nameHeight/2.0f);
    // Уменьшение текста, выходящего за границы
    while (text.getLocalBounds().width > size.x - 20 && text.getCharacterSize() > 10) {   // Если текст шире, чем поле с отступами и
        text.setCharacterSize(text.getCharacterSize() - 1);                               // размер  шрифтва больще 10px, то уменьшаем на 1px
    }
    // Увеличение шрифта, если текст влезает с запасом
    while (text.getLocalBounds().width < size.x - 40 && text.getCharacterSize() < 18) {
        text.setCharacterSize(text.getCharacterSize() + 1);
    }
}

void TextField::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) {
    if (!visible) return;
    sf::String current;
    
    if (event.type == sf::Event::MouseButtonPressed) {
        // Если кликнули по полю - фокусируем его
        if (contains(mousePos.x, mousePos.y)) {
            isFocused = true;
            background.setOutlineColor(sf::Color::Blue);  // Синяя рамка при фокусе
            std::cout << "TextField '" << variableName << "' focused" << std::endl;
        } 
        else {
            isFocused = false;
            background.setOutlineColor(sf::Color::Black);  // Черная рамка без фокуса
        }
    }
    else if (event.type == sf::Event::TextEntered && isFocused) {
        // Обработка ввода текста
        if (event.text.unicode == 8) {              // Backspace
            current = text.getString();
            if (!current.isEmpty()) {
                current.erase(current.getSize() - 1);        // НОВОЕ - Проверка наличия символов перед удалением
                text.setString(current);
            }
        }
        else if (event.text.unicode >= 32 ){
            current = text.getString();
        
            sf::String newText = current + event.text.unicode;  // Пробуем добавить символ

            // Через sf::String (для русского)
            if (!checkTextFits(newText)) {  // БЕЗ точки с запятой!
                return;  
            }
            // if (!checkTextFits((current + event.text.unicode).toAnsiString())) {
            //     return; 
            // }
            current += event.text.unicode;                   // НОВОЕ - sf::String преобразует число unicode в символ
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
    text.setString(sf::String::fromUtf8(newText.begin(), newText.end()));
}

std::string TextField::getText() const {
    return text.getString();
}

// Проверка для sf::String (для русского текста)                  // НОВОЕ!!!!
bool TextField::checkTextFits(const sf::String& textStr) const {
    if (size.x <= 0) return true;
    sf::Text tempText = text;                           // Временный текст для измерения
    tempText.setString(textStr);                        // Прямая установка sf::String
    sf::FloatRect bounds = tempText.getLocalBounds();   // Границы текста  
    float padding = 10.0f;                              // Отступы слева и справа
    return bounds.width <= (size.x - padding);          // Текст помещается, если его ширина меньше ширины поля минус отступы
}

// БЛОК 5 - TextDisplay 

TextDisplay::TextDisplay(const std::string& varName, const std::string& val, const sf::Font& font) {
    variableName = varName;
    currentValue = val;
       
     // Настройка метки имени (прямоугольничек сверху)
    nameLabel.setFillColor(sf::Color(60, 60, 60));        // Серый цвет
    nameLabel.setOutlineColor(sf::Color::Black); 
    nameLabel.setOutlineThickness(2.0f);
    
    // Настройка текста метки
    nameText.setFont(font);
    
    nameText.setCharacterSize(14);
    nameText.setFillColor(sf::Color::White);
    nameText.setString(sf::String::fromUtf8(variableName.begin(), variableName.end()));

    background.setFillColor(sf::Color::White);  
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(2.0f);
    
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
}

void TextDisplay::draw(sf::RenderTarget& target) const {
    if (!visible) return;
    target.draw(nameLabel);
    target.draw(nameText);

    target.draw(background);
    target.draw(text);
}

void TextDisplay::update(float mouseX, float mouseY) {
    text.setString(sf::String::fromUtf8(currentValue.begin(), currentValue.end()));  
    // Обновляем позицию и размер
    background.setPosition(position);
    background.setSize(size);
    
    // Центрируем текст
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width/2.0f,
                   textBounds.top + textBounds.height/2.0f);
    text.setPosition(position.x + size.x/2.0f,
                     position.y + size.y/2.0f);
    
    // Обновляем метку имени
    float nameHeight = 20.0f;
    nameLabel.setPosition(position.x, position.y - nameHeight);
    nameLabel.setSize(sf::Vector2f(size.x, nameHeight));
    
    nameText.setString(sf::String::fromUtf8(variableName.begin(), variableName.end()));
    sf::FloatRect nameBounds = nameText.getLocalBounds();
    nameText.setOrigin(nameBounds.left + nameBounds.width/2.0f,
                       nameBounds.top + nameBounds.height/2.0f);
    nameText.setPosition(position.x + size.x/2.0f,
                         position.y - nameHeight/2.0f);
}

void TextDisplay::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) {
    // Дисплей обычно не обрабатывает события, но метод нужен
}

bool TextDisplay::contains(float x, float y) const {
    return background.getGlobalBounds().contains(x, y);
}

void TextDisplay::updateDisplay(const std::string& newValue) {  // НОВОЕ - newValue вместо value                                         // НОВОЕ
    currentValue = newValue;
    text.setString(sf::String::fromUtf8(newValue.begin(), newValue.end()));
}


void TextDisplay::setValue(const std::string& newValue) {       // Внешнее изменение значения
    updateDisplay(newValue);
}

std::string TextDisplay::getValue() const {
    return currentValue;
}