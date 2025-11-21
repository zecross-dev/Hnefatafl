/**
 * @file typedef.h
 *
 * @brief This file defines various enums and structures used in the game.
 *
 * @author JMB - IUT Informatique La Rochelle
 * @date 10/11/2025
 */

#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <string>
using namespace std;


/**
 * @enum BoardSize
 * @brief Represents possible board sizes for the game.
 *
 * The two board sizes available are:
 * - `LITTLE`: 11x11 board.
 * - `BIG`: 13x13 board.
 */
enum BoardSize
{
    LITTLE = 11,  /**< Represents the smaller board size (11x11). */
    BIG = 13      /**< Represents the larger board size (13x13). */
};

/**
 * @enum PlayerRole
 * @brief Represents the roles of the players in the game.
 *
 * The two roles available are:
 * - `ATTACK`: Represents the attacker.
 * - `DEFENSE`: Represents the defender.
 */
enum PlayerRole
{
    ATTACK,   /**< Represents the role of the attacker. */
    DEFENSE   /**< Represents the role of the defender. */
};

/**
 * @enum CellType
 * @brief Represents the types of cells on the game board.
 *
 * - `NORMAL`: A regular cell without any special properties.
 * - `FORTRESS`: A cell that acts as a fortress.
 * - `CASTLE`: A cell that acts as a castle.
 */
enum CellType
{
    NORMAL,    /**< Represents a normal game board cell. */
    FORTRESS,  /**< Represents a fortress cell. */
    CASTLE     /**< Represents a castle cell. */
};

/**
 * @enum PieceType
 * @brief Represents the types of pieces that can occupy a cell.
 *
 * - `NONE`: Indicates no piece is present.
 * - `SHIELD`: Represents a shield piece.
 * - `SWORD`: Represents an attacker (sword) piece.
 * - `KING`: Represents the king piece.
 */
enum PieceType
{
    NONE,    /**< Represents an empty cell (no piece). */
    SHIELD,  /**< Represents a shield piece. */
    SWORD,   /**< Represents a sword piece. */
    KING     /**< Represents the king piece. */
};

/**
 * @struct Cell
 * @brief Structure to represent the state of a single cell on the board.
 *
 * Each cell has a type (`itsCellType`) and a piece (`itsPieceType`),
 * which can be empty or occupied by a specific piece.
 */
struct Cell
{
    CellType itsCellType;   /**< The type of the cell (e.g., NORMAL, FORTRESS, CASTLE). */
    PieceType itsPieceType; /**< The type of piece occupying the cell (e.g., NONE, SHIELD, SWORD, KING). */
};

/**
 * @struct Board
 * @brief Structure representing the game board as a 2D grid of `Cell` structures.
 *
 * The board contains a set of cells arranged in a grid with a size defined by `itsSize`.
 */
struct Board
{
    Cell** itsCells = nullptr;  /**< 2D array representing the cells on the board. */
    BoardSize itsSize = LITTLE; /**< The size of the board (LITTLE or BIG). */
};

/**
 * @struct Position
 * @brief Structure to represent the coordinates of a cell on the board.
 *
 * Each position is defined by a row (`itsRow`) and a column (`itsCol`) within the board grid.
 */
struct Position
{
    int itsRow;  /**< The row (vertical position) of a cell. */
    int itsCol;  /**< The column (horizontal position) of a cell. */
};

/**
 * @struct Move
 * @brief Structure to represent a move made by a player.
 *
 * A move is defined by the starting position (`itsStartPosition`)
 * and the ending position (`itsEndPosition`).
 */
struct Move
{
    Position itsStartPosition; /**< The starting position of the move. */
    Position itsEndPosition;   /**< The ending position of the move. */
};

/**
 * @struct Player
 * @brief Structure representing a player in the game.
 *
 * Each player has a name (`itsName`) and a role (`itsRole`),
 * which can either be ATTACK or DEFENSE.
 */
struct Player
{
    string itsName;         /**< The name of the player. */
    PlayerRole itsRole;     /**< The role of the player (ATTACK or DEFENSE). */
};

/**
 * @struct Game
 * @brief Structure representing the state of the game.
 *
 * The game consists of a `Board` and two players (`itsPlayer1` and `itsPlayer2`),
 * with a pointer to the current player (`itsCurrentPlayer`).
 */
struct Game
{
    Board itsBoard;             /**< The game board. */
    Player itsPlayer1 = {"Player 1", ATTACK}; /**< The first player (attacker). */
    Player itsPlayer2 = {"Player 2", DEFENSE}; /**< The second player (defender). */
    Player* itsCurrentPlayer = &itsPlayer1; /**< A pointer to the current player. */
};

#endif // TYPEDEF_H
