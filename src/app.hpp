#pragma once
#include "scada_app.hpp"

class App{
public:
    bool initialize();
    void run();
    void setXMLConfig(const std::string& filename);
private:
    bool loadResources();
    void createWidgets();
    void handleEvents();
    void update();
    void render();

    sf::RenderWindow window{sf::VideoMode(1900, 1000), L"SCADA"};
    sf::Font font;
    
    XMLParcer parser;
    XMLData data;
    
    std::vector<std::unique_ptr<Button>> buttons;
    std::vector<std::unique_ptr<TextField>> inputs;
    std::vector<std::unique_ptr<TextDisplay>> outputs;

    std::string XMLfile;


};