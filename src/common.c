#include "common.h"

#include <stdlib.h>

// TODO: define global variables needed for your snake! (part 1A)


// Definition of global variables for game status.
int g_game_over;
int g_score;

/** Sets the seed for random number generation.
 * Arguments:
 *  - `seed`: the seed.
 */
void set_seed(unsigned seed) {
    /* DO NOT MODIFY THIS FUNCTION */
    srand(seed);
    /* DO NOT MODIFY THIS FUNCTION */
}
// void initialize_snake(size_t start_x, size_t start_y, enum input_key start_direction) {
//     g_snake.head_x = start_x;
//     g_snake.head_y = start_y;
//     g_snake.direction = start_direction;
// }

/** Returns a random index in [0, size)
 * Arguments:
 *  - `size`: the upper bound for the generated value (exclusive).
 */
unsigned generate_index(unsigned size) {
    /* DO NOT MODIFY THIS FUNCTION */
    return rand() % size;
    /* DO NOT MODIFY THIS FUNCTION */
}