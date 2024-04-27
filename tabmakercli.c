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



#define NUM_TUNINGS 8   /* Number of tunings, change as you add more. */
#define NUM_TIMESIG 25  /* Number of time signatures, change as you add more. */
#define NUM_STRINGS 6   /* Number of strings, don't change as not supported yet. */
#define NUM_ARGS    10  /* Number of arguments in one input. */
#define LEN_ARGS    20  /* Length of each argument. */
#define COM_GAP     3   /* Gap left for comments, tempo etc. */

int xpos       = 7;        /* initial X position */
int ypos       = COM_GAP;  /* initial Y position */
int curTab     = 1;        /* initial current tab */
int numTabs    = 1;        /* initial number of tabs */
int rows, cols;            /* initialisation of terminal rows and columns */
WINDOW *output_win;

/*char filePath[100] = "/home/kliogka/"; remove comments here and in saveOutput*/

char *tuningNames[NUM_TUNINGS] =
    {"E", "D#", "D", "C#", "C", "B", "A#", "A"};
char *tuningStrings[NUM_TUNINGS][NUM_STRINGS] = {
    {"E", "B", "G", "D", "A", "E"},        
    {"D#", "A#", "F#", "C#", "G#", "D#"}, 
    {"D", "A", "F", "C", "G", "D"},        
    {"C#", "G#", "F#", "C#", "G#", "C#"},  
    {"C", "G", "E", "B", "F#", "C"},      
    {"B", "F#", "D#", "A#", "F", "B"},     
    {"A#", "F", "D", "A", "E", "A#"},    
    {"A", "E", "C#", "G#", "D#", "A"}     
};

char *timeSignatures[NUM_TIMESIG] =
    {
        "4/4", "10/8", "2/4", "6/8", "9/8",
        "12/8", "5/4", "7/8", "3/8", "2/2",
        "5/8", "7/4", "3/2", "4/2", "2/8",
        "6/4", "9/4", "12/16", "7/16", "11/8",
        "5/16", "13/8", "10/8", "15/8", "16/8"
    };

void 
showPos()
{
  curs_set(0);
  wmove(output_win, ypos + NUM_STRINGS, 0);
  wclrtoeol(output_win);
  mvwprintw(output_win, ypos + NUM_STRINGS, xpos, "^");
  wrefresh(output_win);
 }

void 
renderTab()
{
  for (int i = ypos; i < ypos + NUM_STRINGS; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      mvwprintw(output_win, i, j, "-");
    }
  }
  numTabs++;
}

void 
moveYPos(int y)
{

  if (y == -1)
  {
    if (curTab > 1 && xpos >= 6)
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
  wrefresh(output_win);
}

void 
moveXPos(int x)
{
  wmove(output_win, ypos + NUM_STRINGS, 0);
  wclrtoeol(output_win);
  if (xpos + x >= 0)
  {
    xpos += x;
    if (xpos >= cols)
    {
      xpos = 0;
      if (numTabs == curTab)
      {
        moveYPos(1);
        renderTab();
      }
      else if (numTabs > curTab)
      {
        moveYPos(1);
      }
    }
    showPos(output_win);
  }
  else if (curTab > 1)
  {
    xpos = cols - 1;
    moveYPos(-1);
    showPos();
  }
 }

void 
setTuning(char tuning[][LEN_ARGS])
{
  int tuningIndex = -1;
  for (int i = 0; i < NUM_TUNINGS; ++i)
  {
    if (strcasecmp(tuning[1], tuningNames[i]) == 0)
    {
      tuningIndex = i;
      break;
    }
  }
  if (tuningIndex == -1)
  {
    mvwprintw(output_win, LINES - 2, 0, "No tuning found: %s", tuning[1]);
    return;
  }
  for (int i = 0; i < NUM_STRINGS; i++)
  {
    mvwprintw(output_win, COM_GAP + i, 0, " ");
    mvwprintw(output_win, COM_GAP + i, 2, " ");
    mvwprintw(output_win, COM_GAP + i, 1, "%s", tuningStrings[tuningIndex][i]);
  }
 
}

void 
addNote(char notes[][LEN_ARGS])
{
  int selectedString = notes[0][0] - '0' - 1;
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

void 
addTimeSignature(char *timeSig)
{
  bool flag = false;

  for (int i = 0; i < NUM_TIMESIG; i++)
  {
    if (strcmp(timeSig, timeSignatures[i]) == 0)
    {
      flag = true;
      break;
    }
  }
  if (flag)
  {
    int line = ypos + 2;
    int column = xpos;
    for (int i = 0; i < strlen(timeSig); i++)
    {
      if (timeSig[i] == '/')
      {
        line++;
        mvwprintw(output_win, line, column, "/");
        line++;
        i++;
      }
      mvwprintw(output_win, line, column, "%c", timeSig[i]);
    }
    moveXPos(3);
  }
}

void 
addChord(char chords[][LEN_ARGS])
{
  for (int i = 1; i < NUM_ARGS; i++)
  {
    if (strcmp(chords[i], "") == 0)
    {
      break;
    }
    else
    {
      for (int j = 0; j < NUM_STRINGS; j++)
      {
        mvwprintw(output_win, j + ypos, xpos, "%c", chords[i][j]);
      }
      moveXPos(2);
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
    else if (ch == KEY_RIGHT)
    {
      moveXPos(1);
    }
    else if (ch == KEY_LEFT)
    {
      moveXPos(-1);
    }
    else if (ch == KEY_DOWN)
    {
      if (numTabs != curTab)
      {
        moveYPos(1);
      }
    }
    else if (ch == KEY_UP)
    {
      moveYPos(-1);
    }
    else if (ch == KEY_DC)
    {
      moveXPos(-1);
      for (int j = 0; j < NUM_STRINGS; j++)
      {
        mvwprintw(output_win, j + ypos, xpos, "-");
      }
    }
    else if (input_len < sizeof(input) - 1 && arg_len<LEN_ARGS)
    {

      curs_set(1);
      input[input_len++] = ch;
      input[input_len] = '\0';

      mvwaddch(input_win, 0, 14 + input_len - 1, ch);
      wrefresh(input_win);
      wmove(output_win, LINES - 2, 0);
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
      strcpy(output[pos], pch + '\0');
      pos++;
      pch = strtok(NULL, " ");
    }
    if (strcmp(output[0], "t") == 0)
    {
      setTuning(output);
    }
    else if (isdigit(output[0][0]) && output[0][0] - '0' <= NUM_STRINGS && output[0][1] == '\0')
    {
      addNote(output);
    }
    else if (strcmp(output[0], "s") == 0)
    {
      addTimeSignature(output[1]);
    }
    else if (strcmp(output[0], "c") == 0)
    {
      addChord(output);
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
  output_win = newwin(LINES-2,COLS, 0, 0);

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
  showPos(output_win);
  wrefresh(output_win);
  while (1)
  {
    handleInput(input_win);
  }
  endwin();
  return 0;
}
