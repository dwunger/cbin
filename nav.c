#include <windows.h>
#include <stdio.h>
#include <time.h>

#define UP_KEY    L'↑'
#define DOWN_KEY  L'↓'
#define LEFT_KEY  L'←'
#define RIGHT_KEY L'→'

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

    wchar_t array[4] = {L'↑', L'↓', L'←', L'→'};
    size_t length = sizeof(array)/sizeof(array[0]);

    char c;
    while (1) {
        srand((unsigned int) time(NULL));
        wchar_t expected_input = array[rand() % length];
        wprintf(L"%lc\n", expected_input);
        if (expected_input = UP_KEY) {
            expected_input = 'k';
        }
        if (expected_input = DOWN_KEY) {
            expected_input = 'j';
        }
        if (expected_input = LEFT_KEY) {
            expected_input = 'h';
        }
        if (expected_input = RIGHT_KEY) {
            expected_input = 'l';
        }
        if (ReadFile(user_input, &c, 1, &bytesRead, NULL) && bytesRead == 1) {
            if (c == 'q') break;  // Quit on 'q'
            printf("You pressed: %c\n", c);

        }
    }

    // Restore original mode
    SetConsoleMode(user_input, originalMode);
    return 0;
}
