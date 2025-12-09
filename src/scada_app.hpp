#pragma once
#include<string>
#include<fstream>
#include<vector>
#include<filesystem>
#include<iostream>
#include<algorithm>

struct Button
{
    std::string name;
    int y;
    int x;
    int width;
    int height;
    std::string text;
};
struct Input
{
    std::string name;
    int y;
    int x;
    int width;
    int height;
};
struct Output
{
    std::string name;
    int y;
    int x;
    int width;
    int height;
    std::string value;
    std::string unit;
};

struct XMLData {
    std::vector<Button> buttons;
    std::vector<Input> inputs;
    std::vector<Output> outputs;
};

class XMLParcer {
public:
    XMLParcer();
    bool getData(const std::string& filename, XMLData& data);  // Функция парсинга
    ~XMLParcer();

};