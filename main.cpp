#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

int cur_id = 0;
int get_new_id() {
    return cur_id++;
}

std::map<std::string, sf::Texture> textures;
int hex_size = 200;

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

class Unit {
    private:
        int speed, atk_range, armor, damage, i, j, id;
        std::string cls, race;
        sf::Sprite sprite;
    public:
        Unit(std::string tp, int i, int j) {
            this->i = i;
            this->j = j;
            id = get_new_id();

            if (tp == "marine") {
                speed = 3;
                atk_range = 2;
                armor = 1;
                damage = 3;
                cls = "light";
                race = "human";
                sprite.setTexture(textures["marine"]);
            }

            sprite.setScale(100 / sprite.getLocalBounds().width, 100 / sprite.getLocalBounds().height);
            float x = j * hex_size + hex_size/4;
            float y = i * hex_size * 3/4 + hex_size/4;

            if (i % 2 == 1) {
                x += hex_size / 2;
            }
            sprite.setPosition(x, y);
        }

        int get_id() {
            return id;
        }

        sf::Sprite get_sprite(){
            return sprite;
        }
};

class GraphicsHandler {
    private:
        std::vector<std::pair<int, sf::Sprite>> units;
        std::vector<sf::Sprite> hex_sprites;
    public:
        GraphicsHandler(std::vector<sf::Sprite> hex_sprites) {
            this->hex_sprites = hex_sprites;
        }

        void add_unit(int id, sf::Sprite sprite) {
            units.push_back(std::pair<int, sf::Sprite>(id, sprite));
        }

        void del_unit(int id) {
            for (int i = 0; i < units.size(); i++) {
                if (units[i].first == id) {
                    units.erase(units.begin() + i);
                }
            }
        }

        void update(sf::RenderWindow *window) {
            (*window).clear(sf::Color::White);

            for (int i = 0; i < hex_sprites.size(); i++) {
                (*window).draw(hex_sprites[i]);
            }

            for (int i = 0; i < units.size(); i++) {
                (*window).draw(units[i].second);
            }

            (*window).display();
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

    /*
    std::vector<std::pair<int, int>> res = field_handler.get_available_cells(0, 0, 4);
    std::cout << std::endl;
    for (auto p: res) {
        std::cout << p.first << " " << p.second << std::endl;
    }
     */



    sf::Image image;
    sf::Texture texture;
    image.loadFromFile("../images/hex_empty.png"); image.createMaskFromColor(sf::Color::White); texture.loadFromImage(image); textures["hex_empty_texture"] = texture;
    image.loadFromFile("../images/hex_forest.png"); image.createMaskFromColor(sf::Color::White); texture.loadFromImage(image); textures["hex_forest_texture"] = texture;
    image.loadFromFile("../images/marine.png"); image.createMaskFromColor(sf::Color::White); texture.loadFromImage(image); textures["marine"] = texture;

    sf::Sprite hex;
    std::vector<sf::Sprite> hex_sprites;
    for (int i = 0; i < field_rows; i++) {
        for (int j = 0; j < field_cols; j++) {
            std::string t = field[i][j].get_type();
            if (t == "void") continue;

            if (t == "empty") {
                hex.setTexture(textures["hex_empty_texture"]);
            }
            else if (t == "forest") {
                hex.setTexture(textures["hex_forest_texture"]);
            }

            float x = j * hex_size;
            float y = i * hex_size * 3/4;

            if (i % 2 == 1) {
                x += hex_size / 2;
            }
            hex.setPosition(x, y);

            hex_sprites.push_back(hex);
        }
    }

    FieldHandler field_handler(pfield, field_rows, field_cols);
    GraphicsHandler graphics_handler(hex_sprites);

    std::vector<Unit> units = {Unit("marine", 0, 0), Unit("marine", 3, 0), Unit("marine", 1, 4)};

    for (int i = 0; i < units.size(); i++) {
        graphics_handler.add_unit(units[i].get_id(), units[i].get_sprite());
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        graphics_handler.update(&window);
    }
}