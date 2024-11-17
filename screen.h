

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdio.h>

/* ANSI Terminal Control Sequences */
#define ESC            "\033"      // Escape sequence start
#define NORMALTEXT     "[0m"       // Normal text
#define BOLDTEXT       "[1m"       // Bold text
#define ITALICTEXT     "[3m"       // Italic text (not supported in all terminals)
#define BLINKTEXT      "[5m"       // Blinking text
#define REVERSETEXT    "[7m"       // Reverse video mode
#define HOMECURSOR     "[H"        // Move cursor to home position
#define SHOWCURSOR     "[?25h"     // Show cursor
#define HIDECURSOR     "[?25l"     // Hide cursor
#define CLEARSCREEN    "[2J"       // Clear screen

/* Box Drawing Characters for Unix-Like Terminals */
#define BOX_ENABLE     "(0"        // Enable box drawing mode
#define BOX_DISABLE    "(B"        // Disable box drawing mode
#define BOX_VLINE      0x78        // Vertical line
#define BOX_HLINE      0x71        // Horizontal line
#define BOX_UPLEFT     0x6C        // Upper left corner
#define BOX_UPRIGHT    0x6B        // Upper right corner
#define BOX_DWNLEFT    0x6D        // Lower left corner
#define BOX_DWNRIGHT   0x6A        // Lower right corner
#define BOX_CROSS      0x6E        // Cross
#define BOX_TLEFT      0X74        // T-Shape left
#define BOX_TRIGHT     0X75        // T-Shape right
#define BOX_TUP        0X77        // T-Shape up
#define BOX_TDOWN      0X76        // T-Shape down
#define BOX_DIAMOND    0x60        // Diamond
#define BOX_BLOCK      0x61        // Block
#define BOX_DOT        0x7E        // Dot

/* Screen Boundaries */
#define SCRSTARTX      3           // Initial horizontal position for the game area
#define SCRENDX        75          // Final horizontal position for the game area
#define SCRSTARTY      1           // Initial vertical position for the game area
#define SCRENDY        23          // Final vertical position for the game area

#define MINX           1           // Minimum screen horizontal position
#define MINY           1           // Minimum screen vertical position
#define MAXX           80          // Maximum screen horizontal position
#define MAXY           24          // Maximum screen vertical position

/**
 * Screen Colors Enum
 */
typedef enum {
    BLACK, RED, GREEN, BROWN, BLUE, MAGENTA, CYAN, LIGHTGRAY,
    DARKGRAY, LIGHTRED, LIGHTGREEN, YELLOW, LIGHTBLUE, LIGHTMAGENTA, LIGHTCYAN, WHITE
} screenColor;

/* Inline Functions for Terminal Control */

/**
 * Move the cursor to the top-left corner (home position).
 */
static inline void screenHomeCursor()
{
    printf("%s%s", ESC, HOMECURSOR);
}

/**
 * Show the terminal cursor.
 */
static inline void screenShowCursor()
{
    printf("%s%s", ESC, SHOWCURSOR);
}

/**
 * Hide the terminal cursor.
 */
static inline void screenHideCursor()
{
    printf("%s%s", ESC, HIDECURSOR);
}

/**
 * Clear the screen and move cursor to the home position.
 */
static inline void screenClear()
{
    printf("%s%s", ESC, CLEARSCREEN);
    screenHomeCursor();
}

/**
 * Immediately update the screen by flushing stdout.
 */
static inline void screenUpdate() {
    fflush(stdout);
}

/**
 * Set text display mode to normal.
 */
static inline void screenSetNormal()
{
    printf("%s%s", ESC, NORMALTEXT);
}

/**
 * Set text display mode to bold.
 */
static inline void screenSetBold()
{
    printf("%s%s", ESC, BOLDTEXT);
}

/**
 * Set text display mode to blink (may not work on all terminals).
 */
static inline void screenSetBlink()
{
    printf("%s%s", ESC, BLINKTEXT);
}

/**
 * Set text display mode to reverse (swap foreground and background colors).
 */
static inline void screenSetReverse()
{
    printf("%s%s", ESC, REVERSETEXT);
}

/**
 * Enable box drawing characters for ASCII art and UI borders.
 */
static inline void screenBoxEnable()
{
    printf("%s%s", ESC, BOX_ENABLE);
}

/**
 * Disable box drawing characters, returning to normal text.
 */
static inline void screenBoxDisable()
{
    printf("%s%s", ESC, BOX_DISABLE);
}

/* Core Screen Functions */

/**
 * Initialize the screen, clear it, and optionally draw borders.
 * 
 * @param drawBorders If non-zero, draws borders around the screen.
 */
void screenInit(int drawBorders);

/**
 * Restore screen to initial state and clear any drawn elements.
 */
void screenDestroy();

/**
 * Move the cursor to a specified position.
 * 
 * @param x X position (horizontal).
 * @param y Y position (vertical).
 */
void screenGotoxy(int x, int y);

/**
 * Set foreground and background text colors.
 * 
 * @param fg Foreground color, can be any value from BLACK to WHITE.
 * @param bg Background color, can be any value from BLACK to LIGHTGRAY.
 */
void screenSetColor(screenColor fg, screenColor bg);

#endif /* __SCREEN_H__ */
