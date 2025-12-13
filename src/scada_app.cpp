#include "scada_app.hpp"
#include <fstream>
#include <vector>

std::string XMLParcer::getAttr(const std::string& line, const std::string& attr, bool& ok, int lineNum) {
    size_t pos = line.find(attr + "=\"");
    if (pos == std::string::npos) {
        std::cout << "Line " << lineNum << ": no attribute '" << attr << "' in button" << std::endl;
        ok = false;
        return "";
    }
    pos += attr.length() + 2;
    size_t end = line.find("\"", pos);
    return line.substr(pos, end - pos);
}

bool XMLParcer::getData(const std::string& filename, XMLData& data){
    // Открываем файл
    std::ifstream file("../config/" + filename);
    if (!file.is_open()) {
        std::cout << "Can not open the file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    int lineNum = 0;
    bool hasErrors = false;
    
    // Читаем файл построчно
    while (std::getline(file, line)) {
        lineNum++;
        
        // Убираем пробелы в начале
        size_t startPos = line.find_first_not_of(" \t");
        if (startPos != std::string::npos) {
            line = line.substr(startPos);
        }
        
        // Пропускаем пустые строки и комментарии
        if (line.empty() || line.find("<!--") == 0) {
            continue;
        }
        
        // === ПАРСИМ КНОПКУ ===
        if (line.find("<button") == 0) {
            Btn btn;
            bool ok = true;
            
            // Извлекаем атрибуты
            btn.name = getAttr(line,"name", ok, lineNum);
            if (btn.name.empty()) ok = false;
            
            // Парсим числа с проверкой
            try {
                btn.x = std::stoi(getAttr(line, "x", ok, lineNum));
                btn.y = std::stoi(getAttr(line, "y", ok, lineNum));
                btn.width = std::stoi(getAttr(line, "width", ok, lineNum));
                btn.height = std::stoi(getAttr(line, "height", ok, lineNum));
            } catch (const std::exception& e) {
                std::cerr << "Line " << lineNum << ": " << e.what() << std::endl;
                ok = false;
            }
            
            // Извлекаем текст кнопки (между > и <)
            size_t textStart = line.find(">");
            size_t textEnd = line.find("<", textStart + 1);
            if (textStart != std::string::npos && textEnd != std::string::npos) {
                btn.text = line.substr(textStart + 1, textEnd - textStart - 1);
            } else {
                btn.text = "";
            }
            
            // Проверяем размеры
            if (btn.width <= 0 || btn.height <= 0) {
                std::cout << "Line " << lineNum << ": invalid button`s size" << std::endl;
                ok = false;
            }
            
            if (ok) {
                data.buttons.push_back(btn);
                std::cout << "Button '" << btn.name << "' downloaded" << std::endl;
            } else {
                hasErrors = true;
            }
        }
        
        // === ПАРСИМ ПОЛЕ ВВОДА ===
        else if (line.find("<textfield") == 0) {
            Inpt inp;
            bool ok = true;
            
            inp.name = getAttr(line, "name", ok, lineNum);
            if (inp.name.empty()) ok = false;
            
            try {
                inp.x = std::stoi(getAttr(line, "x", ok, lineNum));
                inp.y = std::stoi(getAttr(line, "y", ok, lineNum));
                inp.width = std::stoi(getAttr(line, "width", ok, lineNum));
                inp.height = std::stoi(getAttr(line, "height", ok, lineNum));
            } catch (const std::exception& e) {
                std::cerr << "Line " << lineNum << ":" << e.what() << std::endl;
                ok = false;
            }
            
            if (inp.width <= 0 || inp.height <= 0) {
                std::cout << "Line" << lineNum << ": invalid input`s size" << std::endl;
                ok = false;
            }
            
            if (ok) {
                data.inputs.push_back(inp);
                std::cout << "Input '" << inp.name << "' downloaded" << std::endl;
            } else {
                hasErrors = true;
            }
        }
        
        // === ПАРСИМ ТЕКСТОВЫЙ ДИСПЛЕЙ ===
        else if (line.find("<textdisplay") == 0) {
            Otpt out;
            bool ok = true;
            
            out.name = getAttr(line, "name", ok, lineNum);
            if (out.name.empty()) ok = false;
            
            try {
                out.x = std::stoi(getAttr(line, "x", ok, lineNum));
                out.y = std::stoi(getAttr(line, "y", ok, lineNum));
                out.width = std::stoi(getAttr(line, "width", ok, lineNum));
                out.height = std::stoi(getAttr(line, "height", ok, lineNum));
            } catch (const std::exception& e) {
                std::cout << "Line " << lineNum << ": " << e.what() << std::endl;
                ok = false;
            }
            
            out.value = getAttr(line, "value", ok, lineNum);
            
            if (out.width <= 0 || out.height <= 0) {
                std::cout << "Line " << lineNum << ": invalid output`s size" << std::endl;
                ok = false;
            }
            
            if (ok) {
                data.outputs.push_back(out);
                std::cout << "Output '" << out.name << "' = " << out.value << std::endl;
            } else {
                hasErrors = true;
            }
        }
    }
    
    file.close();    
    
    // Итог
    std::cout << "\n=== RESULTS ===" << std::endl;
    std::cout << "Buttons: " << data.buttons.size() << std::endl;
    std::cout << "Inputs: " << data.inputs.size() << std::endl;
    std::cout << "Outputs: " << data.outputs.size() << std::endl;
    
    if (hasErrors) {
        std::cout << "Errors occured" << std::endl;
    }
    
    return true;
};