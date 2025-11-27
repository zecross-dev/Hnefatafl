/**
 * @file main.cpp
 *
 * @brief Implementation of the Hnefatafl game.
 *
 * This file contains the implementation of the Hnefatafl game, including setup, gameplay, and outcome determination.
 * It also provides a testing suite for various game functions.
 *
 * @author JMB and zecross-dev- IUT Informatique La Rochelle
 * @date 10/11/2025
 */

#include <iostream>
#include "Headers/typeDef.h"
#include "Headers/functions.h"
#include "Headers/tests.h"

using namespace std;

/**
 * @brief Function to play the Hnefatafl game.
 *
 * This function orchestrates the Hnefatafl game, including setup, gameplay, and outcome determination.
 */
void playGame()
{
    displayHnefataflLogo();
    //size initialisation
    BoardSize gameBoardSize;
    //board initialisation
    Board gameBoard;
    bool validationSize = false;
    //Select valid size
    while ( validationSize == false ) {
         validationSize=chooseSizeBoard(gameBoardSize);
    }
    gameBoard.itsSize = gameBoardSize;
    bool validBoard;
    do {
        validBoard = createBoard(gameBoard);
    }while (validBoard != true);

    clearConsole();
    displayHnefataflLogo();
    initializeBoard(gameBoard);
    displayBoard(gameBoard);
    Position pos;
    getPositionFromInput(pos ,gameBoard);
    // Hint: Create a Game structure, configure terminal, get players, create board,
    // run game loop until finished, display winner, and clean up memory
}


/**
 * @brief Launches the testing suite.
 *
 * This function runs a suite of tests to validate various game functions.
 */
void launchTests(){
    // Configure terminal for colors and UTF-8 support (important on Windows)
    enableTerminalFormatting();

    // Configure test display settings
    // Set to (true, true) for debug mode with boards and prompts
    // Set to (false, false) for clean test output
    configureTestDisplay(false, false);

    // Display test suite header
    printTestSuiteHeader();

    // ─────────────────────────────────────────────────────────────────
    // Step 1: Board Setup and Management Tests
    // ─────────────────────────────────────────────────────────────────
    test_chooseSizeBoard();
    test_createBoard();
    test_deleteBoard();
    test_initializeBoard();

    // ─────────────────────────────────────────────────────────────────
    // Step 2: Position and Cell Validation Tests
    // ─────────────────────────────────────────────────────────────────
    test_getPositionFromInput();
    test_isValidPosition();
    test_isEmptyCell();

    // ─────────────────────────────────────────────────────────────────
    // Step 3: Movement and Action Tests
    // ─────────────────────────────────────────────────────────────────
    test_isValidMovement();
    test_movePiece();
    test_capturePieces();
    test_switchCurrentPlayer();

    // ─────────────────────────────────────────────────────────────────
    // Step 4: Game State and Victory Condition Tests
    // ─────────────────────────────────────────────────────────────────
    test_isSwordLeft();
    test_getKingPosition();
    test_isKingEscaped();
    test_isKingCapturedSimple();
    // test_isKingCapturedRecursive();  // Optional: Advanced recursive king capture detection
    test_isGameFinished();
    test_whoWon();

    // Display test suite footer
    printTestSuiteFooter();
}

/**
 * @brief Main function of the game.
 *
 * The main function serves as the entry point for the Hnefatafl game.
 * It can be used to start the game or run tests.
 *
 * @return 0 for successful execution.
 */
int main() {
    // Uncomment the line below to run tests
    launchTests();
    // Start the game
    playGame();

    return 0;
}
