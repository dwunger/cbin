#include <windows.h>
#include <stdio.h>
#include <time.h>

#define UP_SYMBOL    '^'
#define DOWN_SYMBOL  'v'
#define LEFT_SYMBOL  '<'
#define RIGHT_SYMBOL '>'

// and so all your directions are now become mine
char convert_direction_to_vim_mapping(char *direction) {
    switch (*direction)
    {
    case LEFT_SYMBOL:
        *direction = 'h'; 
        break;
    case RIGHT_SYMBOL:
        *direction = 'l'; 
        break;
    case UP_SYMBOL:
        *direction = 'k'; 
        break;
    case DOWN_SYMBOL:
        *direction = 'j'; 
        break;
    default:
        break;
    }
}

int main(void) {
    HANDLE user_input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD originalMode, newMode, bytesRead;

    // Get the original input mode
    GetConsoleMode(user_input, &originalMode);

    // Remove line input and echo input flags to simulate raw mode
    newMode = originalMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(user_input, newMode);

    // Set console mode to UTF-16 (Windows uses UTF-16 for Unicode)
    SetConsoleOutputCP(CP_UTF8);

    char array[4] = {UP_SYMBOL, DOWN_SYMBOL, LEFT_SYMBOL, RIGHT_SYMBOL};
    size_t length = sizeof(array)/sizeof(array[0]);

    char c;
    while (1) {
        srand((unsigned int) time(NULL));
        char *direction_symbol = array[rand() % length];
        printf("%c\n", *direction_symbol);
        convert_direction_to_vim_mapping(*direction_symbol);
        if (ReadFile(user_input, &c, sizeof(char), &bytesRead, NULL) && bytesRead == sizeof(char)) {
            if (c == 'q') { 
            break;  // Quit on 'q'
            }
            else if ( c != *direction_symbol) {
                puts("get good");
            }

            printf("You pressed: %c\n", c);
        }

    }

    // Restore original mode
    SetConsoleMode(user_input, originalMode);
    return 0;
}
