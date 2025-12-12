#pragma once
#include <SFML/Graphics.hpp>
#include <string>

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