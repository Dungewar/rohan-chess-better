#include <board_ui.h>
#include <pieces.h>
#include <boardstates.h>
#include <vector>
#include <settings.h>
#include <iostream>
#include <algorithm>
namespace game {
    int numMoves = 0;

    // background
    sf::Font font;
    std::vector<Square> squares;
    std::vector<sf::Text> coords;
    // pieces
    bd current_board {starting_fen};
    std::vector<std::vector<Piece*>> piece_board {8, std::vector<Piece*>(8, {nullptr})};
    Piece* selected_piece = nullptr;
    
    std::vector<bd::Move> legalMoves {};
    std::vector<sf::CircleShape> legalMoveCircles {};

    std::vector<bd::Move> move_history;

    void init () {
        if (!font.openFromFile(project_dir + "/assets/fonts/roboto-mono.ttf")) {
            std::cout << "font not loaded: board_ui.cpp\n";
            return;
        }
        
        // 64 squares
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                squares.push_back(Square {x, y, square_size});
            }
        }

        // text
        for (int i = 0; i < 8; i++) {
            sf::Text letter {font, std::string{static_cast<char>('a' + i)}, 64};
            letter.setPosition(getRealPos(i+0.5, 8) + sf::Vector2f {-16, 0});
            coords.push_back(letter);

            sf::Text number {font, std::string{static_cast<char>('0' + 8-i)}, 64};
            number.setPosition(getRealPos(-0.5, i) + sf::Vector2f {0, 32});
            coords.push_back(number);
        }

        // board setup
        setPieceBoard(current_board);
    }
    void tick (sf::RenderWindow& window) {
        // checkerboard
        for (Square& sq : squares) {
            window.draw(sq.rect);
        }
        
        // pieces
        if (selected_piece && !selected_piece->toggled) {
            selected_piece -> moveToMouse(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        }
        for (std::vector<Piece*>& row : piece_board) {
            for (Piece* p : row) {
                if (!p) continue;
                p->draw(window);
            }
        }

        // coords
        for (sf::Text& t : coords) {
            window.draw(t);
        }

        // legal move circles
        for (sf::CircleShape& circle : legalMoveCircles) {
            window.draw(circle);
        }
    }
    void setPieceBoard (const bd& bs) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                const Piece* c = bs.board[x][y];

                // skip if pieces already match
                if (piece_board[x][y] && piece_board[x][y]->type == c->type)
                    continue;
                
                // skip both are blank
                if (!piece_board[x][y] && c == nullptr)
                    continue;

                // replace piece with c
                delete piece_board[x][y];
                if (c == nullptr) {
                    // or make it blank
                    piece_board[x][y] = nullptr;
                }
                else {
                    piece_board[x][y] = new Piece {*c};
                }
            }
        }
    }

    void mousePressed(sf::Vector2f mousePos) {
        if (selected_piece) {
            // simulates drag to mousePos, this is if you first click on a piece then click where to put it
            if (selected_piece->toggled) {
                mouseReleased(mousePos);
                return;
            }
        }

        // find the piece that was clicked
        for (std::vector<Piece*>& row : piece_board) {
            for (Piece* p : row) {
                if (p == nullptr) continue;
                if (p->containsPoint(mousePos)) {
                    // skip if not your turn
                    if (numMoves % 2 == p->white && !freedom) break;

                    // get legal moves
                    legalMoves.clear();
                    current_board.getMoves(p->boardPos, legalMoves);
                    
                    if (legalMoves.size() == 0) break;

                    selected_piece = p;

                    // add all legal moves to legalMoveCircles
                    for (bd::Move& move : legalMoves) {

                        sf::CircleShape circle {square_size * circle_scale};
                        circle.setFillColor(sf::Color{0, 80, 0, 150});
                        circle.setPosition(getRealPos(move.end.x + 0.5 - circle_scale, move.end.y + 0.5 - circle_scale));
                        
                        legalMoveCircles.push_back(circle);
                    }
                    break;
                }
            }
        }
    }

    void unselect () {
        if (selected_piece) {
            selected_piece->toggled = false;
            selected_piece->resetPos();
            selected_piece = nullptr;
        }
        legalMoves.clear();
        legalMoveCircles.clear();
    }
    
    void pushMove(const bd::Move& move) {
        // if there is more history and it clashes
        if (move_history.size() > numMoves) {
            if (!(move == move_history[numMoves])) {
                // clear future history
                while (move_history.size() > numMoves) {
                    move_history.pop_back();
                }
            }
        }
        if (numMoves == move_history.size()) {
            // push the new move
            move_history.push_back(move);
        }
        
        numMoves++;
        current_board.makeMove(move);
        setPieceBoard(current_board);

        selected_piece = nullptr;
    }
    
    void mouseReleased(sf::Vector2f mousePos) {
        if (selected_piece == nullptr) return;

        for (Square& sq : squares) {
            // find square that piece is being dropped on
            if (sq.rect.getGlobalBounds().contains(sf::Vector2f {mousePos.x, mousePos.y})) {
                // if legalMoves contains sq.boardPos
                auto it = std::find(legalMoves.begin(), legalMoves.end(), bd::Move {selected_piece->boardPos, sq.boardPos});
                if (it != legalMoves.end()) {
                    pushMove(*it);
                }

                // if released on same square as pressed, toggle
                else if (sq.boardPos == selected_piece->boardPos) {
                    selected_piece->toggled = !selected_piece->toggled;
                    selected_piece->resetPos();
                    // unselect if untoggled
                    if (!selected_piece->toggled) {
                        unselect();
                    }
                }
                // if released on a non-legal square
                else {
                    unselect();
                }
                break;
            }
        }
        // unselect if not toggling
        if (!(selected_piece && selected_piece->toggled))
            unselect();
    }
    void stonkfishMove () {
        unselect();
        bd::MoveData bestMove = current_board.minimax(stonkfish_depth, -1000, 1000);
        pushMove(bestMove.moveStack.top());
    }

    void undoMove() {
        if (move_history.size() == 0) return;
        if (numMoves == 0) return;
        
        current_board.undoMove(move_history[numMoves-1]);
        numMoves--;
        setPieceBoard(current_board);
        unselect();
    }
    void redoMove() {
        if (numMoves < move_history.size()) {
            pushMove(move_history[numMoves]);
        }
        unselect();
    }

    void flipBoard (int windowHeight) {
        // BS
        for (std::vector<Piece*>& row : piece_board) {
            for (Piece* p : row) {
                if (!p) continue;
                p->realPos.y = windowHeight - p->realPos.y - square_size;
            }
        }
        /*for (int i = 0; i < 8; i++) {
            int index = 2*i+1;
            coords[index].setPosition(
                coords[index].getPosition().x,
                windowHeight - coords[index].getPosition().y - square_size/2
            );
        }*/
    }

    void printBoard () {
        std::cout << "Move " << numMoves << " of " << move_history.size() << "\n";
        current_board.print();
    }

    void debug () {
        current_board.stonkfish();
    }

    void debug2 () {
        std::cout << "static_eval: " << current_board.static_eval() << std::endl;
    }
}
