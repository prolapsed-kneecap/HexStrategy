#include <libs/SFML-2.6.1/Graphics.hpp>


class Cell {
public:
    char symbol;
protected:
    void setSymbol(char);

};

void Cell::setSymbol(char c) {
    this->symbol = c;
}



int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Application");
    sf::Texture texture;
    texture.loadFromFile("C:\\Users\\79635\\CLionProjects\\HexStrategy\\images\\hex_2.png");
    sf::Sprite hex;
    sf::Sprite hex1;
    sf::Sprite hex2;
    hex.setTexture(texture);
    hex1.setTexture(texture);
    hex2.setTexture(texture);
    hex.setPosition(0,0);
    hex1.setPosition(94, 0);
    hex2.setPosition(47, 28);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //asd

        window.clear(sf::Color::White);
        window.draw(hex);
        window.draw(hex1);
        window.draw(hex2);
        window.display();
    }
}