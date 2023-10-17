#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define UP_SYMBOL    '^'
#define DOWN_SYMBOL  'v'
#define LEFT_SYMBOL  '<'
#define RIGHT_SYMBOL '>'

void ClearConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };  // Top left corner
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks
    FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);

    // Get the current text attribute
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    // Set the buffer's attributes accordingly
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);

    // Put the cursor at its home coordinates
    SetConsoleCursorPosition(hConsole, coordScreen);
}

// all your directions are now become mine
void convert_direction_to_vim_mapping(char *direction) {
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

void seed_generator() {
    srand((unsigned int) time(NULL));
}

char random_choice(const char *array, size_t length) {

    char choice = array[rand() % length];
    return choice;
}

void configure_terminal(HANDLE *user_input, DWORD *originalMode) {
    // Get the original input mode
    GetConsoleMode(*user_input, originalMode);

    // Remove line input and echo input flags to simulate raw mode
    DWORD newMode = *originalMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(user_input, newMode);

    // Set console mode to UTF-16 (Windows uses UTF-16 for Unicode)
    SetConsoleOutputCP(CP_UTF8);
}
struct score {
    int correct;
    int incorrect;
};

int main(void) {
    HANDLE user_input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD originalMode, bytesRead;
    configure_terminal(&user_input, &originalMode);
    // Array of navigation directions
    char array[4] = {UP_SYMBOL, DOWN_SYMBOL, LEFT_SYMBOL, RIGHT_SYMBOL};
    size_t length = sizeof(array)/sizeof(array[0]);

    struct score scoreboard = {0,0};
    seed_generator();
    
    char c;
    while (true) 
    {

        char direction_symbol = random_choice(array, length);
        printf("Score\nCorrect: %.4d Incorrect: %.4d\n%c\n", scoreboard.correct, scoreboard.incorrect, direction_symbol);

        convert_direction_to_vim_mapping(&direction_symbol);

        if (ReadFile(user_input, &c, sizeof(char), &bytesRead, NULL) && bytesRead == sizeof(char)) 
        {
            if (c == 'q') 
            {
                break;  // Quit on 'q'
            }
            else if ( c != direction_symbol) 
            {
                scoreboard.incorrect++;
                puts("get good");
            }
            else
            {
                scoreboard.correct++;
                ClearConsole();
            }
        
	    }

    }

    // Restore original mode
    SetConsoleMode(user_input, originalMode);
    return 0;
}
