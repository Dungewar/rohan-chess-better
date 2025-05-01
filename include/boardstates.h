#pragma once
#include <SFML/Graphics.hpp>
#include <pieces.h>
#include <vector>
#include <stack>
#include <bitset>

class Bd {
    public:
    struct Move {
        sf::Vector2i start {};
        sf::Vector2i end {};
        char captured_piece = ' ';
        bool isCastle {};
        bool isEnPassant {};
        bool isPromotion {};
        std::bitset<6> castle_state;
        inline bool operator== (const Bd::Move& other) const {
            return start == other.start
             && end == other.end;
        }

        float eval;
    };

    struct MoveData {
        float eval {};
        std::stack<Move> moveStack {};
    };
    
    // attributes
    std::vector<std::vector<Piece*>> board {8, std::vector<Piece*>(8, nullptr)};
    bool isWhiteTurn {true};
    
    // (0) white king didn't move, (1) Q rook didn't move, (2) K rook didn't move, 
    // (3) black king didn't move, (4) q rook didn't move, (5) k rook didn't move, 
    std::bitset<6> castle_state {"111111"};
    sf::Vector2i ep_pawn {-1, -1};

    // constructors
    Bd (const std::string& fen) {
        loadFromFen(fen);
    };
    /*Bd (const Bd& copy) {
        board = copy.board;
        castle_state = copy.castle_state;
        ep_pawn = {copy.ep_pawn};
    }*/

    // methods

    /**
     * Retrieves all possible valid moves for a piece located at the specified starting position on the board.
     *
     * @param startPos The starting position of the piece on the board, represented as a 2D vector (x, y).
     * @param out A reference to a vector where all valid moves for the piece will be stored as Move objects.
     */
    void getMoves (const sf::Vector2i&, std::vector<Move>&) const;

    /**
     * Generates a complete list of all possible valid moves for the current state of the board,
     * considering the turn and the positions of all pieces.
     *
     * @param out A reference to a vector where all legal moves for the current board state will be stored as Move objects.
     */
    void getAllMoves (std::vector<Move>&) const;
    void loadFromFen (const std::string&);
    std::string getFen ();

    /**
     * Executes a move on the board based on the provided move object, updating the board state accordingly.
     *
     * @param m The Move object containing details about the move being executed, including starting position, ending position, and any special rules such as castling, en passant, or pawn promotion.
     */
    void makeMove (const Move&);
    void undoMove (const Move&);
    void print ();
    
    float static_eval () const;

    /**
     * Performs a minimax algorithm to evaluate the best possible move from the current board state.
     * Implements alpha-beta pruning to optimize the search process.
     *
     * @param depth The current depth of search, specifying how many moves ahead to evaluate.
     * @param alpha The alpha value used in alpha-beta pruning as the best already explored option along the maximizer path.
     * @param beta The beta value used in alpha-beta pruning as the best already explored option along the minimizer path.
     * @param moves A vector of pre-calculated possible moves for the current board state. If empty, all moves will be generated.
     * @param isFirstCall A boolean flag indicating whether this is the first call in the minimax process, used to store additional evaluation details.
     * @return A MoveData object containing evaluation results and the sequence of moves leading to the determined best move.
     */
    MoveData minimax (int depth, float alpha, float beta, std::vector<Move> moves={}, bool isFirstCall=true);
    void minimax2(int);

    void stonkfish ();

    inline bool operator== (const Bd& other) const { return board == other.board; }
};
