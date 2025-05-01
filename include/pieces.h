#pragma once
#include <SFML/Graphics.hpp>
#include <board_ui.h>
#include <iostream>
#include <map>
#include <settings.h>

class Piece;

sf::Vector2f getRealPos (float, float);
sf::Vector2f getRealPos (const sf::Vector2i&);

enum class Type { Pawn, Rook, Knight, Bishop, Queen, King };



/**
 * @class Piece
 * @brief Represents a chess piece with its type, position, graphical representation, and interactions.
 *
 * This class models a chess piece, including its type, position on the board, and visual appearance
 * using SFML's Texture and Sprite components. It also supports functionality such as drawing
 * to a window, resetting and updating positions, and detecting interactions based on mouse input.
 */
class Piece {
private:
    static sf::Texture& getTexture (const Piece& piece) {
        struct PieceHash {
            Type type;
            bool white;
        };
        static std::pmr::map<PieceHash, sf::Texture> piece_textures;
        static bool texturesLoaded = false;

        if (!texturesLoaded) {
            // Load each texture only once
            if (!piece_textures[{Type::Pawn, true}].loadFromFile("assets/pieces/wp"))
                std::cout << "texture not loaded. it's joever" << std::endl;
            piece_textures[{Type::Rook, true}].loadFromFile("assets/pieces/wr");
            piece_textures[{Type::Knight, true}].loadFromFile("assets/pieces/wn");
            piece_textures[{Type::Bishop, true}].loadFromFile("assets/pieces/wb");
            piece_textures[{Type::Queen, true}].loadFromFile("assets/pieces/wq");
            piece_textures[{Type::King, true}].loadFromFile("assets/pieces/wk");

            piece_textures[{Type::Pawn, false}].loadFromFile("assets/pieces/bp");
            piece_textures[{Type::Rook, false}].loadFromFile("assets/pieces/br");
            piece_textures[{Type::Knight, false}].loadFromFile("assets/pieces/bn");
            piece_textures[{Type::Bishop, false}].loadFromFile("assets/pieces/bb");
            piece_textures[{Type::Queen, false}].loadFromFile("assets/pieces/bq");
            piece_textures[{Type::King, false}].loadFromFile("assets/pieces/bk");

            texturesLoaded = true;
        }
        return piece_textures.at({piece.type, piece.white});
    }

public:
    Type type{};
    bool white;
    bool toggled {};

    sf::Texture texture {};
    sf::Sprite sprite {texture};

    sf::Vector2<int> boardPos {};
    sf::Vector2f realPos {};



    // Constructor that takes in the type of piece and its position
    Piece (const Type& type, const sf::Vector2i& boardPos, const bool white):
    type {type}, white {white}, boardPos {boardPos} {
        this->init();
    }
    // Constructors that copies another piece
    Piece (const Piece& p):
    Piece(p.type, p.boardPos, p.white) {}

    /**
     * @brief Renders the chess piece onto the given window.
     *
     * Draws the graphical representation of the chess piece using its associated sprite
     * onto the provided SFML render window.
     *
     * @param window Reference to the SFML RenderWindow where the piece will be drawn.
     */
    void draw (sf::RenderWindow&);

    /**
     * @brief Resets the position of the chess piece based on its board position.
     *
     * Aligns the graphical representation (sprite) of the chess piece with its logical
     * position on the chessboard. Uses the board position to calculate its real position
     * in screen coordinates and updates the sprite's placement accordingly.
     *
     * This method is typically used to ensure the piece is correctly positioned after any
     * toggles, drag operations, or initialization routines.
     */
    void resetPos ();

    /**
     * @brief Initializes the chess piece's graphical representation and configuration.
     *
     * This method sets up the SFML texture and sprite for the chess piece based on its type.
     * It determines the correct texture file path, loads the texture, and configures its
     * appearance, including scale and smoothness. Additionally, it aligns the sprite's position
     * with the chessboard grid by invoking the resetPos method.
     */
    void init ();
    void moveToMouse (sf::Vector2f);
    inline bool containsPoint (sf::Vector2f mousePos) {
        return this->sprite.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y));
    }
};

class Square {
    public:
    const int size {};
    sf::Vector2i boardPos {};
    sf::RectangleShape rect {};
    
    Square (int x, int y, int s=square_size):
    boardPos {x, y}, size {s} {
        this->init();
    }
    Square (const Square& sq):
    boardPos {sq.boardPos}, size {sq.size} {
        this->init();
    }
    
    void init();
};
