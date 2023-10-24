#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <stdint.h>

#define UP_SYMBOL    '^'
#define DOWN_SYMBOL  'v'
#define LEFT_SYMBOL  '<'
#define RIGHT_SYMBOL '>'
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[92m"
#define ANSI_COLOR_GRAY    "\x1b[2m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[95m"
#define ANSI_COLOR_CYAN    "\x1b[96m"
#define ANSI_COLOR_WHITE   "\x1b[97m"
#define ANSI_COLOR_RESET   "\x1b[0m"
void seed_generator() {
    srand((unsigned int) time(NULL));
}

char random_choice(const char *array, size_t length) {

    char choice = array[rand() % length];
    return choice;
}

void get_insult(char insult[]) {
    char insults[][20] = {
        "Git Gud",
        "Noob",
        "L2P",
        "2 EZ",
        "ur trash",
        "just quit",
        "Potato Aim"
    };
    size_t len = sizeof(insults) / sizeof(insults[0]);
    int r = rand() % len;
    strcpy(insult, insults[r]);
}

void random_color(char * ansi_str) {
    char ansi_sequences[][20] = {
        ANSI_COLOR_RED,
        ANSI_COLOR_GREEN,
        ANSI_COLOR_GRAY,
        ANSI_COLOR_BLUE,
        ANSI_COLOR_MAGENTA,
        ANSI_COLOR_CYAN,
        ANSI_COLOR_WHITE,
    };
    size_t options = sizeof(ansi_sequences) / sizeof(ansi_sequences[0]);
    int seed = rand() % options;
    strcpy(ansi_str, ansi_sequences[seed]);
}


void get_exit_phrase(char insult[]) {
    char insults[][20] = {
        "quitter",
        "rage quit",
        "too hard?",
        "sucks2suck",
    };
    size_t len = sizeof(insults) / sizeof(insults[0]);
    int r = rand() % len;
    strcpy(insult, insults[r]);
}
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
    
    uint8_t color_enabled = 1;
    char c;
    char color_str[64] = "";
    while (true) 
    {
        char direction_symbol = random_choice(array, length);
        printf("Score\nCorrect: %.4d Incorrect: %.4d\n", scoreboard.correct, 
               scoreboard.incorrect);
        
        random_color(color_str);

        if (color_enabled) {
            printf("%s", color_str);
        }
        
        printf("%c\n", direction_symbol);

        if (color_enabled) {
            printf("%s", ANSI_COLOR_RESET);
        }
        
        convert_direction_to_vim_mapping(&direction_symbol);

        //if (ReadFile(user_input, &c, sizeof(char), &bytesRead, NULL) && bytesRead == sizeof(char)) 
        c = getch();
        if (c != EOF)
        {
            if (c == 'o') 
            {
                printf("Color: %u  Toggle: 'c'\n", color_enabled/255);
                c = getch();
            }
            if (c == 'c') 
            {
                color_enabled = ~color_enabled;
            }
            if (c == 'q')
            {
                char exit_phrase[20];
                get_exit_phrase(exit_phrase);
                puts(exit_phrase);
                break;  // Quit on 'q'
            }
            else if ( c != direction_symbol) 
            {
                scoreboard.incorrect++;
                char insult[20];
                get_insult(insult);
                puts(insult);
            }
            else
            {
                ClearConsole();
                scoreboard.correct++;
            }
        
	    }

    }

    // Restore original mode
    SetConsoleMode(user_input, originalMode);
    return 0;
}
