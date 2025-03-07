/*
 _____      _                  _              ___ _ _
/__   \__ _| |__   /\/\   __ _| | _____ _ __ / __\ (_)
  / /\/ _` | '_ \ /    \ / _` | |/ / _ \ '__/ /  | | |
 / / | (_| | |_) / /\/\ \ (_| |   <  __/ | / /___| | |
 \/   \__,_|_.__/\/    \/\__,_|_|\_\___|_| \____/|_|_|
*/

/* Simple program to write tabs */


#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>


#define NUM_TUNINGS 11   /* Number of tunings, change as you add more. */
#define NUM_TIMESIG 25  /* Number of time signatures, change as you add more. */
#define NUM_STRINGS 6   /* Number of strings, don't change as not supported yet. */
#define NUM_CHORDS  9
#define NUM_ARGS    10  /* Number of arguments in one input. */
#define LEN_ARGS    20  /* Length of each argument. */
#define COM_GAP     3   /* Gap left for comments, tempo etc. */

#define KEY_TAB 9
#define LOWER_BOUND 4
#define TAB_HEIGHT 6

int tuningIndex = 0;        /* store the current tuning */
int xpos        = 7;        /* initial X position */
int ypos        = COM_GAP;  /* initial Y position */
int curTab      = 0;        /* initial current tab */
int numTabs     = 1;        /* initial number of tabs */
int rows, cols;             /* initialisation of terminal rows and columns */
WINDOW *output_win;

/*char filePath[100] = "/home/$USER/"; Add custom path, remove comments here and in saveOutput*/

char *tuningNames[NUM_TUNINGS] = {
        "E", 
        "standard", 
        "dropd",
        "dadgad",
        "D#", 
        "D", 
        "C#", 
        "C", 
        "B", 
        "A#", 
        "A"
    };
char *tuningStrings[NUM_TUNINGS][NUM_STRINGS] = {
        {"E",  "B",  "G",  "D",  "A",  "E" },
        {"E",  "B",  "G",  "D",  "A",  "E" },
        {"E",  "B",  "G",  "D",  "A",  "D" },        
        {"D",  "A",  "G",  "D",  "A",  "D" },        
        {"D#", "A#", "F#", "C#", "G#", "D#"}, 
        {"D",  "A",  "F",  "C",  "G",  "D" },        
        {"C#", "G#", "F#", "C#", "G#", "C#"},  
        {"C",  "G",  "E",  "B",  "F#", "C" },      
        {"B",  "F#", "D#", "A#", "F",  "B" },     
        {"A#", "F",  "D",  "A",  "E",  "A#"},    
        {"A",  "E",  "C#", "G#", "D#", "A" }     
    };

char *chordNames[NUM_CHORDS] =
    {"A", "Am", "C", "D", "Dm", "E", "Em", "F", "G" };

char *chords[NUM_CHORDS] =
    {"02220x", "012200", "01023x", "2320xx", "1320xx", "001220", "000220", "112311",   "330023" };

char *timeSignatures[NUM_TIMESIG] = {
        "4/4", "10/8", "2/4", "6/8", "9/8",
        "12/8", "5/4", "7/8", "3/8", "2/2",
        "5/8", "7/4", "3/2", "4/2", "2/8",
        "6/4", "9/4", "12/16", "7/16", "11/8",
        "5/16", "13/8", "10/8", "15/8", "16/8",
    };

void 
showPos(void)
{

    curs_set(0);
    wmove(output_win, ypos + NUM_STRINGS, 0);
    wclrtoeol(output_win);
    mvwprintw(output_win, ypos + NUM_STRINGS, xpos, "^");
    for(int row=-1;row<6;row++)
    {
        mvwchgat(output_win,ypos+row,xpos,1,A_REVERSE,0,NULL);

    }
    wrefresh(output_win);
    for(int row=-1;row<6;row++)
    {
        mvwchgat(output_win,ypos+row,xpos,1,A_NORMAL,0,NULL);

    }
}

void 
renderTab()
{
    for (int i = ypos; i < ypos + NUM_STRINGS; i++)
    {
        mvwprintw(output_win, i, 0, " %-2s|", tuningStrings[tuningIndex][i-ypos]);
        for (int j = 4; j < cols; j++)
        {
            mvwprintw(output_win, i, j, "-");
        }
    }

    numTabs++;
    xpos = LOWER_BOUND;
}

void
moveYPos(int y)
{
    if (y == -1)
    {
        if (curTab > 0)
        {
            wmove(output_win, ypos + NUM_STRINGS, 0);
            wclrtoeol(output_win);
            ypos -= COM_GAP + NUM_STRINGS;
            curTab--;
            showPos();
        }
    }
    else if (y == 1)
    {
        wmove(output_win, ypos + NUM_STRINGS, 0);
        wclrtoeol(output_win);
        ypos += COM_GAP + NUM_STRINGS;
        curTab++;
        showPos();
    }
}

void 
moveXPos(int x)
{
    if (xpos + x >= 0)
    {
        xpos += x;
        if (xpos >= cols)
        {
            xpos = 0;
            if (numTabs - 1 == curTab)
            {
                moveYPos(1);
                renderTab();
                xpos = LOWER_BOUND; // Moving the cursor inside the new tab
            }
            else if (numTabs > curTab)
            {
                moveYPos(1);
            }
        }
    }
    else if (curTab > 0)
    {
        xpos = cols - 1;
        moveYPos(-1);
    }
    showPos();
}

void 
addTimeSignature(char *timeSig)
{
    for (int i = 0; i < NUM_TIMESIG; i++)
    {
        if (strcmp(timeSig, timeSignatures[i]) == 0)
        {
            int line = ypos + 2;
            int column = xpos;
            for (int j = 0; j < strlen(timeSignatures[i]); j++)
            {
                if (timeSignatures[i][j] == '/')
                {
                    column=xpos;
                    line++;
                    mvwprintw(output_win, line, column, "/");
                    line++;
                }
                else
                {
                    mvwprintw(output_win, line, column, "%c", timeSignatures[i][j]);
                    column++;
                }
            }
            moveXPos(3);
            break;
        }
    }
}

void
setTuning(char tuning[][LEN_ARGS])
{
    int index = -1;
    for (int i = 0; i < NUM_TUNINGS; ++i)
    {
        if (strcasecmp(tuning[1], tuningNames[i]) == 0)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        mvwprintw(output_win, LINES - 2, 0, "No tuning found: %s", tuning[1]);
        return;
    }
    
    tuningIndex = index;

    for (size_t tab = 0; tab < numTabs; tab++)
    {
        int yOffset = tab * (TAB_HEIGHT + COM_GAP) + COM_GAP;

        for (int i = 0; i < NUM_STRINGS; i++)
        {
            mvwprintw(output_win, 
                    yOffset + i, 0, 
                    " %-2s|", tuningStrings[tuningIndex][i]);
        }
    }
    
    wrefresh(output_win);
}

void 
addNote(char notes[][LEN_ARGS], int numTimes)
{
    int selectedString = notes[0][0] - '0' - 1;
    for(int multiples=1; multiples<=numTimes; multiples++)
    {
        for (int i = 1; i < NUM_ARGS; i++)
        {
            if (strcmp(notes[i], "") == 0)
            {
                break;
            }
            else
            {
                for(int j=0; j<LEN_ARGS; j++)
                {
                    if(notes[i][j]=='\0')
                    {
                        break;
                    }
                    mvwprintw(output_win, selectedString + ypos, xpos, "%c", notes[i][j]);
                    moveXPos(1);
                }
                moveXPos(1);
            }
        }

    }
}


void 
addChord(char chord[][LEN_ARGS], int numTimes)
{
    int line;
    for(int multiples=1; multiples<=numTimes; multiples++)
    {
        for(int i=1; i<NUM_ARGS;i++)
        {
            line=0;
            if (strcmp(chord[i], "") == 0)
            {
                break;
            }
            for(int j=0;j<NUM_CHORDS;j++)
            {
                if (strcasecmp(chord[i], chordNames[j]) == 0 && line<=NUM_STRINGS-1)
                {
                    for(int k=0;k<strlen(chords[j]);k++)
                    {
                        mvwprintw(output_win, ypos+line, xpos, "%c", chords[j][k]);
                        line++;
                    }  
                    moveXPos(2);
                    break; 
                }  
            }
            if(line==0)
            {
                for(int j=0; j<NUM_ARGS; j++)
                {
                    if(line<=NUM_STRINGS-1)
                    {
                        if(chord[i][j]=='^' && chord[i][j+1]!='\0' )
                        {
                            mvwprintw(output_win, ypos+line, xpos, "%c", chord[i][j+1]);

                            if(chord[i][j+2]!='\0')
                            {
                                mvwprintw(output_win, ypos+line, xpos+1, "%c", chord[i][j+2]);
                                j++;
                            }
                            j++;
                        }
                        else
                        {
                            mvwprintw(output_win, ypos+line, xpos, "%c", chord[i][j]);
                        }
                        line++;
                    }
                }
                moveXPos(2); 
            }
        }
    }
}

void 
saveOutput(char *filename)
{
    mvwprintw(output_win, ypos + NUM_STRINGS, xpos, " "); 
    //strcat(filePath, filename);
    //strcpy(filename, filePath);
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            int item = mvwinch(output_win, y, x);
            if(item == ERR)
            {
                fputc(' ', file);
            }
            else 
            {
                fputc(item & A_CHARTEXT, file);
            }
        }
        fputc('\n', file);
    }
    fclose(file);
    mvwprintw(output_win, ypos + NUM_STRINGS, xpos, "^");
}

int
ismultiple(const char *output)
{
    for(int i=1; output[i]!='\0';i++)
    {
        if(output[i]=='\0')
        {
            return 0;
        }
        if(!isdigit(output[i]))
        { 
            return 0;
        }
    }
    return 1;
}

void 
handleInput(WINDOW *input_win)
{
    char input[(NUM_ARGS*LEN_ARGS*2)-1];
    int input_len = 0;
    int arg_len = 0;
    mvwprintw(input_win, 0, 0, "input command: ");
    wrefresh(input_win);
    while (1)
    {

        int ch = wgetch(input_win);

        if( ch == ' ')
        {
            arg_len = 0;
        }
        if (ch == '\n')
        {
            wmove(input_win, 0, 14);
            wclrtoeol(input_win);
            wrefresh(input_win);
            arg_len=0;
            break;
        }
        else if (ch == KEY_BACKSPACE)
        {
            if (input_len > 0)
            {
                mvwaddch(input_win, 0, 14 + input_len - 1, ' ');
                wmove(input_win, 0, 14 + input_len - 1);
                wrefresh(input_win);
                input[--input_len] = '\0';
                arg_len--;
            }
        }
        else if (ch == KEY_RESIZE)
        {
            getmaxyx(stdscr, rows, cols);
        }
        else if (ch == KEY_RIGHT)
        {
            if (xpos < cols - 1 || numTabs-1 != curTab)
            {
                moveXPos(1);
            }
        }
        else if (ch == KEY_LEFT)
        {
            if(xpos <= LOWER_BOUND) return;
            moveXPos(-1);
        }
        else if (ch == KEY_DOWN)
        {
            if (numTabs-1 != curTab)
            {
                moveYPos(1);
            }
        }
        // else if (ch == KEY_SDC) // Shift+Del
        else if (ch == KEY_TAB)
        {
            if(xpos < LOWER_BOUND) return;
            mvwprintw(output_win, ypos-1, xpos, " ");
            moveXPos(-1);
        }
        else if (ch == KEY_UP)
        {
            moveYPos(-1);
        }
        else if (ch == KEY_DC)
        {
            if(xpos < LOWER_BOUND) return;
            for (int j = 0; j < NUM_STRINGS+1; j++)
            {
                mvwprintw(output_win, j + ypos, xpos, "-");
            }
            moveXPos(-1);

        }
        else if (input_len < sizeof(input) - 1 && arg_len<LEN_ARGS)
        {
            curs_set(1);
            input[input_len++] = ch;
            input[input_len] = '\0';

            mvwaddch(input_win, 0, 14 + input_len - 1, ch);
            wrefresh(input_win);
            wmove(output_win, LINES - COM_GAP - 1, 0);
            wclrtoeol(output_win);
            arg_len++;
        }
    }
    if (strlen(input) > 1)
    {
        char output[NUM_ARGS][LEN_ARGS];
        int pos = 0;
        char *pch;
        pch = strtok(input, " ");
        for (int i = 0; i < NUM_ARGS; i++)
        {
            for (int j = 0; j < LEN_ARGS; j++)
            {
                output[i][j] = '\0';
            }
        }
        while (pch != NULL)
        {
            strcpy(output[pos], &pch ['\0']); // Appends null-terminator
            pos++;
            pch = strtok(NULL, " ");
        }
        if (strcmp(output[0], "t") == 0)
        {
            setTuning(output);
        }
        else if (isdigit(output[0][0]) && 
                output[0][0] - '0' <= NUM_STRINGS &&
                output[0][0] - '0' >= 0)
        {
            if(strlen(output[0])==1)
            {
                addNote(output,1);
            }
            else 
            {
                if(ismultiple(output[0]))
                {
                    addNote(output,atoi(output[0]+1));
                }
            }
        }
        else if (strcmp(output[0], "s") == 0)
        {
            addTimeSignature(output[1]); 
        }
        else if (output[0][0] == 'c')
        {
            if(strlen(output[0])==1)
            {
                addChord(output,1);
            }
            else 
            {
                if(ismultiple(output[0]))
                {
                    addChord(output,atoi(output[0]+1));
                }
            }   
        }
        else if (strcmp(input, "x") == 0)
        {
            saveOutput(output[1]);
        }
    }else {
        if (strcmp(input, "|") == 0)
        {
            for (int j = 0; j < NUM_STRINGS; j++)
            {
                mvwprintw(output_win, j + ypos, xpos, "|");
            }
            moveXPos(2);
        }
    }
    wrefresh(output_win);
    showPos(); // Repaint bar after executing command
}

int 
main()
{
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    curs_set(0);
    WINDOW *input_win = newwin(1, COLS, LINES - 1, 0);
    output_win = newwin(LINES-COM_GAP,COLS, 0, 0);

    scrollok(input_win, FALSE);
    scrollok(output_win, TRUE);

    keypad(input_win, TRUE);
    getmaxyx(stdscr, rows, cols);
    for (int i = ypos; i < COM_GAP + NUM_STRINGS; i++)
    {
        mvwprintw(output_win, i, 1, "%s %s", tuningStrings[0][i - ypos], "|");
        mvwprintw(output_win, i, 0, " ");
        for (int j = 4; j < cols; j++)
        {
            mvwprintw(output_win, i, j, "-");
        }
    }

    wrefresh(output_win);
    showPos();
    while (1)
    {
        handleInput(input_win);
    }
    endwin();
    return 0;
}
