#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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

class FieldHandler {
    private:
        std::vector<Cell*> field;
        int rows, cols;
    public:
        FieldHandler(Cell *pf[], int r, int c) {
            for (int i = 0; i < r; i++) {
                field.push_back(pf[i]);
            }
            rows = r;
            cols = c;
        }

        std::vector<std::pair<int, int>> get_available_cells(int start_i, int start_j, int move_len) {
            std::vector<std::vector<int>> to_visit = {{start_i, start_j, move_len}};
            std::vector<std::pair<int, int>> visited;

            while (!to_visit.empty()) {
                int hex_i = to_visit[0][0];
                int hex_j = to_visit[0][1];
                int hex_mlen = to_visit[0][2];

                to_visit.erase(to_visit.begin());
                visited.push_back({hex_i, hex_j});

                std::pair<int, int> to_check[6] = {std::pair<int, int>(-1, 0), std::pair<int, int>(0, -1), std::pair<int, int>(1, 0), std::pair<int, int>(0, 1)};
                if (hex_i % 2 == 0) {
                    to_check[4] = std::pair<int, int>(-1, -1);
                    to_check[5] = std::pair<int, int>(1, -1);
                }
                else {
                    to_check[4] = std::pair<int, int>(-1, 1);
                    to_check[5] = std::pair<int, int>(1, 1);
                }

                for (auto p: to_check) {
                    int di = p.first, dj = p.second;

                    bool f = false;
                    for (auto v: to_visit) {
                        if (hex_i + di == v[0] && hex_j + dj == v[1]) {
                            f = true;
                            break;
                        }
                    }
                    if (f) continue;

                    if (0 <= hex_i + di && hex_i + di <= rows - 1 && 0 <= hex_j + dj && hex_j + dj <= cols - 1) {
                        if (std::find(visited.begin(), visited.end(), std::pair<int, int>(hex_i + di, hex_j + dj)) == visited.end()) {
                            int move_left = hex_mlen - field[hex_i + di][hex_j + dj].get_weight();
                            if (move_left >= 0) {
                                to_visit.push_back({hex_i + di, hex_j + dj, move_left});
                            }
                        }
                    }
                }
            }

            return visited;
        }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Application");
    window.setFramerateLimit(60);

    Cell field[4][5] = {
            {Cell("empty"), Cell("empty"), Cell("empty"), Cell("forest"), Cell("forest")},
            {Cell("empty"), Cell("void"), Cell("void"), Cell("void"), Cell("empty")},
            {Cell("empty"), Cell("empty"), Cell("forest"), Cell("empty"), Cell("empty")},
            {Cell("empty"), Cell("void"), Cell("forest"), Cell("void"), Cell("empty")}
    };
    int field_cols = sizeof field[0] / sizeof(Cell), field_rows = sizeof field / sizeof field[0];

    Cell *pfield[field_rows];
    for (int i = 0; i < field_rows; i++) {
        pfield[i] = field[i];
    }

    FieldHandler field_handler(pfield, field_rows, field_cols);

    std::vector<std::pair<int, int>> res = field_handler.get_available_cells(0, 0, 4);
    std::cout << std::endl;
    for (auto p: res) {
        std::cout << p.first << " " << p.second << std::endl;
    }

    sf::Image image;
    image.loadFromFile("../images/hex_empty.png"); image.createMaskFromColor(sf::Color::White); sf::Texture hex_empty_texture; hex_empty_texture.loadFromImage(image);
    image.loadFromFile("../images/hex_forest.png"); image.createMaskFromColor(sf::Color::White); sf::Texture hex_forest_texture; hex_forest_texture.loadFromImage(image);

    sf::Sprite hex;

    std::vector<sf::Sprite> hex_sprites;
    for (int i = 0; i < field_rows; i++) {
        for (int j = 0; j < field_cols; j++) {
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