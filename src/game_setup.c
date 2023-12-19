#include "game_setup.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
// Some handy dandy macros for decompression
#define E_CAP_HEX 0x45
#define E_LOW_HEX 0x65
#define S_CAP_HEX 0x53
#define S_LOW_HEX 0x73
#define W_CAP_HEX 0x57
#define W_LOW_HEX 0x77
#define DIGIT_START 0x30
#define DIGIT_END 0x39

/** Initializes the board with walls around the edge of the board.
 *
 * Modifies values pointed to by cells_p, width_p, and height_p and initializes
 * cells array to reflect this default board.
 *
 * Returns INIT_SUCCESS to indicate that it was successful.
 *
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 */
enum board_init_status initialize_default_board(int** cells_p, size_t* width_p,
                                                size_t* height_p) {
    *width_p = 20;
    *height_p = 10;
    int* cells = malloc(20 * 10 * sizeof(int));

    *cells_p = cells;
    for (int i = 0; i < 20 * 10; i++) {
        cells[i] = FLAG_PLAIN_CELL;
    }
  
    // Set edge cells!
    // Top and bottom edges:
    for (int i = 0; i < 20; ++i) {
        cells[i] = FLAG_WALL;
        cells[i + (20 * (10 - 1))] = FLAG_WALL;
    }
    // Left and right edges:
    for (int i = 0; i < 10; ++i) {
        cells[i * 20] = FLAG_WALL;
        cells[i * 20 + 20 - 1] = FLAG_WALL;
    }

    // Add snake
    cells[20 * 2 + 2] = FLAG_SNAKE;

    return INIT_SUCCESS;

}

/** Initialize variables relevant to the game board.
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 *  - snake_p: a pointer to your snake struct (not used until part 2!)
 *  - board_rep: a string representing the initial board. May be NULL for
 * default board.
 */

enum board_init_status initialize_game(int** cells_p, size_t* width_p,
                                       size_t* height_p, snake_t* snake_p,
                                       char* board_rep) {
    // TODO: implement! 
    enum board_init_status status;

    if (board_rep != NULL) {
        status = decompress_board_str(cells_p, width_p, height_p, snake_p, board_rep);
    } else {
        // Fallback to default initialization if board_rep is NULL
        status = initialize_default_board(cells_p, width_p, height_p);
    }

    // Handle the return status
    switch (status) {
        case INIT_SUCCESS:
            g_game_over = 0;
            g_score = 0;
            place_food(*cells_p, *width_p, *height_p);
            break;
        case INIT_ERR_INCORRECT_DIMENSIONS:
        case INIT_ERR_WRONG_SNAKE_NUM:
        case INIT_ERR_BAD_CHAR:
        case INIT_UNIMPLEMENTED:
            // Handle error statuses accordingly
            // You might want to print an error message or exit the program
            break;
    }

    return status;
}



// Helper function to convert a string to an integer
int str_to_int(const char* str) {
    return atoi(str);
}

// Helper function to calculate the index of a cell in the cells array
size_t get_cell_index(size_t row, size_t col, size_t width) {
    return row * width + col;
}

// Helper function to handle errors and set appropriate return status
enum board_init_status handle_error(enum board_init_status error_status, int** cells, size_t* width_p, size_t* height_p) {
    free(*cells); // Free allocated memory if an error occurs
    *cells = NULL;
    *width_p = 0;
    *height_p = 0;
    return error_status;
}
char key_input[] = "RRDLL";
// Main function to decompress the board string
enum board_init_status decompress_board_str(int** cells_p, size_t* width_p, size_t* height_p, snake_t* snake_p, char* compressed) {
    
    if (compressed == NULL) {
        return INIT_SUCCESS;  // No compression, use default board
    }

    char* token = strtok(compressed, "|");  // Process the first token to get the dimensions
    if (token == NULL || token[0] != 'B') {
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }

    token++;  // Move to the next character after 'B'

    if (token == NULL || sscanf(token, "%zux%zu", height_p, width_p) != 2) {
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }

    // Allocate memory for the cells
    *cells_p = (int*)malloc((*width_p) * (*height_p) * sizeof(int));

    // Initialize the cells to FLAG_PLAIN_CELL
    for (size_t i = 0; i < (*width_p) * (*height_p); ++i) {
        (*cells_p)[i] = FLAG_PLAIN_CELL;
    }

    // Iterate through each row and decompress
    size_t row = 0;
    while ((token = strtok(NULL, "|")) != NULL) {
        // Check if the row exceeds the specified height
        
        if (row >= *height_p) {
            return INIT_ERR_INCORRECT_DIMENSIONS;
        }
        
        // Decompress the row
        size_t col = 0;
        while (*token != '\0') {
            char cell_type = *token;
            ++token;
            
             // Check for valid characters
            if (cell_type != 'W' && cell_type != 'E' && cell_type != 'S') {
                return INIT_ERR_BAD_CHAR;
            }

            int count = 1;
               
            // Check if a count is specified
            if (*token >= '0' && *token <= '9') {
                sscanf(token, "%d", &count);
                while (*token >= '0' && *token <= '9') {
                    ++token;
                }
            }
            
            // Check if the row length exceeds the specified width
            if (col + count > *width_p) {
                return INIT_ERR_INCORRECT_DIMENSIONS;
            }

            // Fill in the cells
            for (int i = 0; i < count; ++i) {
                size_t index = row * (*width_p) + col;
                (*cells_p)[index] = (cell_type == 'W') ? FLAG_WALL : ((cell_type == 'S') ? FLAG_SNAKE : FLAG_PLAIN_CELL);
                ++col;
            }
        }
        // Check if the row length matches the specified width
        if (col != *width_p) {
            return INIT_ERR_INCORRECT_DIMENSIONS;
        }
        ++row;
    }
    // Check if the number of rows matches the specified height
    if (row != *height_p) {
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }
// Validate the board for the wrong number of snake cells
    size_t snake_count = 0;
    for (size_t i = 0; i < (*width_p) * (*height_p); ++i) {
        if ((*cells_p)[i] & FLAG_SNAKE) {
            ++snake_count;
            if (snake_count > 1) {
                return handle_error(INIT_ERR_WRONG_SNAKE_NUM, cells_p, width_p, height_p);
            }
        }
    }

    // Check if there's no snake cell (optional: you may want to check if there's exactly one initial snake cell)
    if (snake_count == 0) {
        return handle_error(INIT_ERR_WRONG_SNAKE_NUM, cells_p, width_p, height_p);
    }

    if (snake_p != NULL && key_input != NULL) {
        for (const char* key = key_input; *key != '\0'; ++key) {
            // Process each key input (you may need to update this logic based on your game's input handling)
            switch (*key) {
                case 'U':
                    // Handle UP input
                    // Example: move_snake_up(snake_p);
                    --(snake_p->head_x);
                    break;
                case 'D':
                    // Handle DOWN input
                    // Example: move_snake_down(snake_p);
                     ++(snake_p->head_x);
                    break;
                case 'L':
                    // Handle LEFT input
                    // Example: move_snake_left(snake_p);
                    --(snake_p->head_y);
                    break;
                case 'R':
                    // Handle RIGHT input
                    // Example: move_snake_right(snake_p);
                    ++(snake_p->head_y);
                    break;
                default:
                    // Handle other input or ignore
                    ++(snake_p->head_y);
                    break;
            }
        }
    }
    return INIT_SUCCESS;
}
/** Takes in a string `compressed` and initializes values pointed to by
 * cells_p, width_p, and height_p accordingly. Arguments:
 *      - cells_p: a pointer to the pointer representing the cells array
 *                 that we would like to initialize.
 *      - width_p: a pointer to the width variable we'd like to initialize.
 *      - height_p: a pointer to the height variable we'd like to initialize.
 *      - snake_p: a pointer to your snake struct (not used until part 2!)
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B24x80|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row
 * (delineated by the `|` character), and read out a letter (E, S or W) a number
 * of times dictated by the number that follows the letter.
 */

