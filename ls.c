#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <assert.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[92m"
#define ANSI_COLOR_GRAY    "\x1b[2m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[95m"
#define ANSI_COLOR_CYAN    "\x1b[96m"
#define ANSI_COLOR_WHITE   "\x1b[97m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define MAX_PATH 260
#define MAX_EXT 5

// "Greater Than String Length"
// gt_strlen("dog", 4) -> false
// gt_strlen("dogs", 4) -> false  
// gt_strlen("doggo", 4) -> true 
int gt_strlen(const char* string, size_t max_length)
{
    if (string == NULL) {
        puts("Function received null pointer");
        return 0;
    }
    for (int i = 0; i <= max_length; i++)
    {
        if (string[i] == '\0') 
        {
            return 0;
        }
    }
    return 1;
}


// abbreviate string to a length of `n`
void abrv8_fname(char* filename, size_t max_length)
{
    //!DEBUG!
    printf("abrv_fname:\n%s\n\n", filename);

    if (max_length <= MAX_EXT) {
        return;
    }
    // check if string length greater than length
    if (!gt_strlen(filename, max_length)) {
        return;
    }
    /* greedy implementation expects mostly early returns,
       hence the redundancy of a custom strlen */
    
    size_t filename_len = strlen(filename);
    char file_ext[MAX_EXT] = "\0\0\0\0\0";
    int has_ext = 0;
    /*
    len 7
    
    cat.png
    ^^^^^^^
    0123456
    
            2 = (7 - 5)
    start idx = len - MAX_EXT 

    cat.png
      ^
      2 start idx

    cpy to buffer after '.':
    
    cat.png
      ^^^^^
       read (5 chars)

    cat.png
       ^^^^
      copied (4 chars)

    buffer = ".png";
    */
    size_t ext_idx = 0;
    for (int i = 0; i < MAX_EXT; i++) 
    {
        if (filename[filename_len - MAX_EXT + i] == '.') {
            has_ext = 1;
        }   
        if (has_ext) {
            file_ext[ext_idx] = filename[filename_len - MAX_EXT + i];
            ext_idx++;
            filename[filename_len - MAX_EXT + i] = '\0';
        }
    }
    //!DEBUG!
    printf("ext truncated:\n%s\n\n", filename);
    size_t ext_len;
    if (has_ext) {
        ext_len = strlen(file_ext);
    } else {
        ext_len = 0;
    }
    // len = 20; max = 10;
    // quick_brown_dogs.png
    // ->
    // quick...s.png
    //
    // len = 20; max = 10 + 3;
    // quick_brown_dogs.png
    // ->
    // quick_...ogs.png
    //      ^   ^^
    //      1   23
    
    int remainder = max_length % 2;
    
    char *tmp_ptr = filename;
    
    tmp_ptr += max_length / 2;
    
    // quick_...wn_dogs.png
    //tmp_ptr^

    strncpy(tmp_ptr, "~", 1);
    //!DEBUG!
    printf("insertion:\n%s\n\n", filename);
    char *cpy_ptr = filename;
    tmp_ptr += 1;
    size_t cpy_offset = filename_len - (max_length / 2) + remainder; 
    cpy_ptr += cpy_offset;

    // quick_...wn_dogs.png
    //   tmp_ptr^

    strncpy(tmp_ptr, cpy_ptr, (max_length / 2) + remainder - ext_len);
    //!DEBUG!
    printf("overwritten:\n%s\n\n", filename);
    if (has_ext) {
        strcat(filename, file_ext);
    }

    filename[max_length + 1] = '\0';
    //!DEBUG!
    printf("ext appended:\n%s\n\n", filename);
}
// abbreviate string to a length of `n`
void abrv_fname(char* filename, size_t max_length)
{
    size_t filename_len = strlen(filename);
    
    if (filename_len < max_length) {
        return;
    }

    size_t remainder = max_length % 2;
    size_t half_max_length = max_length / 2;
    
    char buffer[MAX_PATH] = "";

    // copy half_max_length characters into buffer
    strncpy(buffer, filename, half_max_length);

    // indicate tear in string
    strcat(buffer, "*");

    char *temp_ptr = filename;

    // advance ptr to end of filename //! remove + 1 to ignore tear "*"
    temp_ptr += ((filename_len) - (half_max_length + remainder) + 1);

    strcat(buffer, temp_ptr);

    strcpy(filename, buffer);
}
int is_windows_terminal() {
    char* term = getenv("WT_SESSION");
    return term != NULL;
}

int set_color( const char color[], int ansi_color_support ) {
    // Color documenation for later me:
    // https://learn.microsoft.com/en-us/windows/console/console-screen-buffers#character-attributes
    // TMI for something that won't be used. Maybe look into using ncurses later
    if (ansi_color_support) {
	printf("%s", color);
    }
    //puts("ANSI colors not supported");
}
	
int is_image(const char* filename) {
    const char* extensions[] = {".png", ".tiff", ".tif", ".jpg", ".jpeg", 
                                ".gif", ".svg", ".bmp", ".ico", ".heif", 
                                ".avif", ".ppm"};
    int num_extensions = sizeof(extensions) / sizeof(extensions[0]);

    for (int i = 0; i < num_extensions; i++) {
        if (strstr(filename, extensions[i]) != NULL) {
            return 1; 
        }
    }
    return 0; 
}
void fast_strcat(char dest[], const char source[], int offset) {
    int i = 0;
    while (source[i] != '\0') {
        dest[offset] = source[i];
        offset ++;
        i++;
    }
}

int main(int argc, char *argv[]) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hConsole == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to get console handle (%d)\n", GetLastError());
        return 1;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int cs_width;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        cs_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    } else {
        fprintf(stderr, "Failed to get console screen buffer info (%d)\n", GetLastError());
        return 1;
    }

    LPCSTR path = "C:\\Users\\dento\\Desktop\\Visual Studio Workspaces\\cbin\\";

    if(SetCurrentDirectory(path)) {
        printf("Working directory changed successfully to %s\n", path);
    } else {
        printf("Failed to change the working directory. Error code: %lu\n", GetLastError());
    }

    // some tests
    assert(gt_strlen("dog", 4) == 0);
    assert(gt_strlen("dogs", 4) == 0);
    assert(gt_strlen("doggo", 4) == 1);

    int flag_n_present = 0;  // set this if "-n" flag found
    int padding_flag = 0;
    int disable_abbreviate_flag = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            flag_n_present = 1;
        }
    	if (strcmp(argv[i], "-p") == 0) {
	    padding_flag = 1;
    	}
        if (strcmp(argv[i], "-a") == 0) {
	    disable_abbreviate_flag = 1;
    	}
    }

    int ansi_color_support = is_windows_terminal(); 
    char terminus[2] = "\0\0";
    if (flag_n_present) {
        terminus[0] = '\n';
    } else {
        terminus[0] = ' ';
    }
    char filename_list_buffer[MAX_PATH * 4096];
    char filename_buffer[MAX_PATH];
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(".\\*", &findFileData);
    // find the appealing colors
    // for(int i = 0; i <= 255; i++) {
    //     printf("\x1b[%dm %d: ANSI color code %d \x1b[0m\n", i, i, i);
    // }

    int str_per_cs_width = cs_width / 20;  
    printf("cs_width: %d\nstrings_per_console_width: %d\n", cs_width, str_per_cs_width);
    int strings = 0;
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return 1;
    } 
    do {
        if (!disable_abbreviate_flag) {
            abrv_fname(findFileData.cFileName, 18);
        }
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Bright Cyan for directories
            sprintf(filename_buffer, "%s%-20s%s%s", ANSI_COLOR_CYAN, findFileData.cFileName, ANSI_COLOR_RESET, terminus);
            strcat(filename_list_buffer, filename_buffer);
        } else {
            if (strstr(findFileData.cFileName, ".exe") != NULL) {
                // Bright Green for .exe
                sprintf(filename_buffer, "%s%-20s%s%s", ANSI_COLOR_GREEN, findFileData.cFileName, ANSI_COLOR_RESET, terminus);
                strcat(filename_list_buffer, filename_buffer);
            } else if (is_image(findFileData.cFileName)) {
                // Linux uses Magenta/Fuchsia for images, but color not available in Command Prompt
                sprintf(filename_buffer, "%s%-20s%s%s", ANSI_COLOR_GRAY, findFileData.cFileName, ANSI_COLOR_RESET, terminus);
                strcat(filename_list_buffer, filename_buffer);
            } else {
                // Bright White for files
                sprintf(filename_buffer, "%s%-20s%s%s", ANSI_COLOR_WHITE, findFileData.cFileName, ANSI_COLOR_RESET, terminus);
                strcat(filename_list_buffer, filename_buffer);
            }
        }
        // Wrap to console width
        if (!flag_n_present) {
            strings++; 
            if (strings == str_per_cs_width) {
                strcat(filename_list_buffer, "\n");
                strings = 0;
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    printf(filename_list_buffer);
    FindClose(hFind);
    return 0;
}
