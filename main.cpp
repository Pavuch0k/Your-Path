#include <SFML/Graphics.hpp>
#include "tinyxml2.h"
#include <algorithm> // Добавляем заголовочный файл для функции std::min

class Hero {
private:
    float HeroX, HeroY;
    float HeroWidth, HeroHeight;
    sf::RectangleShape shape; // Форма героя (прямоугольник)
    sf::Texture texture; // Текстура спрайта героя
    float speed = 300.0f; // Начальная скорость героя (пикселей в секунду)
    sf::Clock clock; // Таймер для отслеживания времени
    float deltaTime; // Время, прошедшее с предыдущего кадра
    sf::Vector2u windowSize; // Размеры окна
    std::string spritePath; // Путь к спрайту героя

public:
    // Конструктор для инициализации героя из XML
    Hero(const sf::Vector2u& size) : windowSize(size) {
        const char* xmlFilePath = "XML/Hero.xml"; // Путь к XML файлу
        // Загрузка XML файла
        tinyxml2::XMLDocument doc;
        doc.LoadFile(xmlFilePath);

        // Получение данных о герое из XML файла
        tinyxml2::XMLElement* root = doc.FirstChildElement("Hero");
        root->FirstChildElement("Width")->QueryFloatText(&HeroWidth);
        root->FirstChildElement("Height")->QueryFloatText(&HeroHeight);
        spritePath = root->FirstChildElement("SpritePath")->GetText();

        // Установка начальной позиции героя по центру экрана
        HeroX = (windowSize.x - HeroWidth) / 2.0f;
        HeroY = (windowSize.y - HeroHeight) / 2.0f;

        // Загрузка текстуры спрайта героя
        if (!texture.loadFromFile(spritePath)) {
            // Обработка ошибки загрузки текстуры
            // Можно вывести сообщение об ошибке или применить альтернативный способ загрузки текстуры
        }

        // Применение текстуры к прямоугольнику формы героя
        shape.setTexture(&texture);

        // Установка размеров прямоугольника
        shape.setSize(sf::Vector2f(HeroWidth, HeroHeight));

        // Установка начальной позиции прямоугольника
        shape.setPosition(HeroX, HeroY);
    }

    // Метод для отрисовки героя
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    // Метод для обновления времени и позиции героя
    void update() {
        // Рассчитываем прошедшее время с предыдущего кадра
        deltaTime = clock.restart().asSeconds();

        // Перемещение героя на основе скорости и времени
        float distance = speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            HeroY = std::max(0.0f, HeroY - distance); // Проверяем, не вышел ли герой за верхнюю границу
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            HeroY = std::min(static_cast<float>(windowSize.y) - HeroHeight, HeroY + distance); // Проверяем, не вышел ли герой за нижнюю границу
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            HeroX = std::max(0.0f, HeroX - distance); // Проверяем, не вышел ли герой за левую границу
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            HeroX = std::min(static_cast<float>(windowSize.x) - HeroWidth, HeroX + distance); // Проверяем, не вышел ли герой за правую границу

        // Обновление позиции прямоугольника
        shape.setPosition(HeroX, HeroY);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Your Path", sf::Style::Fullscreen);
    Hero hero(window.getSize());

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        hero.update();
        window.clear();
        hero.draw(window);
        window.display();
    }

    return 0;
}
