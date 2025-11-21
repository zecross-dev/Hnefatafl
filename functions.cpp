/**
 * @file functions.cpp
 *
 * @brief Implementation of functions for the Hnefatafl game.
 *
 * This file contains the implementation of various functions used in the Hnefatafl game, including
 * board initialization, display, piece movement, capturing, and game state checks.
 *
 * @author JMB - IUT Informatique La Rochelle
 * @date 10/11/2025
 */

#include <iostream>
#include "typeDef.h"
#include "functions.h"


#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ============================================================================
// SECTION 1: TERMINAL & DISPLAY FUNCTIONS
// ============================================================================

/**
 * @brief Enables terminal formatting support for colors and UTF-8 characters.
 *
 * Configures the terminal to display ANSI color codes and UTF-8 characters.
 * On Windows, activates UTF-8 encoding and ANSI escape sequences.
 * On Linux/macOS, no configuration needed (enabled by default).
 *
 * @return `true` if successful, `false` if an error occurred.
 * @note The game will still work if this fails, but without colors and special characters.
 */
bool enableTerminalFormatting() {
#ifdef _WIN32
    // === STEP 1: Enable UTF-8 output on Windows ===
    // Changes console code page to 65001 (UTF-8)
    // This allows displaying special characters like ⛒ (king), ⏺ (sword), ○ (shield)
    int result = system("chcp 65001 > nul");
    if (result != 0) {
        cerr << "Warning: Failed to set UTF-8 code page (chcp 65001)." << endl;
        // Continue anyway - not critical, game can still run
    }
    
    // === STEP 2: Enable ANSI color codes on Windows 10+ ===
    // This allows using ANSI escape sequences for colored text output
    
    // Get the console output handle
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        cerr << "Error: Failed to get console output handle." << endl;
        cout << "Warning: Terminal formatting could not be enabled." << endl;
        cout << "Colors and special characters may not display correctly." << endl;
        return false;
    }
    
    // Get the current console mode
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        cerr << "Error: Failed to get console mode." << endl;
        cout << "Warning: Terminal formatting could not be enabled." << endl;
        cout << "Colors and special characters may not display correctly." << endl;
        return false;
    }
    
    // Enable virtual terminal processing (ANSI escape sequences)
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) {
        cerr << "Error: Failed to enable ANSI escape sequences (virtual terminal processing)." << endl;
        cout << "Warning: Terminal formatting could not be enabled." << endl;
        cout << "Colors and special characters may not display correctly." << endl;
        return false;
    }
    
    return true;
#else
    // On Unix-like systems (Linux, macOS), UTF-8 and ANSI codes are usually enabled by default
    return true;
#endif
}

/**
 * @brief Clears the console screen based on the operating system.
 *
 * Executes platform-specific system commands: "cls" on Windows, "clear" on Linux/macOS.
 *
 * @return `true` if successful, `false` if an error occurred.
 * @note If this fails, the game continues but the screen won't be cleared.
 */
bool clearConsole() {
    int result;
#ifdef _WIN32
    result = std::system("cls"); // For Windows
#elif defined(__unix__) || defined(__APPLE__) // Linux or macOS
    result = std::system("clear"); // For Linux and macOS
#else
    // Unsupported operating system
    cerr << "Error: Unsupported operating system for console clearing." << endl;
    cerr << "       Supported systems: Windows, Linux, macOS." << endl;
    return false;
#endif

    // Check if the command executed successfully
    if (result != 0) {
        cerr << "Error: Failed to clear the console screen. Exit code: " << result << endl;
        return false;
    }
    
    return true;
}

/**
 * @brief Displays the game logo in ASCII art.
 *
 * Prints the Hnefatafl game logo with stylized text and credit line.
 * Best called after `clearConsole()` for clean display.
 *
 * @note Uses UTF-8 block characters. Call `enableTerminalFormatting()` first for proper Windows display.
 */
void displayHnefataflLogo() {
    // TODO: Implement logo display
}

// ============================================================================
// SECTION 2: BOARD MANAGEMENT
// ============================================================================

/**
 * @brief Asks the user to choose the game board size.
 *
 * Prompts the user to select board size: 11 (LITTLE) or 13 (BIG).
 * Validates input strictly (rejects non-integers, invalid sizes, leading zeros, whitespace).
 *
 * @param aBoardSize Reference to store the selected board size (LITTLE or BIG).
 * @return `true` if valid size chosen, `false` otherwise.
 */
bool chooseSizeBoard(BoardSize& aBoardSize)
{
    // TODO: Implement board size selection
    return false;
}

/**
 * @brief Dynamically creates a game board.
 *
 * Allocates memory for a 2D array of cells. Validates board size and checks for existing allocation.
 * Cleans up memory on failure to avoid leaks.
 *
 * @param aBoard Reference to the Board object (`itsSize` must be set).
 * @return `true` if successful, `false` on allocation failure or invalid input.
 */
bool createBoard(Board& aBoard) {
    // TODO: Implement dynamic board allocation
    return false;
}

/**
 * @brief Frees the memory allocated for the game board.
 *
 * Deallocates the 2D array of cells and sets `itsCells` to `nullptr`.
 * Safe to call multiple times (does nothing if already freed).
 *
 * @param aBoard Reference to the Board object to deallocate.
 */
void deleteBoard(Board& aBoard) {
    // TODO: Implement board deallocation
}

/**
 * @brief Displays the game board with piece positions and labels.
 *
 * Shows the board with column numbers (1-N) and row letters (A-N).
 * Displays pieces (shields, swords, king) and special cells (castle, fortresses).
 *
 * @param aBoard The game board object to display.
 * @note Handles both LITTLE (11x11) and BIG (13x13) board sizes.
 */
void displayBoard(const Board& aBoard) {
    // TODO: Implement board display
}

/**
 * @brief Initializes the game board with starting positions.
 *
 * Places fortresses (4 corners), castle (center), king (center),
 * shields (12 defenders), and swords (24 attackers) based on board size.
 *
 * @param aBoard The board object to initialize (`itsCells` must be allocated, `itsSize` must be set).
 * @note Adjusts piece positions for LITTLE (11x11) vs BIG (13x13) boards.
 */
void initializeBoard(Board& aBoard)
{
    // TODO: Implement board initialization with starting positions
}

// ============================================================================
// SECTION 3: POSITION MANAGEMENT
// ============================================================================

/**
 * @brief Checks if a position is valid within the game board.
 *
 * Verifies that row and column indices are within board bounds (0-based indexing).
 *
 * @param aPos The position to check (row and column indices).
 * @param aBoard The game board containing size information.
 * @return `true` if position is within bounds, `false` otherwise.
 */
bool isValidPosition(const Position& aPos, const Board& aBoard) {
    // TODO: Implement position validation
    return false;
}


/**
 * @brief Retrieves and validates a position from user input (format: letter + number).
 *
 * Expects format: letter (A-Z) + number (1-based, e.g., "A1", "C3").
 * Validates: letter format, numeric digits, overflow protection, board bounds.
 * Updates `aPosition` with 0-based indices if valid.
 *
 * @param aPosition Reference to Position structure to update with user input.
 * @param aBoard The game board for bounds validation.
 * @return `true` if input valid and position updated, `false` otherwise.
 */
bool getPositionFromInput(Position& aPosition, const Board& aBoard)
{
    // TODO: Implement position input parsing and validation
    return false;
}



/**
 * @brief Checks if a cell is empty (contains no piece).
 *
 * A cell is empty if `itsPieceType` is NONE (ignores `itsCellType`).
 *
 * @param aBoard The game board.
 * @param aPos The position of the cell to check.
 * @return `true` if cell is empty, `false` if it contains a piece.
 */
bool isEmptyCell(const Board& aBoard, const Position& aPos) {
    // TODO: Implement empty cell check
    return false;
}

// ============================================================================
// SECTION 4: MOVEMENT & ACTIONS
// ============================================================================

/**
 * @brief Checks if a move is valid for the current player.
 *
 * Validates: piece ownership (SWORD for ATTACK, SHIELD/KING for DEFENSE),
 * horizontal/vertical movement only, clear path (no pieces blocking),
 * special cell restrictions (only KING can finish on FORTRESS/CASTLE).
 *
 * @param aGame Current game state (player, board).
 * @param aMove The move to validate (start and end positions).
 * @return `true` if move is valid, `false` otherwise.
 */
bool isValidMovement(const Game& aGame, const Move& aMove)
{
    // TODO: Implement movement validation
    return false;
}

/**
 * @brief Moves a piece on the board.
 *
 * Transfers piece from start position to end position. Clears start cell (sets to NONE).
 * Cell types (NORMAL, FORTRESS, CASTLE) remain unchanged.
 *
 * @param aGame Current game state.
 * @param aMove The move to execute (start and end positions).
 * @note Assumes move is valid. Use `isValidMovement()` first to validate.
 */
void movePiece(Game& aGame, const Move& aMove) {
    // TODO: Implement piece movement
}

/**
 * @brief Removes captured pieces from the board.
 *
 * Checks all 4 cardinal directions from move end position. Applies capture rules:
 * - ATTACK captures SHIELD when sandwiched between SWORD/FORTRESS/empty CASTLE
 * - DEFENSE captures SWORD when sandwiched between SHIELD/KING/FORTRESS/CASTLE
 *
 * @param aGame Current game state.
 * @param aMove The executed move (uses end position for capture check).
 * @note Assumes move already validated and executed. Modifies board to remove captured pieces.
 */
void capturePieces(Game& aGame, const Move& aMove) {
    // TODO: Implement capture logic
}


/**
 * @brief Switches the active player.
 *
 * Toggles between Player1 and Player2 for alternating turns.
 *
 * @param aGame Reference to the Game object (updates `itsCurrentPlayer`).
 */
void switchCurrentPlayer(Game& aGame)
{
    // TODO: Implement player switch
}

// ============================================================================
// SECTION 5: ENDGAME & VICTORY
// ============================================================================

/**
 * @brief Checks if any SWORD pieces remain on the board.
 *
 * Scans entire board for at least one SWORD piece.
 *
 * @param aBoard The game board to check.
 * @return `true` if at least one SWORD exists, `false` if none remain.
 */
bool isSwordLeft(const Board& aBoard) {
    // TODO: Implement sword detection
    return false;
}

/**
 * @brief Gets the king's position on the board.
 *
 * Scans board to locate the KING piece.
 *
 * @param aBoard The game board to scan.
 * @return Position of the king, or {-1, -1} if not found.
 */
Position getKingPosition(const Board& aBoard) {
    // TODO: Implement king position search
    return {-1, -1};
}

/**
 * @brief Checks if the king has escaped to a fortress.
 *
 * Victory condition for DEFENSE: king reaches any corner FORTRESS cell.
 *
 * @param aBoard The game board to check.
 * @return `true` if king is on a FORTRESS cell, `false` otherwise.
 */
bool isKingEscaped(const Board& aBoard) {
    // TODO: Implement king escape check
    return false;
}

/**
 * @brief Checks if the king is captured (simple version - 4 directions).
 *
 * King captured if all 4 adjacent cells (up/down/left/right) are hostile:
 * out of bounds, SWORD pieces, FORTRESS, or CASTLE cells.
 *
 * @param aBoard The game board to check.
 * @return `true` if king surrounded on all 4 sides, `false` otherwise.
 * @note Simpler version for students. Checks only immediate neighbors.
 */
bool isKingCapturedSimple(const Board& aBoard) {
    // TODO: Implement simple king capture check (4 directions)
    return false;
}

/**
 * @brief Checks if the king is captured (recursive version with escape detection).
 *
 * Advanced version: explores SHIELD chains recursively to find escape routes.
 * King captured only if completely blocked with no path to free cells.
 * Automatically manages memory for visited cell tracking.
 *
 * @param aBoard The game board to check.
 * @param aKingPos Position to check (default {-1,-1} auto-detects king).
 * @param isCellChecked Internal tracking array (default nullptr, auto-initialized).
 * @return `true` if king completely blocked, `false` if escape route exists.
 * @note Advanced version for bonus points. Call with default params: `isKingCapturedRecursive(board)`.
 */
bool isKingCapturedRecursive(const Board& aBoard, Position aKingPos, bool** isCellChecked) {
    // TODO: Implement recursive king capture check with escape detection
    return false;
}

/**
 * @brief Checks if the game has ended.
 *
 * Game ends when: king captured, no SWORD pieces left, or king escaped to FORTRESS.
 *
 * @param aGame The current game state.
 * @return `true` if game finished, `false` otherwise.
 */
bool isGameFinished(const Game& aGame)
{
    // TODO: Implement game end detection
    return false;
}


/**
 * @brief Determines the winner of the game.
 *
 * Returns winner if game finished:
 * - ATTACK (Player1) wins if king captured
 * - DEFENSE (Player2) wins if king escaped or no swords left
 *
 * @param aGame The current game state.
 * @return Pointer to winning Player, or `nullptr` if game not finished.
 * @note Call after `isGameFinished()` returns true.
 */
const Player* whoWon(const Game& aGame)
{
    // TODO: Implement winner determination
    return nullptr;
}
