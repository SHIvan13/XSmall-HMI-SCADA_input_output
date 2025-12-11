#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <fstream>

struct Btn{
    std::string name;
    int y;
    int x;
    int width;
    int height;
    std::string text;
};
struct Inpt{
    std::string name;
    int y;
    int x;
    int width;
    int height;
};
struct Otpt{
    std::string name;
    int y;
    int x;
    int width;
    int height;
    std::string value;
};

struct XMLData {
    std::vector<Btn> buttons;
    std::vector<Inpt> inputs;
    std::vector<Otpt> outputs;
};

class XMLParcer {
public:
    XMLParcer();
    bool getData(const std::string& filename, XMLData& data);
    ~XMLParcer();
};

class Widget{
    public:
        Widget() = default;
        virtual ~Widget() = default;   // virtual - для правильного удаления наследников

        // virtual - наследники могут переопределить метод, если =0 - наследники должны реализовать 
        virtual void draw(sf::RenderTarget& target) const = 0;   // Отрисовка объекта
        virtual void update(float mouseX, float mouseY) = 0;     // Обновление состояния (60 раз/сек)
        virtual void handleEvent(const sf::Event& event,         // Обработка событий (клик, нажатие)
                                 const sf::Vector2f& mousePos) = 0;   
        virtual bool contains(float x, float y) const = 0;       // Проверка попадания мышки в ...
        
        // Наследники исп ф-ции, как есть
        void setPosition(float x, float y);            // Координаты
        void setSize(float w, float h); 
        void setVariableName(const std::string& name); // name - имя виджета в XML
        std::string getVariableName() const;           // Возвращает имя

    protected:
        sf::Vector2f position;    // Координаты виджета
        sf::Vector2f size;
        std::string variableName; // variableName - имя переменной 
        bool visible = true;      // Проверка состояния виджета (рисуется или нет)

};
class Button: public Widget{ 
    public:
        Button(const std::string& label, const sf::Font& font);
        void draw(sf::RenderTarget& target) const override;  // Рисует кнопку
        void update(float mouseX, float mouseY) override;    // Обновляет hover/цвета
        void handleEvent(const sf::Event& event, 
                         const sf::Vector2f& mousePos) override;
        bool contains(float x, float y) const override;      // Проверяет попадание
        
        // Свои методы Button
        void setColors(const sf::Color& normal, 
                       const sf::Color& hover,            // Обработка кликов (смена цвета при р. состояниях)
                       const sf::Color& pressed);   
        void setCallback(std::function<void()> callback); // Привязка ф-ции, которая вып. при клике
        void setText(const std::string& text);            // Меняет текст на кнопке
    private:
        sf::RectangleShape shape;
        sf::Text text;
        sf::Color normalColor, hoverColor, pressColor; // Цвет: Обычный, при наведении, при нажатии
        sf::RectangleShape nameLabel;                  
        sf::Text nameText; 
        bool isHovered = false;
        bool isPressed = false;
        std::function<void()> onClick;                 // Коробка для хранения функции (чтобы можно было ее вызвать)
};

class TextField: public Widget{
    public:
        TextField(const std::string& varName, const sf::Font& font);

        void draw(sf::RenderTarget& target) const override;      // Поле + текст
        void update(float mouseX, float mouseY) override;        // Мигание курсора
        void handleEvent(const sf::Event& event, 
                         const sf::Vector2f& mousePos) override; // Ввод с клавиатуры
        bool contains(float x, float y) const override;
        
        void setOnChange(std::function<void(const std::string&)> callback); // Привязка ф-ции, которая вызовется при изм текста
        void setText(const std::string& text);                              // Изм. текста на кнопке
        bool checkTextFits(const sf::String& textStr) const;                // НОВОЕ (2) Защита от выхода текста за границы, sf::String  
        std::string getText() const;                                        // для правильного хранения Русских символов (2 байта)
    private:
        sf::RectangleShape background;                    // Фон поля для текста
        sf::Text text;
        std::function<void(const std::string&)> onChange; // При изменении текста
        bool isFocused = false;                           // Активно ли поле для ввода
        sf::RectangleShape nameLabel;                 
        sf::Text nameText; 
};
class TextDisplay: public Widget{
    public:
        TextDisplay(const std::string& varName, const std::string& val, const sf::Font& font);
        void draw(sf::RenderTarget& target) const override;   // Рисует фон + текст
        void update(float mouseX, float mouseY) override;     // Центрирует текст
        void handleEvent(const sf::Event& event,              // Пустой, может пригодиться
                        const sf::Vector2f& mousePos) override;
        bool contains(float x, float y) const override;
        
        // Свои методы:
        void updateDisplay(const std::string& value);  // Обновляет значение для отображения
        void setValue(const std::string& newValue);     
        std::string getValue() const;                   // НОВОЕ
    private:
        sf::RectangleShape background; // Фон дисплея
        sf::Text text;
        std::string currentValue;      // Текущее значение
        sf::RectangleShape nameLabel;
        sf::Text nameText;                 
};