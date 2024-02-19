#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

class Cell {
    private:
        std::string tp;
        int weight;
    public:
        Cell(std::string t) {
            tp = t;
            if (t == "void") {
                weight = 99999;
            }
            else if (t == "empty") {
                weight = 1;
            }
            else if (t == "forest") {
                weight = 2;
            }
        }
        std::string get_type() {
            return tp;
        }
        int get_weight() {
            return weight;
        }
};


int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Application");

    Cell field[4][5] = {
            {Cell("empty"), Cell("empty"), Cell("empty"), Cell("forest"), Cell("forest")},
            {Cell("empty"), Cell("void"), Cell("void"), Cell("void"), Cell("empty")},
            {Cell("empty"), Cell("empty"), Cell("forest"), Cell("empty"), Cell("empty")},
            {Cell("empty"), Cell("void"), Cell("forest"), Cell("void"), Cell("empty")}
    };
    int field_width = sizeof field[0] / sizeof(Cell), field_height = sizeof field / sizeof field[0];

    sf::Image image;
    image.loadFromFile("../images/hex_empty.png"); image.createMaskFromColor(sf::Color::White); sf::Texture hex_empty_texture; hex_empty_texture.loadFromImage(image);
    image.loadFromFile("../images/hex_forest.png"); image.createMaskFromColor(sf::Color::White); sf::Texture hex_forest_texture; hex_forest_texture.loadFromImage(image);

    sf::Sprite hex;

    std::vector<sf::Sprite> hex_sprites;
    for (int i = 0; i < field_height; i++) {
        for (int j = 0; j < field_width; j++) {
            std::string t = field[i][j].get_type();
            if (t == "void") continue;

            if (t == "empty") {
                hex.setTexture(hex_empty_texture);
            }
            else if (t == "forest") {
                hex.setTexture(hex_forest_texture);
            }

            float x = j * 208;
            float y = i * 206 * 3/4;

            if (i % 2 == 1) {
                x += 208 / 2;
            }
            hex.setPosition(x, y);

            hex_sprites.push_back(hex);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        for (int i = 0; i < hex_sprites.size(); i++) {
            window.draw(hex_sprites[i]);
        }

        window.display();
    }
}