






#include <ncurses.h>
#include <string.h>
#include <ctype.h>

// HEADERS 
//
// Define constants for tuning names and strings
#define NUM_TUNINGS 8
#define POSITION    5

char *tuningNames[NUM_TUNINGS] = {"E", 
                                  "D#",
                                  "D", 
                                  "C#", 
                                  "C",  
                                  "B", 
                                  "A#",
                                  "A"};

char *tuningStrings[NUM_TUNINGS][6] = {
    // Standard Tunings
    {"E", "B", "G", "D", "A", "E"}, // E Standard
    {"D#", "A#", "F#", "C#", "G#", "D#"}, // D# Standard / Eb Standard
    {"D", "A", "F", "C", "G", "D"}, // D Standard
    {"C#", "G#", "F#", "C#", "G#", "C#"}, // C# Standard / Db Standard
    {"C", "G", "E", "B", "F#", "C"}, // C Standard / B Standard
    {"B", "F#", "D#", "A#", "F", "B"}, // B Standard
    {"A#", "F", "D", "A", "E", "A#"}, // A# Standard / Bb Standard
    {"A", "E", "C#", "G#", "D#", "A"} // A Standard
};



// MAIN PROGRAM
void setTuning(char *input){
  // Extract tuning specifier from input
        char tuningSpec[256];
        strcpy(tuningSpec, input + 2); // Skip "t " prefix
        int tuningIndex = -1;
        int rows, cols;
        getmaxyx(stdscr, rows, cols); // Get the dimensions of the terminal screen
 
        // Determine the new tuning index based on the input tuning specifier
        // Modify this part according to your tuning specifications
        if (strcmp(tuningSpec, "e") == 0) {
           tuningIndex = 0; // E Standard
        } else if (strcmp(tuningSpec, "d#") == 0) {
           tuningIndex = 1; // D Standard
        } else {
            // Add cases for other tunings as needed
            mvprintw(LINES - 2, 0, "Invalid tuning specifier"); // Notify user of invalid input
            refresh();
            return;
        }

               // Draw guitar tuning
    for (int i = 2; i < 8; i++) {
        mvprintw(i, 1, " ");
        mvprintw(i, 0, "%s", tuningStrings[tuningIndex][i-2]);
    }
  refresh();

         
      

}


void addNote(char* input)
{
   
   mvprintw(2,POSITION,"%c", &input[1]);
   refresh();
   
}

void handleInput(WINDOW *input_win) {
    char input[256];
    int input_len = 0;
    // Display input prompt
    mvwprintw(input_win, 0, 0, "input command: ");
    wrefresh(input_win); // Refresh input window to make prompt visible

    // Enable cursor visibility
    curs_set(1);

    while (1) {
        int ch = wgetch(input_win); // Get character input
        if (ch == '\n') {
            // Clear input line
            wmove(input_win, 0, 14);
            wclrtoeol(input_win);
            wrefresh(input_win);
            break; // Exit loop on Enter key
        } else if (ch == 127 || ch == KEY_BACKSPACE) { // Handle backspace/delete key
            if (input_len > 0) {
                // Move cursor one position back
                mvwaddch(input_win, 0, 14 + input_len - 1, ' ');
                wmove(input_win, 0, 14 + input_len - 1);
                wrefresh(input_win);
                input[--input_len] = '\0'; // Remove character from input
            }
        } else if (input_len < sizeof(input) - 1) { // Add character to input
            input[input_len++] = ch;
            input[input_len] = '\0'; // Null-terminate input string

            // Display character at cursor position
            mvwaddch(input_win, 0, 14 + input_len - 1, ch);
            wrefresh(input_win);
        }
    }

    // Disable cursor visibility
    curs_set(0);

    // Convert input string to lowercase
    for (int i = 0; input[i]; i++) {
        input[i] = tolower(input[i]);
    }

    // Check for tuning command
    if (strncmp(input, "t ", 2) == 0)
    {
      setTuning(input);
    }else if(strncmp(input, "1 ", 2) == 0)
    {
      addNote(input);
    }
}

int main() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();

    // Create window for input
    WINDOW *input_win = newwin(1, COLS, LINES - 1, 0);
    scrollok(input_win, FALSE);

    // Display help message
    mvprintw(LINES - 2, 0, "(? for help)");

 
   // Draw guitar tabs for E Standard
    for (int i = 2; i < 8; i++) {
        mvprintw(i, 1, "%s %s", tuningStrings[0][i-2], "|-------------------------------");
    }

    // Refresh the screen
    refresh();    // Main loop to handle user input
    while (1) {
        handleInput(input_win);
    }

    // End ncurses
    endwin();

    return 0;
}

