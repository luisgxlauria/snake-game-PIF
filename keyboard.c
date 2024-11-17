

#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

#include "keyboard.h"

static struct termios initialSettings, newSettings;
static int peekCharacter = -1;
static struct timeval lastKeyPressTime = {0, 0};
static int debounceDelay = 100; // Debounce delay in milliseconds

void keyboardInit()
{
    tcgetattr(0, &initialSettings);
    newSettings = initialSettings;
    newSettings.c_lflag &= ~ICANON;
    newSettings.c_lflag &= ~ECHO;
    newSettings.c_lflag &= ~ISIG;
    newSettings.c_cc[VMIN] = 1;
    newSettings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &newSettings);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // Set non-blocking mode
}

void keyboardDestroy()
{
    tcsetattr(0, TCSANOW, &initialSettings);
}

int keyhit()
{
    unsigned char ch;
    int nread;

    if (peekCharacter != -1) return 1;

    newSettings.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &newSettings);
    nread = read(STDIN_FILENO, &ch, 1);
    newSettings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &newSettings);

    if (nread == 1) 
    {
        peekCharacter = ch;
        return 1;
    }
    
    return 0;
}

int readch()
{
    char ch;

    if (peekCharacter != -1)
    {
        ch = peekCharacter;
        peekCharacter = -1;
        return ch;
    }

    read(STDIN_FILENO, &ch, 1);

    // Handle special characters for arrow keys (ANSI escape codes)
    if (ch == '\033') { // Escape sequence
        char arrowKey[2];
        read(STDIN_FILENO, &arrowKey[0], 1);
        read(STDIN_FILENO, &arrowKey[1], 1);

        if (arrowKey[0] == '[') {
            switch (arrowKey[1]) {
                case 'A': return 'w'; // Up arrow
                case 'B': return 's'; // Down arrow
                case 'C': return 'd'; // Right arrow
                case 'D': return 'a'; // Left arrow
            }
        }
    }

    return ch;
}

int debounceCheck()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    long timeDifference = (currentTime.tv_sec - lastKeyPressTime.tv_sec) * 1000 +
                          (currentTime.tv_usec - lastKeyPressTime.tv_usec) / 1000;

    if (timeDifference >= debounceDelay) {
        lastKeyPressTime = currentTime;
        return 1;
    }
    return 0;
}
