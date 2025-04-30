#ifndef CARD_H
#define CARD_H

typedef struct {
    char color; // ('R' - red, 'B' - blue, 'G' - green, 'Y' - yellow, 'W' - wildcard)
    char value; /*  (0-9, 
                    'R' - reverse, 
                    'S' - skip, 
                    'D' - draw two (draw four when color = 'W'), 
                    'W' - wildcard) */
} Card;

#endif