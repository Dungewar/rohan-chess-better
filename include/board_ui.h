#pragma once
#include <SFML/Graphics.hpp>
#include <boardstates.h>

class bd;

namespace game {
    void init ();
    void tick (sf::RenderWindow&);
    void setPieceBoard (const bd&);
    
    // user events

    /**
     * Handles mouse press events in the game board. Determines interactions
     * with pieces on the board based on the given mouse position.
     *
     * @param mousePos The position of the mouse click in world coordinates.
     */
    void mousePressed (sf::Vector2f);

    /**
     * Handles mouse release events in the game board.
     * Interprets the release of a mouse button for interactions with the board.
     *
     * @param mousePos The position of the mouse release in world coordinates.
     */
    void mouseReleased(sf::Vector2f);

    /**
     * Flips the board's perspective.
     *
     * @param height The height of the window to help calculate the flip.
     */
    void flipBoard(int height);

    /**
     * Executes a move by the Stonkfish AI.
     * Triggers the AI to calculate and play its next move.
     */
    void stonkfishMove();

    /**
     * Prints the current state of the board.
     * Primarily used for debugging or logging purposes.
     */
    void printBoard();

    /**
     * Undoes the last move made on the board.
     * Restores the board to its previous state.
     */
    void undoMove();

    /**
     * Redoes the last undone move on the board.
     * Reapplies the most recently undone move to the board.
     */
    void redoMove();

    /**
     * Executes the first debug routine.
     * Provides a custom behavior for debugging purposes.
     */
    void debug();

    /**
     * Executes the second debug routine.
     * Provides an alternative custom behavior for debugging purposes.
     */
    void debug2();
}
