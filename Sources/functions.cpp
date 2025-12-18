/**
 * @file functions.cpp
 *
 * @brief Implementation of functions for the Hnefatafl game.
 *
 * This file contains the implementation of various functions used in the Hnefatafl game, including
 * board initialization, display, piece movement, capturing, and game state checks.
 *
 * @author JMB and zecross-dev - IUT Informatique La Rochelle
 * @date 10/11/2025
 */
#include <charconv>
#include <complex.h>
#include <complex>
#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream> //for save functions
#include <filesystem>
#include "../Headers/typeDef.h"
#include "../Headers/functions.h"

using namespace std;
namespace fs = std::filesystem;
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
    clearConsole();
    cout << "╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║     ■           ■                                                                ■■■■■■  ■■■■■■                                                           ║" << endl;
    cout << "║      ■■       ■■     ■■             ■■■■■■■■■       ■■■■■■■■■■      ■■■       ■■■■   ■■■■■■   ■■■■     ■■■              ■■■■■■■■■       ■■■■■             ║" << endl;
    cout << "║      ■■■     ■■■     ■■■■■■■■■       ■■■■    ■■      ■■■      ■■    ■■■■■   ■■        ■■■        ■■    ■■■■■             ■■■      ■■      ■■■             ║" << endl;
    cout << "║      ■■       ■■     ■■■■   ■■■    ■■■      ■      ■■■       ■     ■■■  ■■■            ■■■            ■■■  ■■■         ■■■       ■        ■■■             ║" << endl;
    cout << "║      ■■       ■■     ■■■     ■■■    ■■■             ■■■            ■■■   ■■■           ■■■            ■■■   ■■■         ■■■               ■■■             ║" << endl;
    cout << "║      ■■■     ■■■      ■■■     ■■■   ■■■             ■■■           ■■■    ■■■            ■■■          ■■■    ■■■         ■■■              ■■■              ║" << endl;
    cout << "║     ■■■■■   ■■■■■     ■■■     ■■■   ■■■■           ■■■■          ■■■     ■■■  ■■        ■■■         ■■■     ■■■  ■■    ■■■■              ■■■              ║" << endl;
    cout << "║    ■■■■■■■■■■■■■■     ■■■     ■■■  ■■■■■■ ■■      ■■■■■■ ■■     ■■■■■   ■■■■■■ ■■      ■■■         ■■■■■   ■■■■■■ ■■   ■■■■■■ ■■         ■■■              ║" << endl;
    cout << "║      ■■■■   ■■■■      ■■■     ■■■   ■■■             ■■■         ■■■  ■■■■  ■■■        ■■■          ■■■  ■■■■  ■■■       ■■■            ■■■                ║" << endl;
    cout << "║       ■■     ■■      ■■■     ■■■    ■■■             ■■■         ■■■      ■■■         ■■■           ■■■        ■■■       ■■■          ■■■                  ║" << endl;
    cout << "║      ■■■     ■■■     ■■■     ■■■    ■■■      ■      ■■■         ■■■      ■■■          ■■■          ■■■      ■■■         ■■■        ■■■                    ║" << endl;
    cout << "║      ■■       ■■     ■■     ■■      ■■■     ■■      ■■■         ■■■      ■■■       ■   ■■■  ■      ■■■      ■■■         ■■■       ■■■               ■■■   ║" << endl;
    cout << "║     ■           ■    ■     ■      ■■■■■■■■■■■     ■■■■■           ■■■      ■■■      ■■■■■■■■        ■■■      ■■■      ■■■■■        ■■■■■■■■■■■■■■■■■■     ║" << endl;
    cout << "╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝" << endl;
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
    int size ;
    cout << "Select a Board size between 11 or 13 here :  " ;
    cin >> size ;
    if (size == 13) {
        cout << "You selected 13x13 board." << endl;
        aBoardSize = BIG;
    }
    else if (size ==11) {
        cout << "You selected 11x11 board." << endl;
        aBoardSize = LITTLE;
    }
    else {
        cout << "invalid size please retry." << endl;
        return false;
    }
    return true;
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
    const int SIZE = aBoard.itsSize;
    // dont do if size is null
    if ( SIZE == 0 ) {
        return false;
    }
    // prevent double allocation
    if (aBoard.itsCells != nullptr) {
        return false;
    }
    // allocation of the boards lines
    aBoard.itsCells = new Cell*[SIZE];
    // allocation of Columns for each line
    for (int line = 0; line < SIZE; line++){
            aBoard.itsCells[line] = new Cell[SIZE];
    }
    return true;
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
    const int SIZE = aBoard.itsSize;
    for (int line = 0; line < SIZE; line++) {
        //free each row of the table
        if (aBoard.itsCells != nullptr) {
            delete[] aBoard.itsCells[line] ;
            aBoard.itsCells[line] = nullptr;
        }
    }
    //free all the table
    if (aBoard.itsCells != nullptr) {
        delete[] aBoard.itsCells ;
        aBoard.itsCells = nullptr;
    }
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
    const int SIZE = aBoard.itsSize;
    const string CHARBOARD = "ABCDEFGHIJKLM";
    cout << "    ";

    for (int column = 0 ; column<SIZE ; column++ ) {
        cout << "  " << column +1 ;
        if (column>=9) {
            cout <<"  ";
        }
        else {
            cout <<"   ";
        }
    }
    cout <<endl;
    cout<<"   ╬";
    for (int column = 0 ; column<SIZE ; column++ ) {
        if (column != SIZE-1) {
            cout << "═════╬";
        }
        else {
            cout << "═════╣";
        }
    }
    cout <<endl;
    for (int line = 0 ; line<SIZE ; line++) {
        cout << " " << CHARBOARD[line] << " ║";
        for (int column = 0 ; column<SIZE ; column++) {
            //take the enums as integers
            CellType pos = aBoard.itsCells[line][column].itsCellType;
            PieceType piece = aBoard.itsCells[line][column].itsPieceType;
            cout << "  ";
            if (piece == NONE ) {

                if (pos == NORMAL) {
                    cout << " ";
                }
                else if (pos == CASTLE) {
                    cout << "x";
                }
                else if (pos == FORTRESS) {
                    cout << "♜";
                }
            }
            else if (piece == KING) {
                cout << "♕";
            }
            else if (piece == SHIELD) {
                cout << "♦";
            }
            else if (piece == SWORD) {
                cout <<"⚔";
            }
            cout << "  ║";
        }
        if (line != SIZE-1) {
            cout<< endl << "   ╬";
        }
        else {
            cout<< endl << "   ╩";
        }
        for (int column = 0 ; column<SIZE ; column++ ) {
            if (column != SIZE-1 && line != SIZE-1) {
                cout << "═════╬";
            }
            else if (column == SIZE-1 && line != SIZE-1) {
                cout << "═════╣";
            }
            else if (column != SIZE-1 && line == SIZE-1) {
                cout <<"═════╩";
            }
            else {
                cout <<"═════╝";
            }
        }
        cout << endl;
    }
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
void initializeBoard(Board& aBoard) {
    const int SIZE = aBoard .itsSize;
    const int BIG_CENTER = 6 , LITTLE_CENTER = 5;
    //set cellType and set piece to NONE
    for (int line = 0 ; line < SIZE ; line++) {
        for (int column = 0 ; column < SIZE ; column++) {
            //test if the coords exist
            if (aBoard.itsCells != nullptr){
                //set castle and king in center
                if (line == SIZE/2 && column == SIZE/2) {
                    aBoard.itsCells[line][column].itsCellType  = CASTLE;
                    aBoard.itsCells[line][column].itsPieceType = KING;
                }
                //set fortress in corners
                else if ((line == 0 || line == SIZE-1 ) && (column== 0 || column == SIZE-1 )){
                    aBoard.itsCells[line][column].itsCellType  = FORTRESS;
                    aBoard.itsCells[line][column].itsPieceType = NONE;
                }
                //set all the other pieces at normal case and none piece
                else {
                    aBoard.itsCells[line][column].itsCellType  = NORMAL;
                    aBoard.itsCells[line][column].itsPieceType = NONE;
                }
            }
        }
    }
    //set all the piece type
    //test the size before the loop (checking one time)
    if (SIZE == 11) {
        for (int line = 0 ; line < SIZE ; line++) {
            for (int column = 0 ; column < SIZE; column++) {
                if (aBoard.itsCells != nullptr){
                    //place the shields around the king
                    if (line == LITTLE_CENTER && column != LITTLE_CENTER && column >= LITTLE_CENTER-2 && column <= LITTLE_CENTER+2) {
                        aBoard.itsCells[line][column].itsPieceType = SHIELD;
                        //place the invert coords pieces for optimisation
                        aBoard.itsCells[column][line].itsPieceType = SHIELD;
                    }
                    //place shields on the diagonals of the king
                    if ((line == LITTLE_CENTER-1 || line == LITTLE_CENTER +1) && (column == LITTLE_CENTER-1 || column == LITTLE_CENTER+1)) {
                        aBoard.itsCells[line][column].itsPieceType = SHIELD;
                    }

                    //place the swords on the board
                    if ((line == 0 || line == SIZE -1 )&& column > 2 && column < 8) {
                        aBoard.itsCells[line][column].itsPieceType = SWORD;
                        aBoard.itsCells[column][line].itsPieceType = SWORD;
                    }

                    if (line == LITTLE_CENTER && (column == 1 || column == 9)) {
                        aBoard.itsCells[line][column].itsPieceType = SWORD;
                        aBoard.itsCells[column][line].itsPieceType = SWORD;
                    }
                }
            }
        }
    }
    else if (SIZE == 13) {
        for (int line = 0 ; line < SIZE ; line++) {
            for (int column = 0 ; column < SIZE; column++) {
                if (aBoard.itsCells != nullptr){
                    //place the shields around the king
                    if (line == BIG_CENTER && column != BIG_CENTER && column >= BIG_CENTER-3 && column <= BIG_CENTER+3) {
                        aBoard.itsCells[line][column].itsPieceType = SHIELD;
                        //place the invert coords pieces for optimisation
                        aBoard.itsCells[column][line].itsPieceType = SHIELD;
                    }
                    //place the swords on the board
                    if ((line == 0 || line == SIZE -1 )&& column > 3 && column < 9) {
                        aBoard.itsCells[line][column].itsPieceType = SWORD;
                        aBoard.itsCells[column][line].itsPieceType = SWORD;
                    }
                    if (line == BIG_CENTER && (column == 1 || column == 11)) {
                        aBoard.itsCells[line][column].itsPieceType = SWORD;
                        aBoard.itsCells[column][line].itsPieceType = SWORD;
                    }
                }
            }
        }
    }
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
    const int SIZE = aBoard.itsSize;
    //test if position is valid
    if (aPos.itsCol>=0 && aPos.itsCol<SIZE && aPos.itsRow>=0 && aPos.itsRow<SIZE) {
        return true;
    }
    cout << "invalid position";
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
bool getPositionFromInput(Position& aPosition, const Board& aBoard){
    const int CHARMAJ = 'A';
    const int CHARMIN = 'a';
    bool validSelection = false;
    string pos;
    do {
        pos ="";
        cout << "Select a position :  ";
        cin >> pos;
        if (pos.length()!=2 && pos.length()!=3) {
            cout << endl << "Error of position format the good format is : letter + number" << endl << "exemple : A1";
            return false;
        }
        int ftCoord = pos[0];
        if (ftCoord <CHARMIN) {
            ftCoord -= CHARMAJ;
        }
        else {
            ftCoord -= CHARMIN;
        }
        int ndCoord;

        if (pos.length()==2) {
            ndCoord = pos[1]-49;
        }
        else {
            ndCoord = (pos[1]-48)*10 + pos[2]-49;
        }
        aPosition.itsRow = ftCoord;
        aPosition.itsCol = ndCoord;
        validSelection = isValidPosition(aPosition , aBoard);
    }while (validSelection == false);
    return true;
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
    //use ternary operator for test if piece type is NONE (if yes return true else return false)
    return aBoard.itsCells[aPos.itsRow][aPos.itsCol].itsPieceType ? NONE : true;
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
bool isValidMovement(const Game& aGame, const Move& aMove) {
    const int SIZE = aGame.itsBoard.itsSize;
    Player *player=aGame.itsCurrentPlayer;
    //test if the position is on the bounds of the board
    if (aMove.itsEndPosition.itsRow < 0 || aMove.itsEndPosition.itsRow >= SIZE || aMove.itsEndPosition.itsCol < 0 || aMove.itsEndPosition.itsCol >= SIZE ||
        aMove.itsStartPosition.itsRow<0 ||aMove.itsStartPosition.itsRow >=SIZE || aMove.itsStartPosition.itsCol < 0 || aMove.itsStartPosition.itsCol >=SIZE)
    {
        cout << "Error : ";
        cerr << "Selection is out of bounds" <<endl ;
        return false;
    }
    //test if player try to moove a right piece
    if (player->itsRole == ATTACK && aGame.itsBoard.itsCells[aMove.itsStartPosition.itsRow][aMove.itsStartPosition.itsCol].itsPieceType != SWORD )
    {
        cout << "Error : ";
        cerr << "Attack c'ant moove other pieces than SWORD" << endl;
        return false;
    }
    if (player->itsRole == DEFENSE && (aGame.itsBoard.itsCells[aMove.itsStartPosition.itsRow][aMove.itsStartPosition.itsCol].itsPieceType == SWORD
       || aGame.itsBoard.itsCells[aMove.itsStartPosition.itsRow][aMove.itsStartPosition.itsCol].itsPieceType == NONE ))
    {
        cout << "Error : ";
        cerr << "DEFENCE can only moove SHIELD and the KING" <<endl;
        return false;
    }

    //test if a other piece than king try to escape or enter in castle
    if  (aGame.itsBoard.itsCells[aMove.itsStartPosition.itsRow][aMove.itsStartPosition.itsCol].itsPieceType !=KING &&
        (aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow][aMove.itsEndPosition.itsCol].itsCellType == FORTRESS ||
         aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow][aMove.itsEndPosition.itsCol].itsCellType == CASTLE)) {
        cout << "Error : ";
        cerr << "Only King can escape or go in castle" << endl;
        return false;
        }

    if (aMove.itsEndPosition.itsCol == aMove.itsStartPosition.itsCol &&  aMove.itsEndPosition.itsRow == aMove.itsStartPosition.itsRow) {
        return false;
    }
    int min,max;
    //test the movement on columns
    if (aMove.itsEndPosition.itsCol == aMove.itsStartPosition.itsCol) {
        if (aMove.itsStartPosition.itsRow < aMove.itsEndPosition.itsRow) {
            min = aMove.itsStartPosition.itsRow;
            max = aMove.itsEndPosition.itsRow;
        }
        else {
            max = aMove.itsStartPosition.itsRow;
            min = aMove.itsEndPosition.itsRow;
        }
        //test if the end case is empty
        if (aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow][aMove.itsStartPosition.itsCol].itsPieceType != NONE) {
            return false;
        }
        //loop for test in the row if a piece block
        for (int i = min+1 ; i <= max ; i++) {
            if (aGame.itsBoard.itsCells[i][aMove.itsStartPosition.itsCol].itsPieceType != NONE
                || aGame.itsBoard.itsCells[i][aMove.itsStartPosition.itsCol].itsCellType != NORMAL) {
                cout << "Error : ";
                cerr << "A piece block the movement in :" << char('A'+i) <<  aMove.itsStartPosition.itsCol << endl;
                return false;
            }
        }
    }
    else if (aMove.itsEndPosition.itsRow == aMove.itsStartPosition.itsRow) {
        if (aMove.itsStartPosition.itsCol < aMove.itsEndPosition.itsCol) {
            min = aMove.itsStartPosition.itsCol;
            max = aMove.itsEndPosition.itsCol;
        }
        else {
            max = aMove.itsStartPosition.itsCol;
            min = aMove.itsEndPosition.itsCol;
        }
        for (int i = min+1 ; i<= max ; i++) {
            if (aGame.itsBoard.itsCells[aMove.itsStartPosition.itsRow][i].itsPieceType != NONE
                || aGame.itsBoard.itsCells[aMove.itsStartPosition.itsRow][i].itsCellType != NORMAL) {
                cout << "Error : ";
                cerr << "A piece block the movement in : "  << char('A' + aMove.itsStartPosition.itsRow) << i << endl;
                return false;
            }
        }

    }
    else {
        return false;
    }
    return true;
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
    PieceType piece = aGame.itsBoard.itsCells[aMove.itsStartPosition.itsRow][aMove.itsStartPosition.itsCol].itsPieceType; //stock a piece on variable
    aGame.itsBoard.itsCells[aMove.itsStartPosition.itsRow][aMove.itsStartPosition.itsCol].itsPieceType = NONE; //set the 1st selected position piece as NONE
    aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow][aMove.itsEndPosition.itsCol].itsPieceType = piece; //replace the 2nd selected position with stocked piece
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
    const int SIZE = aGame.itsBoard.itsSize;
    //constexpr is initialized when compiling
    constexpr Position aroundCells[4] = {{0,-1},{0,1},{-1,0},{1,0}};
    //for defense
    if (aGame.itsCurrentPlayer->itsRole == DEFENSE) {
        //for each loop for test the 4 cells around the moove
        for (Position arroundCell : aroundCells ) {
            //test if the tested cell is in bounds
            if ((aMove.itsEndPosition.itsRow + arroundCell.itsRow) >= 0 && aMove.itsEndPosition.itsRow + arroundCell.itsCol <SIZE &&
                aMove.itsEndPosition.itsCol + arroundCell.itsCol >= 0 && aMove.itsEndPosition.itsCol + arroundCell.itsCol <SIZE){
                //test if the tested cell contain a SWORD
                if (aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +arroundCell.itsRow][aMove.itsEndPosition.itsCol +arroundCell.itsCol].itsPieceType==SWORD){
                    //test all capture condition
                    if (aMove.itsEndPosition.itsRow + arroundCell.itsRow == 0 || aMove.itsEndPosition.itsRow + arroundCell.itsCol == SIZE-1 ||
                        aMove.itsEndPosition.itsCol + arroundCell.itsCol == 0 || aMove.itsEndPosition.itsRow + arroundCell.itsCol == SIZE-1 ||
                        aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +2*arroundCell.itsRow][aMove.itsEndPosition.itsCol +2*arroundCell.itsCol].itsPieceType== SHIELD ||
                        aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +2*arroundCell.itsRow][aMove.itsEndPosition.itsCol +2*arroundCell.itsCol].itsPieceType== KING ||
                        aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +2*arroundCell.itsRow][aMove.itsEndPosition.itsCol +2*arroundCell.itsCol].itsCellType == FORTRESS ||
                        aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +2*arroundCell.itsRow][aMove.itsEndPosition.itsCol +2*arroundCell.itsCol].itsCellType == CASTLE &&
                        aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +2*arroundCell.itsRow][aMove.itsEndPosition.itsCol +2*arroundCell.itsCol].itsPieceType== NONE) {
                        aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +arroundCell.itsRow][aMove.itsEndPosition.itsCol +arroundCell.itsCol].itsPieceType = NONE;
                    }
                }
            }
        }
    }
    //same tests adapted for attack
    if (aGame.itsCurrentPlayer->itsRole == ATTACK) {
        for (Position arroundCell : aroundCells ) {
            //test if the tested cell is in bounds
            if ((aMove.itsEndPosition.itsRow + arroundCell.itsRow) >= 0 && aMove.itsEndPosition.itsRow + arroundCell.itsCol <SIZE &&
                aMove.itsEndPosition.itsCol + arroundCell.itsCol >= 0 && aMove.itsEndPosition.itsCol + arroundCell.itsCol <SIZE){
                //test if the tested cell contain a SHIELD
                if (aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +arroundCell.itsRow][aMove.itsEndPosition.itsCol +arroundCell.itsCol].itsPieceType==SHIELD){
                    //test all capture conditions
                    if (aMove.itsEndPosition.itsRow + arroundCell.itsRow == 0 || aMove.itsEndPosition.itsRow + arroundCell.itsCol == SIZE-1 ||
                        aMove.itsEndPosition.itsCol + arroundCell.itsCol == 0 || aMove.itsEndPosition.itsRow + arroundCell.itsCol == SIZE-1 ||
                        aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +2*arroundCell.itsRow][aMove.itsEndPosition.itsCol +2*arroundCell.itsCol].itsPieceType==SWORD ||
                        aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +2*arroundCell.itsRow][aMove.itsEndPosition.itsCol +2*arroundCell.itsCol].itsCellType == FORTRESS ||
                        aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +2*arroundCell.itsRow][aMove.itsEndPosition.itsCol +2*arroundCell.itsCol].itsCellType == CASTLE &&
                        aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +2*arroundCell.itsRow][aMove.itsEndPosition.itsCol +2*arroundCell.itsCol].itsPieceType== NONE) {
                        aGame.itsBoard.itsCells[aMove.itsEndPosition.itsRow +arroundCell.itsRow][aMove.itsEndPosition.itsCol +arroundCell.itsCol].itsPieceType = NONE;
                    }
                }
            }
        }
    }
}

/**
 * @brief Switches the active player.
 *
 * Toggles between Player1 and Player2 for alternating turns.
 *
 * @param aGame Reference to the Game object (updates `itsCurrentPlayer`).
 */
void switchCurrentPlayer(Game& aGame) {
    if (aGame.itsCurrentPlayer == &aGame.itsPlayer1){
        aGame.itsCurrentPlayer = &aGame.itsPlayer2;
    }
    else {
        aGame.itsCurrentPlayer = &aGame.itsPlayer1;
    }
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
    const int SIZE = aBoard.itsSize;
    for (int line = 0 ; line < SIZE ; line ++) {
        for (int col = 0 ; col < SIZE ; col++) {
            if (aBoard.itsCells[line][col].itsPieceType==SWORD) {
                return true;
            }
        }
    }
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
    const int SIZE = aBoard.itsSize;
    for (int line = 0 ; line < SIZE ; line ++) {
        for (int col = 0 ; col < SIZE ; col++) {
            if (aBoard.itsCells[line][col].itsPieceType==KING) {
                return {line , col};
            }
        }
    }
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
    Position kingCoords = getKingPosition(aBoard);
    if (kingCoords.itsRow == -1) {
        return false;
    }
    if (aBoard.itsCells[kingCoords.itsRow][kingCoords.itsCol].itsCellType == FORTRESS) {
        return true;
    }
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
    const int SIZE =aBoard.itsSize;
    Position kingPos = getKingPosition(aBoard) ;
    if (kingPos.itsRow == -1) {
        return false;
    }
    //List of 4 point around the king
    Position kingArounds[4] = {
            {kingPos.itsRow, kingPos.itsCol+1},
            {kingPos.itsRow, kingPos.itsCol-1},
            {kingPos.itsRow+1, kingPos.itsCol},
            {kingPos.itsRow-1, kingPos.itsCol}
    };
    int count = 0;
    for (auto& cardPoint : kingArounds) {
        if (cardPoint.itsCol < 0 || cardPoint.itsCol >= SIZE || cardPoint.itsRow < 0 || cardPoint.itsRow >=SIZE ) {
            count++;
        }
        else if (aBoard.itsCells[cardPoint.itsRow][cardPoint.itsCol].itsPieceType==SWORD ||
            aBoard.itsCells[cardPoint.itsRow][cardPoint.itsCol].itsCellType == CASTLE ||
            aBoard.itsCells[cardPoint.itsRow][cardPoint.itsCol].itsCellType == FORTRESS) {
            count++;
        }
        else {
            return false;
        }
    }
    if (count == 4) {
        return true;
    }
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
bool isKingCapturedRecursive(const Board& aBoard, Position aKingPos , bool** isCellChecked) {
    const int SIZE = aBoard.itsSize;
    //set the end condition if this is the first function iteration
    bool endCond = (isCellChecked == nullptr);
    // find king position if not gived (base : {-1,-1})
    if (aKingPos.itsCol == -1) {
        aKingPos.itsCol = getKingPosition(aBoard).itsCol;
        aKingPos.itsRow = getKingPosition(aBoard).itsRow;
    }
    // If pos = -1 (king not on the board) return false
    if (aKingPos.itsCol == -1) {
        return false;
    }
    // Allocate the tracking array
    isCellChecked = new bool *[SIZE];
    for (int line = 0 ; line < SIZE ; line++){
        isCellChecked[line] = new bool[SIZE];
        for (int col = 0; col < SIZE; col++) {
            isCellChecked[line][col] = false;
        }
    }
    // If the current cell is not a king or shield, it's not a valid escape route
    if (aBoard.itsCells[aKingPos.itsRow][aKingPos.itsCol].itsPieceType != KING ||
        aBoard.itsCells[aKingPos.itsRow][aKingPos.itsCol].itsPieceType != SHIELD) {
        //stop all if is the first iteration
        if (endCond) {
            //delocate memory
            for (int line = 0 ; line <SIZE ; line++) {
                delete[] isCellChecked[line];
            }
            delete[] isCellChecked;
        }
        return false;
    }
    // valid the visited cell
    isCellChecked[aKingPos.itsRow][aKingPos.itsRow] = true;
    // Check neighboring cells in all 4 directions
    Position checkArray[4] = {{0,-1},{0,1},{-1,0},{1,0}};
    //use a for each loop for all position of the array
    for (Position pos : checkArray) {
        //test if position is already checked and if position is in the bounds
        if (isValidPosition(pos,aBoard) && isCellChecked[pos.itsRow][pos.itsCol] == false ) {
            //verify if this is not sword (for found a escape)
            if (aBoard.itsCells[pos.itsRow][pos.itsCol].itsPieceType != SWORD &&
                aBoard.itsCells[pos.itsRow][pos.itsCol].itsCellType != CASTLE &&
                aBoard.itsCells[pos.itsRow][pos.itsCol].itsCellType != FORTRESS) {
                //explore the path (recursive)
                if(isKingCapturedRecursive(aBoard , pos , isCellChecked) == false) {
                    if (endCond) {
                        for (int line = 0 ; line <SIZE ; line++) {
                            delete[] isCellChecked[line];
                        }
                        delete[] isCellChecked;
                    }
                    return false;
                }
            }
        }
    }
    //if not escape found the king is captured
    if (endCond) {
        for (int line = 0 ; line <SIZE ; line++) {
            delete[] isCellChecked[line];
        }
        delete[] isCellChecked;
    }
    //if the cell is sword (no return before) return true
    return true;
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
    if (isKingCapturedSimple(aGame.itsBoard) == true || isKingEscaped(aGame.itsBoard) == true || isSwordLeft(aGame.itsBoard) == false) {
        return true;
    }
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
    if (isKingCapturedSimple(aGame.itsBoard) == true) {
        return &aGame.itsPlayer1;
    }
    if (isSwordLeft(aGame.itsBoard) == false) {
        return &aGame.itsPlayer2;
    }
    if (isKingEscaped(aGame.itsBoard) == true) {
        return &aGame.itsPlayer2;
    }
    return nullptr;
}

// ============================================================================
// PERSONAL ADDONS :
// ============================================================================


// ============================================================================
// SECTION 6: Save Gestion
// ============================================================================

/**
 * @brief create save of game when game is starting.
 *
 * Create folder /save if not already exist
 * Create a new file of save with selected name
 *  -if name is already used : overwrite suggestion
 *  -if maximum number of save (5) is reached : overwrite suggestion (on selected save)
 *  -if you choose to not overwrite a save , the game will not be able to save and return false.
 *
 *@note all save files will be stocked on the folder /save
 *
 * @param saveName pointer on the name of the save
 *
 * @return if save is successfully created
 */
bool createSave(string& saveName) {

    // Create Save folder if it does not exist
    if (!fs::exists("Save")) {
        cout << "Creating Save folder /Save..." << endl;
        fs::create_directory("Save");
    }
    //Enter the name of the save
    cout << "enter file name :" <<endl;
    cin >>saveName;
    // Count the number of save files
    int nbFile = 0;
    for (const auto& entry : fs::directory_iterator("Save")) {
        if (fs::is_regular_file(entry)) {
            nbFile++;
        }
    }
    // Build save file path
    fs::path savePath = fs::path("Save") / saveName;
    // Check if save already exists
    if (fs::exists(savePath)) {
        cout << "Save already exists: " << saveName << endl;
        cout << "Overwrite ? (y/n): ";
        char choice;
        cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            return false;
        }
    }
    // Check save limit
    else if (nbFile >= 5) {
        cout << "Maximum number of saves reached (5)." << endl;
        cout << "Overwrite selected save ? (y/n): ";
        char choice;
        cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            return false;
        }
    }

    // Create or overwrite the save file
    ofstream saveFile(savePath);
    if (!saveFile.is_open()) {
        cerr << "Failed to create save file." << endl;
        return false;
    }
    saveFile.close();
    cout << "Save created successfully: " << savePath << endl;
    return true;
}

/**
 * @brief update the save of the game with the current game state on the save file.
 *  -if file is not found , a new file with the new filename is created
 *
 * @param aGame the current game state.
 * @param saveName the name of the save
 */
void updateSave(const Game& aGame , string& saveName) {
    const int SIZE = aGame.itsBoard.itsSize;
    fs::path savePath = fs::path("Save") / saveName;
    if (fs::exists(savePath)) {
        //Clear the file and open it
        ofstream oFile(savePath, ios::trunc);
        if (!oFile.is_open()) {
            cout << "Error of save";
        }
        oFile << aGame.itsPlayer1.itsName << endl << aGame.itsPlayer2.itsName << endl; //Write player 1 and player 2
        oFile << aGame.itsCurrentPlayer->itsRole << endl; //Write role of current player
        oFile << aGame.itsBoard.itsSize << endl; //Write the size of the board
        //Loop for write all piece of the board
        for (int line = 0 ; line <SIZE ; line ++) {
            for (int col = 0 ; col <SIZE ; col ++) {
                if (aGame.itsBoard.itsCells[line][col].itsPieceType == NONE) {
                    oFile <<"☒";
                }
                if (aGame.itsBoard.itsCells[line][col].itsPieceType == SWORD) {
                    oFile <<"S";
                }
                if (aGame.itsBoard.itsCells[line][col].itsPieceType == KING) {
                    oFile <<"K";
                }
                if (aGame.itsBoard.itsCells[line][col].itsPieceType == SHIELD) {
                    oFile <<"s";
                }
            }
            oFile << endl;
        }
        oFile.close();
    }
    else {
        cout <<"No file found create new file :" << endl;
        createSave(saveName);
    }
}

/**
 * @brief delete a selected save.
 *  -if save name does not exist : return false and do nothing
 *  -delete only saves in the folder /Save
 *  -if 2 saves have a same name : select one of them (number between 1 & 5)
 *
 * @param saveName Name of the save to delete
 *
 * @return  if the deletion was successfully carried out
 */
bool deleteSave(std::string& saveName) {
    const std::filesystem::path saveDir = "Save";

    //
    if (!fs::exists(saveDir) || !fs::is_directory(saveDir)) {
        //error message with std::cerr
        cerr << "Error: Save directory : '" << saveDir.string()
                  << " does not exist or is not a directory." << std::endl;
        return false;
    }

    //max size for selection (max 5 files in the folder)
    constexpr size_t MAX_OPTIONS = 5;
    fs::path paths[MAX_OPTIONS]; // Tableau C-style pour stocker les chemins
    size_t matchCount = 0;
    fs::path uniquePath;

    //stock the 5 files with a loop (directory iterator)
    for (const auto& entry : fs::directory_iterator(saveDir)) {
        if (entry.is_regular_file()) {
            //test if filename has same name of the tested file
            if (entry.path().filename().string() == saveName) {
                if (matchCount == 0) {
                    uniquePath = entry.path();
                }
                //test
                if (matchCount < MAX_OPTIONS) {
                    paths[matchCount] = entry.path();
                }
                matchCount++;
            }
        }
    }

    if (matchCount == 0) {
        //no file :
        std::cout << "No save found : " << saveName << endl;
        return false;
    }

    fs::path pathToDelete;

    if (matchCount == 1) {
        // one file
        pathToDelete = uniquePath;
        std::cout << "Deletion of the single backup: " << pathToDelete.string() << endl;

    } else {

        cout << "Several backups were found with the name '" << saveName << "':" << endl;

        size_t displayCount = (matchCount > MAX_OPTIONS) ? MAX_OPTIONS : matchCount;

        // display options
        for (size_t i = 0; i < displayCount; i++) {
            cout << "[" << (i + 1) << "] " << paths[i].string() << endl;
        }
        if (matchCount > MAX_OPTIONS) {
            cout << "[...] and " << (matchCount - MAX_OPTIONS) << " others." << endl;
        }
        cout << "[0] Exit" << endl;
        cout << "Choose the save to delete (1-" << displayCount << ") : ";

        // read user choice
        int choice = -1;
        cin >> choice;

        if ( choice < 1 || choice > (int)displayCount) {
            //verification of cancel
            if (choice == 0) {
                cout << "Operation canceled." << endl;
            } else {
                cout << "invalid choice. canceling the delete." << endl;
            }
        }
        //the path to delete is the selected number :
        pathToDelete = paths[choice - 1];
        std::cout << "deleting the save : " << pathToDelete.string() << endl;
    }

    //delete safely the save with try/catch
    try {
        bool success = fs::remove(pathToDelete);
        if (success) {
            cout << "The file has been successfully deleted";
        }
        return success;
    } catch (const fs::filesystem_error& e) {
        cerr << "error of delete of '" << pathToDelete.string() << "': "<< e.what() << endl; //e.what() = error type to display
        return false;
    }
}

/**
 * @brief display a save selection menu
 *  -display 5 line for the 5 saves
 *  -if a save don't exist : display free space
 *  -else display the save name
 *  -display the actions possible :
 *      -Load a save 1
 *      -Delete a save 2
 *      -Exit save manager 0
 */
bool saveManager(Game &aGame , string &saveName) {
    cout << "══════════════════════════════════════════════════════"<<endl;
    int count =1;
    for (const auto& entry : fs::directory_iterator("Save") ) {

        cout <<count<<" : "<< entry.path().filename().string()<<endl;
        count++;
    }
    cout << "══════════════════════════════════════════════════════"<<endl;
    int userInput = -1;
    while (userInput !=0 && userInput!=1 && userInput!=2) {
        cout << "[0] for exit  - [1]for load save  - [2]for delete save"<<endl;
        cin >> userInput;
    }
    string delName;
    if (userInput ==0) {
        cout << "Exiting save manager" << endl;
        clearConsole();
        return false;
    }
    else if (userInput == 1) {
        cout << "Enter a file name to load : ";
        cin >> saveName;
        loadSave(aGame,saveName);
    }
    else if (userInput == 2) {
        cout <<"Enter a file to delete : ";
        cin >>delName;
        deleteSave(delName);
        return false;
    }
    else {
        cout <<"Invalid input , exiting save manager" << endl;
        return false;
    }
    return true;
}

/**
 * @brief Load a selected save for continue to play it
 *  -if no save in folder /Save display an error message (No save to load) and return false
 *  -check if lines contains the goods information (currentPlayer, boardSize , Board )
 *  -if information are incorrect :
 *      -size != 11 or 13
 *      -currentPlayer != itsPlayer1 or itsPlayer2
 *      -if board is missing or invalid (example 2 kings on the board)
 * @param saveName name of the save
 *
 * @return if save was successfully loaded
 */
bool loadSave(Game &aGame, string& saveName) {
    string fileToLoad="Save/"+saveName;
    ifstream iFile(fileToLoad);
    if (!iFile.is_open()) {
        std::cerr << "Error: impossible to read the file : " << saveName << std::endl;
        return false;
    }
    string player1 , player2 , currentPlayer ,boardSize;

    if (getline(iFile,player1)) {
        aGame.itsPlayer1.itsName = player1;
    }
    else {
        cerr << "Loading Error";
        return false;
    }
    if (getline(iFile,player2)) {
        aGame.itsPlayer2.itsName = player2;
    }
    else {
        cerr << "Loading Error";
        return false;
    }
    if (getline(iFile,currentPlayer)) {
        if (currentPlayer == "0") {
            aGame.itsCurrentPlayer = &aGame.itsPlayer1;
        }
        else {
            aGame.itsCurrentPlayer = &aGame.itsPlayer2;
        }
    }

    else {
        cerr << "Loading Error";
        return false;
    }
    if (getline(iFile,boardSize)) {
        if (boardSize == "11") {
            aGame.itsBoard.itsSize = LITTLE;
        }
        else {
            aGame.itsBoard.itsSize = BIG;
        }
        createBoard(aGame.itsBoard);
    }
    else {
        cerr << "Loading Error";
        return false;
    }
    int N = aGame.itsBoard.itsSize;

    char car;
    int kingCount = 0; //for test if 2king are on the board
    for (int line = 0; line < N; line++) {

        for (int col = 0; col < N; col++) {
            if ((line == 0 || line == N-1) && (col == 0 || col == N-1)) {
                aGame.itsBoard.itsCells[line][col].itsCellType=FORTRESS;
            }
            else if (N==11 && line ==5 && col ==5 || N==11 && line ==6 && col ==6) {
                aGame.itsBoard.itsCells[line][col].itsCellType=CASTLE;
            }
            else {
                aGame.itsBoard.itsCells[line][col].itsCellType=NORMAL;
            }

            if (!iFile.get(car)) {
                cerr << "Error: EOF at (" << line << ", " << col << ")" << endl;
                iFile.close();
                return false;
            }

            if (car == 'K') {
                kingCount++;
                aGame.itsBoard.itsCells[line][col].itsPieceType = KING;
            } else if (car == 'S') {
                aGame.itsBoard.itsCells[line][col].itsPieceType = SWORD;
            } else if (car == 's') {
                aGame.itsBoard.itsCells[line][col].itsPieceType = SHIELD;
            } else {
                aGame.itsBoard.itsCells[line][col].itsPieceType = NONE;
            }
        }
        iFile.get(car);
    }
    return true;
}
