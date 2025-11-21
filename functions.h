/**
 * @file fonctions.h
 *
 * @brief Declarations of functions for the Hnefatafl game.
 *
 * This file contains the declarations of various functions used in the Hnefatafl game, including
 * board initialization, display, piece movement, capturing, and game state checks.
 *
 * @author JMB - IUT Informatique La Rochelle
 * @date 10/11/2025
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "typeDef.h"

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
bool enableTerminalFormatting();

/**
 * @brief Clears the console screen based on the operating system.
 *
 * Executes platform-specific system commands: "cls" on Windows, "clear" on Linux/macOS.
 *
 * @return `true` if successful, `false` if an error occurred.
 * @note If this fails, the game continues but the screen won't be cleared.
 */
bool clearConsole();

/**
 * @brief Displays the game logo in ASCII art.
 *
 * Prints the Hnefatafl game logo with stylized text and credit line.
 * Best called after `clearConsole()` for clean display.
 *
 * @note Uses UTF-8 block characters. Call `enableTerminalFormatting()` first for proper Windows display.
 */
void displayHnefataflLogo();

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
bool chooseSizeBoard(BoardSize& aBoardSize);

/**
 * @brief Dynamically creates a game board.
 *
 * Allocates memory for a 2D array of cells. Validates board size and checks for existing allocation.
 * Cleans up memory on failure to avoid leaks.
 *
 * @param aBoard Reference to the Board object (`itsSize` must be set).
 * @return `true` if successful, `false` on allocation failure or invalid input.
 */
bool createBoard(Board& aBoard);

/**
 * @brief Frees the memory allocated for the game board.
 *
 * Deallocates the 2D array of cells and sets `itsCells` to `nullptr`.
 * Safe to call multiple times (does nothing if already freed).
 *
 * @param aBoard Reference to the Board object to deallocate.
 */
void deleteBoard(Board& aBoard);

/**
 * @brief Displays the game board with piece positions and labels.
 *
 * Shows the board with column numbers (1-N) and row letters (A-N).
 * Displays pieces (shields, swords, king) and special cells (castle, fortresses).
 *
 * @param aBoard The game board object to display.
 * @note Handles both LITTLE (11x11) and BIG (13x13) board sizes.
 */
void displayBoard(const Board& aBoard);

/**
 * @brief Initializes the game board with starting positions.
 *
 * Places fortresses (4 corners), castle (center), king (center),
 * shields (12 defenders), and swords (24 attackers) based on board size.
 *
 * @param aBoard The board object to initialize (`itsCells` must be allocated, `itsSize` must be set).
 * @note Adjusts piece positions for LITTLE (11x11) vs BIG (13x13) boards.
 */
void initializeBoard(Board& aBoard);

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
bool isValidPosition(const Position& aPos, const Board& aBoard);

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
bool getPositionFromInput(Position& aPosition, const Board& aBoard);

/**
 * @brief Checks if a cell is empty (contains no piece).
 *
 * A cell is empty if `itsPieceType` is NONE (ignores `itsCellType`).
 *
 * @param aBoard The game board.
 * @param aPos The position of the cell to check.
 * @return `true` if cell is empty, `false` if it contains a piece.
 */
bool isEmptyCell(const Board& aBoard, const Position& aPos);

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
bool isValidMovement(const Game& aGame, const Move& aMove);

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
void movePiece(Game& aGame, const Move& aMove);

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
void capturePieces(Game& aGame, const Move& aMove);

/**
 * @brief Switches the active player.
 *
 * Toggles between Player1 and Player2 for alternating turns.
 *
 * @param aGame Reference to the Game object (updates `itsCurrentPlayer`).
 */
void switchCurrentPlayer(Game& aGame);

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
bool isSwordLeft(const Board& aBoard);

/**
 * @brief Gets the king's position on the board.
 *
 * Scans board to locate the KING piece.
 *
 * @param aBoard The game board to scan.
 * @return Position of the king, or {-1, -1} if not found.
 */
Position getKingPosition(const Board& aBoard);

/**
 * @brief Checks if the king has escaped to a fortress.
 *
 * Victory condition for DEFENSE: king reaches any corner FORTRESS cell.
 *
 * @param aBoard The game board to check.
 * @return `true` if king is on a FORTRESS cell, `false` otherwise.
 */
bool isKingEscaped(const Board& aBoard);

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
bool isKingCapturedSimple(const Board& aBoard);

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
bool isKingCapturedRecursive(const Board& aBoard, Position aKingPos = {-1, -1}, bool** isCellChecked = nullptr);

/**
 * @brief Checks if the game has ended.
 *
 * Game ends when: king captured, no SWORD pieces left, or king escaped to FORTRESS.
 *
 * @param aGame The current game state.
 * @return `true` if game finished, `false` otherwise.
 */
bool isGameFinished(const Game& aGame);

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
const Player* whoWon(const Game& aGame);

#endif // FUNCTIONS_H
