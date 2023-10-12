#include <stdio.h>
#include <windows.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[92m"
#define ANSI_COLOR_GRAY    "\x1b[2m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[95m"
#define ANSI_COLOR_CYAN    "\x1b[96m"
#define ANSI_COLOR_WHITE   "\x1b[97m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define ANSI_COLOR_RED_ENUM     0
#define ANSI_COLOR_GREEN_ENUM   1
#define ANSI_COLOR_GRAY_ENUM    2
#define ANSI_COLOR_BLUE_ENUM    3
#define ANSI_COLOR_MAGENTA_ENUM 4
#define ANSI_COLOR_CYAN_ENUM    5
#define ANSI_COLOR_WHITE_ENUM   6
#define ANSI_COLOR_RESET_ENUM   7

int is_windows_terminal() {
    char* term = getenv("WT_SESSION");
    return term != NULL;
}

void color_printn( char *str, int color, int len ) {
    // switch color `enum` for color string
    if (is_windows_terminal()) {
        
    }
}

int is_image(const char* filename) {
    const char* extensions[] = {".png", ".tiff", ".tif", ".jpg", ".jpeg", ".gif", ".svg", ".bmp", ".ico", ".heif", ".avif", ".ppm"};
    int num_extensions = sizeof(extensions) / sizeof(extensions[0]);

    for (int i = 0; i < num_extensions; i++) {
        if (strstr(filename, extensions[i]) != NULL) {
            return 1; // True
        }
    }
    return 0; // False
}

int main(int argc, char *argv[]) {
    int flag_n_present = 0;  // set this if "-n" flag found
    int padding_flag = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            flag_n_present = 1;
        }
    	if (strcmp(argv[i], "-p") == 0) {
	    padding_flag = 1;
    	}
    }
    char terminus;
    if (flag_n_present) {
        terminus = '\n';
    } else {
        terminus = '\t';
    }

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(".\\*", &findFileData);
    // find the appealing colors
    // for(int i = 0; i <= 255; i++) {
    //     printf("\x1b[%dm %d: ANSI color code %d \x1b[0m\n", i, i, i);
    // }
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return 1;
    } 
    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Bright Cyan for directories
            printf("%s%-16s%s%c", ANSI_COLOR_CYAN, findFileData.cFileName, ANSI_COLOR_RESET, terminus);
        } else {
            if (strstr(findFileData.cFileName, ".exe") != NULL) {
                // Bright Green for .exe
                printf("%s%-16s%s%c", ANSI_COLOR_GREEN, findFileData.cFileName, ANSI_COLOR_RESET, terminus);
            } else if (is_image(findFileData.cFileName)) {
                // Linux uses Magenta/Fuchsia for images, but color not available in Command Prompt
                printf("%s%-16s%s%c", ANSI_COLOR_GRAY, findFileData.cFileName, ANSI_COLOR_RESET, terminus);
            } else {
                // Bright White for files
                printf("%s%-16s%s%c", ANSI_COLOR_WHITE, findFileData.cFileName, ANSI_COLOR_RESET, terminus);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return 0;
}
