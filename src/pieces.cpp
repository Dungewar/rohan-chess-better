#include <SFML/Graphics.hpp>
#include <pieces.h>
#include <board_ui.h>
#include <settings.h>
#include <iostream>

sf::Vector2f getRealPos (float x, float y) {
    return std::move(sf::Vector2f {static_cast<float>(x * square_size + board_shift.x), 
    static_cast<float>(y * square_size + board_shift.y)});
}
sf::Vector2f getRealPos (const sf::Vector2i& boardPos) {
    return getRealPos(boardPos.x, boardPos.y);
}

void Square::init () {
    rect.setSize(sf::Vector2f(size, size));
    rect.setPosition(getRealPos(boardPos));
    if ((boardPos.x+boardPos.y) % 2 == 0)
        rect.setFillColor(sf::Color(240, 217, 181));
    else
        rect.setFillColor(sf::Color(181, 136, 99));
}

void Piece::init () {
    // set up filename
    // std::string filename = project_dir + "/assets/pieces/";
    // char lower = tolower(type);
    // if (white)
    //     filename += "w";
    // else
    //     filename += "b";
    //
    // filename += tolower(lower);
    // filename += ".png";
    
    texture = sf::Texture {getTexture(*this)};
    // // load textures
    // if (!texture.loadFromFile(filename)) {
    //     std::cout << "texture not loaded. it's joever" << std::endl;
    //     return;
    // }
    // texture.create(1024, 1024);
    
    texture.setSmooth(true);
    // sprite.setTexture(texture);
    
    // configure sprite
    sprite = sf::Sprite {texture};
    float pixels = static_cast<float>(texture.getSize().y);
    sprite.setScale(sf::Vector2f {square_size/pixels, square_size/pixels});
    this->resetPos();
}

void Piece::draw (sf::RenderWindow& window) {
    window.draw(sprite);
}

void Piece::resetPos() {
    // align realPos with boardPos
    realPos = getRealPos(boardPos);
    sprite.setPosition(realPos);
}

void Piece::moveToMouse(sf::Vector2f mousePos) {
    // move to mouse
    this->realPos.x = mousePos.x - square_size/2;
    this->realPos.y = mousePos.y - square_size/2;
    sprite.setPosition(realPos);
}
