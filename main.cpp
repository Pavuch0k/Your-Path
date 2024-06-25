#include <SFML/Graphics.hpp>
#include "tinyxml2.h"
#include <algorithm> 

class Hero {
private:
    float HeroX, HeroY;
    float HeroWidth, HeroHeight;
    sf::RectangleShape shape; 
    sf::Texture texture; 
    float speed = 300.0f; 
    sf::Clock clock; 
    float deltaTime; 
    sf::Vector2u windowSize; 
    std::string spritePath; 

public:
  
    Hero(const sf::Vector2u& size) : windowSize(size) {
        const char* xmlFilePath = "XML/Hero.xml"; 
        // Çàãðóçêà XML ôàéëà
        tinyxml2::XMLDocument doc;
        doc.LoadFile(xmlFilePath);

        
        tinyxml2::XMLElement* root = doc.FirstChildElement("Hero");
        root->FirstChildElement("Width")->QueryFloatText(&HeroWidth);
        root->FirstChildElement("Height")->QueryFloatText(&HeroHeight);
        spritePath = root->FirstChildElement("SpritePath")->GetText();

       
        HeroX = (windowSize.x - HeroWidth) / 2.0f;
        HeroY = (windowSize.y - HeroHeight) / 2.0f;

        
        if (!texture.loadFromFile(spritePath)) {
          
        }

       
        shape.setTexture(&texture);

        
        shape.setSize(sf::Vector2f(HeroWidth, HeroHeight));

        
        shape.setPosition(HeroX, HeroY);
    }

  
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    
    void update() {
       
        deltaTime = clock.restart().asSeconds();

        
        float distance = speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            HeroY = std::max(0.0f, HeroY - distance);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            HeroY = std::min(static_cast<float>(windowSize.y) - HeroHeight, HeroY + distance);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            HeroX = std::max(0.0f, HeroX - distance); 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            HeroX = std::min(static_cast<float>(windowSize.x) - HeroWidth, HeroX + distance); 

        
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
