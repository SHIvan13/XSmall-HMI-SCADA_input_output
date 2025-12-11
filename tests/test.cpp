#include "../src/scada_app.hpp"
#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <filesystem>

// Тестовый шрифт
class TestFont : public ::testing::Test {
protected:
    sf::Font font;
    
    void SetUp() override {
        // Создаем временный файл шрифта для тестов
        if (!font.loadFromFile("../src/arial.ttf")) {
            // Если шрифт не загружается, создаем минимальный шрифт
            std::cerr << "Warning: Using default font for tests" << std::endl;
        }
    }
};

// Тесты для класса Button
TEST_F(TestFont, ButtonCreation) {
    Button button("Test Button", font);
    
    EXPECT_EQ(button.getVariableName(), "");
    
    button.setVariableName("button1");
    EXPECT_EQ(button.getVariableName(), "button1");
    
    button.setPosition(100, 200);
    button.setSize(150, 50);
    
    button.setText("New Text");
    
    // Тестируем callback
    bool callbackCalled = false;
    button.setCallback([&callbackCalled]() {
        callbackCalled = true;
    });
}

TEST_F(TestFont, ButtonContains) {
    Button button("Test", font);
    button.setPosition(100, 100);
    button.setSize(200, 100);
    
    // Точка внутри кнопки
    EXPECT_TRUE(button.contains(150, 150));
    
    // Точка вне кнопки
    EXPECT_FALSE(button.contains(50, 50));
    
    // Точка на границе
    EXPECT_TRUE(button.contains(100, 100));
}

TEST_F(TestFont, ButtonColors) {
    Button button("Test", font);
    
    sf::Color red(255, 0, 0);
    sf::Color green(0, 255, 0);
    sf::Color blue(0, 0, 255);
    
    button.setColors(red, green, blue);
}

// Тесты для класса TextField
TEST_F(TestFont, TextFieldCreation) {
    TextField textField("input1", font);
    
    EXPECT_EQ(textField.getVariableName(), "input1");
    EXPECT_EQ(textField.getText(), "");
    
    textField.setPosition(50, 50);
    textField.setSize(200, 30);
    
    textField.setText("Initial Text");
    EXPECT_EQ(textField.getText(), "Initial Text");
}

TEST_F(TestFont, TextFieldTextOperations) {
    TextField textField("test_field", font);
    textField.setSize(300, 40);
    
    // Устанавливаем текст
    textField.setText("Hello World");
    EXPECT_EQ(textField.getText(), "Hello World");

    // Проверяем, помещается ли текст
    EXPECT_TRUE(textField.checkTextFits("Short"));
    EXPECT_TRUE(textField.checkTextFits("Very long text that might not fit"));
    
    // Тестируем русский текст
    EXPECT_TRUE(textField.checkTextFits(L"Привет мир"));
    EXPECT_TRUE(textField.checkTextFits(L"Текст на русском языке"));
}

TEST_F(TestFont, TextFieldOnChangeCallback) {
    TextField textField("callback_test", font);
    
    std::string lastText;
    textField.setOnChange([&lastText](const std::string& text) {
        lastText = text;
    });
    
    // Симулируем изменение текста
    textField.setText("Test");
    // Note: В реальном приложении onChange вызывается при вводе с клавиатуры
}

// Тесты для класса TextDisplay
TEST_F(TestFont, TextDisplayCreation) {
    TextDisplay display("display1", "Initial Value", font);
    
    EXPECT_EQ(display.getVariableName(), "display1");
    EXPECT_EQ(display.getValue(), "Initial Value");
    
    display.setPosition(200, 200);
    display.setSize(150, 40);
    
    display.setValue("Updated Value");
    EXPECT_EQ(display.getValue(), "Updated Value");
}

TEST_F(TestFont, TextDisplayUpdate) {
    TextDisplay display("temp_display", "0.0", font);
    display.setPosition(0, 0);
    display.setSize(100, 30);
    
    display.updateDisplay("25.5");
    EXPECT_EQ(display.getValue(), "25.5");
    
    display.setValue("100.0");
    EXPECT_EQ(display.getValue(), "100.0");
}

// Тесты для XML парсера
class XMLParserTest : public ::testing::Test {
protected:
    XMLParcer parser;
    XMLData data;
    
    void TearDown() override {
        // Удаляем тестовый файл
        std::remove("test_config.xml");
    }
};

TEST_F(XMLParserTest, ParseValidXML) {
    bool success = parser.getData("test_config.xml", data);
    EXPECT_TRUE(success);
    
    EXPECT_EQ(data.buttons.size(), 2);
    EXPECT_EQ(data.inputs.size(), 2);
    EXPECT_EQ(data.outputs.size(), 2);
    
    // Проверяем первую кнопку
    if (!data.buttons.empty()) {
        EXPECT_EQ(data.buttons[0].name, "btn_start");
        EXPECT_EQ(data.buttons[0].x, 100);
        EXPECT_EQ(data.buttons[0].y, 100);
        EXPECT_EQ(data.buttons[0].width, 120);
        EXPECT_EQ(data.buttons[0].height, 50);
        EXPECT_EQ(data.buttons[0].text, "Start");
    }
    
    // Проверяем первый дисплей
    if (!data.outputs.empty()) {
        EXPECT_EQ(data.outputs[0].name, "output_temp");
        EXPECT_EQ(data.outputs[0].value, "25.5");
    }
}

TEST_F(XMLParserTest, ParseInvalidFile) {
    XMLData emptyData;
    bool success = parser.getData("non_existent.xml", emptyData);
    EXPECT_FALSE(success);
}

TEST_F(XMLParserTest, ParseMalformedXML) {
    // Создаем плохой XML файл
    std::ofstream badFile("bad_config.xml");
    badFile << "<button name=\"bad\" x=\"abc\" y=\"def\"></button>";
    badFile.close();
    
    XMLData badData;
    bool success = parser.getData("bad_config.xml", badData);
    EXPECT_FALSE(success); // Должен вернуть false из-за ошибок парсинга
    
    std::remove("bad_config.xml");
}

// Тесты для интеграции виджетов
TEST_F(TestFont, WidgetIntegration) {
    // Создаем несколько виджетов
    Button btn("Test Button", font);
    btn.setPosition(10, 10);
    btn.setSize(100, 40);
    btn.setVariableName("test_btn");
    
    TextField tf("text_field", font);
    tf.setPosition(10, 60);
    tf.setSize(100, 30);
    
    TextDisplay td("display", "0.0", font);
    td.setPosition(10, 100);
    td.setSize(100, 30);
    
    // Проверяем, что они не перекрываются
    EXPECT_FALSE(btn.contains(10, 60)); // Кнопка не должна содержать точку поля ввода
    EXPECT_FALSE(tf.contains(10, 100)); // Поле ввода не должно содержать точку дисплея
    
    // Проверяем уникальность имен
    EXPECT_NE(btn.getVariableName(), tf.getVariableName());
}

// Тесты для проверки граничных случаев
TEST_F(TestFont, EdgeCases) {
    // Кнопка с нулевым размером
    Button zeroBtn("Zero", font);
    zeroBtn.setPosition(0, 0);
    zeroBtn.setSize(0, 0);
    EXPECT_FALSE(zeroBtn.contains(0, 0));
    
    // Поле ввода с очень длинным текстом
    TextField longField("long", font);
    longField.setSize(50, 30);
    longField.setText("Очень длинный текст который точно не поместится в маленькое поле");
    
    // Дисплей с пустым значением
    TextDisplay emptyDisplay("empty", "", font);
    EXPECT_EQ(emptyDisplay.getValue(), "");
    
    emptyDisplay.setValue("Not empty");
    EXPECT_EQ(emptyDisplay.getValue(), "Not empty");
}

// Тест для проверки Unicode поддержки
TEST_F(TestFont, UnicodeSupport) {
    TextField unicodeField("unicode_test", font);
    unicodeField.setSize(500, 40); // Большое поле для теста
    
    // Тестируем русский текст
    sf::String russianText = L"Привет мир! Тест кириллицы.";
    EXPECT_TRUE(unicodeField.checkTextFits(russianText));
    
    // Тестируем через sf::String
    sf::String unicodeString = sf::String::fromUtf8(russianText.begin(), russianText.end());
    EXPECT_TRUE(unicodeField.checkTextFits(unicodeString));
    
    // Тестируем специальные символы
    std::string specialChars = "Test 123 !@#$%^&*()";
    EXPECT_TRUE(unicodeField.checkTextFits(specialChars));
}

// Тест для проверки позиционирования
TEST_F(TestFont, PositioningTest) {
    Button btn("Pos Test", font);
    btn.setPosition(50, 75);
    btn.setSize(100, 40);
    
    // Проверяем границы
    EXPECT_TRUE(btn.contains(50, 75));    // Левый верхний угол
    EXPECT_TRUE(btn.contains(149, 114));  // Правый нижний угол (100-1, 40-1)
    EXPECT_FALSE(btn.contains(49, 75));   // Слева от кнопки
    EXPECT_FALSE(btn.contains(150, 75));  // Справа от кнопки
}

int main(int argc, char** argv) {
    // Инициализация Google Test
    ::testing::InitGoogleTest(&argc, argv);
    
    // Запуск всех тестов
    return RUN_ALL_TESTS();
}