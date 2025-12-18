/**
 * @file tests.h
 *
 * @brief Declarations of test functions for testing the fonctions implemented in 'fonctions.cpp'.
 *
 * This file contains the declarations of test functions used to verify and validate
 * the functionality of the functions implemented in 'fonctions.cpp.'
 * These test functions are designed to thoroughly test various aspects of the code
 * in 'fonctions.cpp' to ensure that it performs as expected.
 * They cover a range of scenarios and edge cases, helping to identify and resolve
 * any potential issues or bugs in the codebase.
 * The tests are conducted on different board sizes and configurations,
 * providing comprehensive coverage to guarantee the reliability and correctness
 * of the functions in 'fonctions.cpp.'
 * Overall, 'test.cpp' plays a crucial role in maintaining code quality and robustness by confirming
 * that the functions it tests meet their intended specifications.
 *
 * @author JMB and zecross-dev - IUT Informatique La Rochelle
 * @date 10/11/2025
 */

#ifndef TESTS_H
#define TESTS_H

// ========================= ANSI COLOR CODES =========================
// ANSI Color codes for better test output readability
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

// ========================= GLOBAL VARIABLES =========================
// Global variables to control test display behavior
extern bool DISPLAY_BOARDS;
extern bool DISPLAY_PROMPTS;

// ========================= TEST FUNCTIONS =========================

// ─────────────────────────────────────────────────────────────────
// Board Setup and Management Tests
// ─────────────────────────────────────────────────────────────────

/**
 * @brief Test function for the chooseSizeBoard function.
 *
 * This function tests the chooseSizeBoard function by simulating user input and
 * checking the returned values.
 */
void test_chooseSizeBoard();

/**
 * @brief Test function for the createBoard function.
 *
 * This function tests the createBoard function by verifying that boards are correctly allocated
 * for different sizes. Tests cover valid board sizes, pointer verification, memory allocation
 * checks, and edge cases including size validation and double allocation detection.
 */
void test_createBoard();

/**
 * @brief Test function for the deleteBoard function.
 *
 * This function tests the deleteBoard function by ensuring that boards are correctly deleted
 * and pointers are set to nullptr after deletion.
 */
void test_deleteBoard();

/**
 * @brief Test function for the initializeBoard function.
 *
 * This function tests the initializeBoard function by comparing the generated board with expected board configurations.
 */
void test_initializeBoard();

// ─────────────────────────────────────────────────────────────────
// Position and Cell Validation Tests
// ─────────────────────────────────────────────────────────────────

/**
 * @brief Test function for the getPositionFromInput function.
 *
 * This function tests the getPositionFromInput function by providing various input strings and
 * comparing the returned positions with expected positions. Tests cover valid inputs, boundary
 * cases, format errors, and invalid input conditions for both board sizes.
 */
void test_getPositionFromInput();

/**
 * @brief Test function for the isValidPosition function.
 *
 * This function tests the isValidPosition function for different board sizes, valid positions,
 * and invalid positions. Tests include corners, edges, center, out-of-bounds positions, and
 * extreme values for both LITTLE and BIG board sizes.
 */
void test_isValidPosition();

/**
 * @brief Test function for the isEmptyCell function.
 *
 * This function tests the isEmptyCell function by checking various combinations of cell types
 * and piece types. It verifies that the function correctly identifies empty cells (NONE) and
 * non-empty cells (SWORD, SHIELD, or KING) across different board sizes and cell types.
 */
void test_isEmptyCell();

// ─────────────────────────────────────────────────────────────────
// Movement and Action Tests
// ─────────────────────────────────────────────────────────────────

/**
 * @brief Test function for the isValidMovement function.
 *
 * This function tests the isValidMovement function by checking various movement scenarios
 * including piece ownership validation, movement direction rules (horizontal/vertical only),
 * path obstruction, and special cell restrictions (FORTRESS, CASTLE). Tests cover both
 * ATTACK and DEFENSE roles with different piece types.
 */
void test_isValidMovement();

/**
 * @brief Test the movePiece function with different board sizes and piece types.
 *
 * This function tests the movePiece function by verifying that pieces are correctly moved
 * on the board. Tests cover all piece types (SWORD, SHIELD, KING) with horizontal and vertical
 * moves, and special KING moves to/from FORTRESS and CASTLE cells. Note: movePiece only handles
 * mechanical piece relocation; move validation is tested separately in test_isValidMovement.
 */
void test_movePiece();

/**
 * @brief Test the capturePieces function with various scenarios.
 *
 * This function tests the capturePieces function by setting up different scenarios
 * to capture pieces on the board. It covers ATTACK captures (SWORD captures SHIELD),
 * DEFENSE captures (SHIELD/KING captures SWORD), and non-capture cases where pieces
 * are protected or on the same team.
 */
void test_capturePieces();

/**
 * @brief Test function for switchCurrentPlayer.
 *
 * This function tests the switchCurrentPlayer function by toggling between players
 * and verifying the current player state.
 */
void test_switchCurrentPlayer();

// ─────────────────────────────────────────────────────────────────
// Game State and Victory Condition Tests
// ─────────────────────────────────────────────────────────────────

/**
 * @brief Test function for isSwordLeft.
 *
 * This function tests the isSwordLeft function by verifying that it correctly detects
 * the presence or absence of SWORD pieces on the board. Tests cover boards with no swords,
 * single sword, and multiple swords across different positions and board sizes.
 */
void test_isSwordLeft();

/**
 * @brief Test function for getKingPosition.
 *
 * This function tests the getKingPosition function by verifying that it correctly retrieves
 * the position of the king on the board. Tests cover different board sizes and various king
 * positions including corners, edges, center, and special cells (FORTRESS, CASTLE).
 */
void test_getKingPosition();

/**
 * @brief Test function for isKingEscaped.
 *
 * This function tests the isKingEscaped function by verifying that it correctly detects
 * when the king has escaped to a FORTRESS cell. Tests cover different board sizes and
 * various king positions including FORTRESS corners, CASTLE center, and NORMAL cells.
 */
void test_isKingEscaped();

/**
 * @brief Test function for isKingCapturedSimple.
 *
 * This function tests the isKingCapturedSimple function by verifying that it correctly detects
 * when the king is captured. Tests cover scenarios with the king surrounded by SWORD pieces on
 * all four sides, partial encirclement, and edge cases with FORTRESS and CASTLE cells.
 */
void test_isKingCapturedSimple();

/**
 * @brief Test function for isKingCapturedRecursive.
 *
 * This function tests the isKingCapturedRecursive function by verifying that it correctly detects
 * king captures with escape route analysis. Tests include complex scenarios with SHIELD chains,
 * blocked escape routes, and situations where the king has valid escape paths.
 */
void test_isKingCapturedRecursive();

/**
 * @brief Test function for isGameFinished.
 *
 * This function tests the isGameFinished function by verifying that it correctly detects
 * game ending conditions: king captured, king escaped to FORTRESS, or no SWORD pieces left.
 * Tests cover various scenarios for both board sizes.
 */
void test_isGameFinished();

/**
 * @brief Test function for whoWon.
 *
 * This function tests the whoWon function by verifying that it correctly returns the winner
 * based on different game ending scenarios: DEFENSE wins if king escaped or no swords left,
 * ATTACK wins if king is captured.
 */
void test_whoWon();

// ========================= HELPER FUNCTIONS =========================

/**
 * @brief Configures the display settings for tests.
 * @param showBoards Whether to display the board during tests
 * @param showPrompts Whether to display prompts during tests
 */
void configureTestDisplay(bool showBoards, bool showPrompts);

/**
 * @brief Prints the main test suite header.
 * Displays a stylized header at the start of the test suite execution.
 */
void printTestSuiteHeader();

/**
 * @brief Prints the main test suite footer.
 * Displays a stylized footer at the end of the test suite execution with helpful information.
 */
void printTestSuiteFooter();

#endif // TESTS_H