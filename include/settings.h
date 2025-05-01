#pragma once
#include <string>
#include <unordered_map>

constexpr bool goofyEnPassant = false;
// Whether turn order can be skipped
constexpr bool freedom = false;
extern int stonkfish_depth; // defined in main.cpp
constexpr int max_stonkfish_calls = 5000000;
constexpr float stonkfish_randomization = 0.f; // 0.0005f;

// Starting position rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
// rnb1k3/pppBnpp1/8/4p3/4P3/8/PPPB1PPR/RN2K1N1 b Qq
const std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

constexpr unsigned int square_size {128};
constexpr float circle_scale {0.125};
const sf::Vector2u board_shift {square_size*3/4, square_size*3/4};

//const std::string project_dir = "C:/Users/rohan/OneDrive/Documents/GitHub/Chess/chess";
const std::string project_dir = "..";

// add castle/en passant flags, work with undo

const std::unordered_map<char, float> piece_weights = {
    {'p', 0}, // 1 to 2 squares, but not important to block
    {'n', 1.7}, // 3 to 4 squares
    {'b', 1},   // 5 to 8 squares
    {'r', 0.1}, // 3 squares in opening
    {'q', 0},   // 4 to 12 squares
    {'k', 0.1}    // no king development :((
};

/* iterative deepening
  * make moves vector
  * minimax(moves, depth 1), assign eval to each Move
  * sort moves by eval
  * minimax(moves, depth 2)
  * sort
  * depth 3
  * sort
*/
