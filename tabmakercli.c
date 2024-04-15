/*
 _____      _                  _              ___ _ _
/__   \__ _| |__   /\/\   __ _| | _____ _ __ / __\ (_)
  / /\/ _` | '_ \ /    \ / _` | |/ / _ \ '__/ /  | | |
 / / | (_| | |_) / /\/\ \ (_| |   <  __/ | / /___| | |
 \/   \__,_|_.__/\/    \/\__,_|_|\_\___|_| \____/|_|_|
*/

/*
 * To-Do:
 *  Add support for more strings
 *  Add Tempo, comments, Parts
 *  Export as txt
 *  Add chords
 *  Delete chords/notes
 *  Delete Tabs
 *  Copy/Paste whole Tabs
 */

#include <ncurses.h>
#include <string.h>
#include <ctype.h>


// HEADERS

#define NUM_TUNINGS 8
#define NUM_TIMESIG 25
#define NUM_STRINGS 6
#define COM_GAP 3
int xpos = 7;
int ypos = COM_GAP;
int curTab = 1;
int rows, cols;
int numTabs = 1;

char *tuningNames[NUM_TUNINGS] =
    {
        "E",
        "D#",
        "D",
        "C#",
        "C",
        "B",
        "A#",
        "A"};
// Add more tunings as needded, dont forget to also add the name and change NUM_TUNINGS
char *tuningStrings[NUM_TUNINGS][NUM_STRINGS] = {
    // Standard Tunings
    {"E", "B", "G", "D", "A", "E"},       // E Standard
    {"D#", "A#", "F#", "C#", "G#", "D#"}, // D# Standard
    {"D", "A", "F", "C", "G", "D"},       // D Standard
    {"C#", "G#", "F#", "C#", "G#", "C#"}, // C# Standard
    {"C", "G", "E", "B", "F#", "C"},      // C Standard
    {"B", "F#", "D#", "A#", "F", "B"},    // B Standard
    {"A#", "F", "D", "A", "E", "A#"},     // A# Standard
    {"A", "E", "C#", "G#", "D#", "A"}     // A Standard
};

char *timeSignatures[NUM_TIMESIG] =
    {
        "4/4", "10/8", "2/4", "6/8", "9/8",
        "12/8", "5/4", "7/8", "3/8", "2/2",
        "5/8", "7/4", "3/2", "4/2", "2/8",
        "6/4", "9/4", "12/16", "7/16", "11/8",
        "5/16", "13/8", "10/8", "15/8", "16/8"};

char *allowedChars = " 1234567890bx~";

char *allowedAno = "ph/'\\'";

char *commands = "ts|";

// MAIN PROGRAM

void showPos()
{
  curs_set(0);
  move(ypos + NUM_STRINGS, 0);
  clrtoeol();
  mvprintw(ypos + NUM_STRINGS, xpos, "^");
  refresh();
}

void renderTab()
{
  for (int i = ypos; i < ypos + NUM_STRINGS; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      mvprintw(i, j, "-");
    }
  }
  numTabs++;
}

void moveYPos(int y)
{

  if (y == -1)
  {
    if (curTab>1 && xpos>=6)
    {
      move(ypos + NUM_STRINGS, 0);
      clrtoeol();
      ypos -= COM_GAP + NUM_STRINGS;
      curTab--;
      showPos();
    }
  }
  else if (y == 1 )
  {
      move(ypos + NUM_STRINGS, 0);
      clrtoeol();
      ypos += COM_GAP + NUM_STRINGS;
      curTab++;
     
      showPos();
  }
}

void moveXPos(int x)
{
  move(ypos + NUM_STRINGS, 0);
  clrtoeol();
  if (xpos+x > 4 || (xpos+x>0 && curTab>1))
  {
    xpos += x;
    if (xpos >= cols)
    {
      xpos = 4;
      
      
      if(numTabs==curTab)
      {
        moveYPos(1);
        renderTab();

      }
      else if(numTabs>curTab)
      {
        moveYPos(1);
      }
    }
    showPos();
  }
  else if (ypos > COM_GAP)
  {
    xpos = cols - 1;
    moveYPos(-1);
    showPos();
  }
}

void setTuning(char *input)
{

  char tuningSpec[16];
  strcpy(tuningSpec, input + 2);
  int tuningIndex = -1;

  for (int i = 0; i < NUM_TUNINGS; ++i)
  {
    if (strcasecmp(tuningSpec, tuningNames[i]) == 0)
    {
      tuningIndex = i;
      break;
    }
  }
  if (tuningIndex == -1)
  {
    mvprintw(LINES - 2, 0, "Invalid tuning");
    refresh();
    return;
  }

  for (int i = 0; i < NUM_STRINGS; i++)
  {
    mvprintw(COM_GAP + i, 2, " ");
    mvprintw(COM_GAP + i, 1, "%s", tuningStrings[tuningIndex][i]);
  }
  refresh();
}

void addNote(char input[])
{
  char notes[32];
  strcpy(notes, input + 2);
  int selectedString = input[0] - '0' - 1;
  for (int i = 0; i < strlen(notes); i++)
  {
    if (notes[i] != ' ' && strchr(allowedAno, notes[i]) == NULL)
    {

      if ((i + 1) < strlen(notes) && notes[i + 1] != ' ' && strchr(allowedChars, notes[i + 1]) != NULL && strchr(allowedChars, notes[i]) != NULL)
      {
        moveXPos(1);
        mvprintw(selectedString + ypos, xpos - 1, "%c", notes[i]);
        mvprintw(selectedString + ypos, xpos, "%c", notes[i + 1]);
        i++;
        moveXPos(2);
      }
      else if (strchr(allowedAno, notes[i + 1]) != NULL && (i + 2) < strlen(notes) && notes[i + 2] != ' ')
      {
        moveXPos(2);
        mvprintw(selectedString + ypos, xpos - 2, "%c", notes[i]);
        mvprintw(selectedString + ypos, xpos - 1, "%c", notes[i + 1]);
        mvprintw(selectedString + ypos, xpos, "%c", notes[i + 2]);

        i += 2;
        moveXPos(2);
      }
      else if ((strchr(allowedChars, notes[i]) != NULL))
      {
        mvprintw(selectedString + ypos, xpos, "%c", notes[i]);
        moveXPos(2);
      }
    }
  }
}

void addTimeSignature(char input[])
{
  bool flag = false;
  for (int j = 0; j < NUM_TIMESIG; j++)
  {
    if (strcmp(input + 2, timeSignatures[j]) == 0)
    {
      flag = true;
      break;
    }
  }
  if (flag)
  {
    int line = ypos + 2;
    int column = xpos;

    for (int j = 2; j < strlen(input); j++)
    {
      if (input[j] == '/')
      {
        column = xpos;
        line++;
        mvprintw(line, column, "/");
        line++;
        j++;
      }
      mvprintw(line, column, "%c", input[j]);
      column++;
    }
    moveXPos(3);
  }
}

void handleInput(WINDOW *input_win)
{
  char input[32];
  int input_len = 0;
  mvwprintw(input_win, 0, 0, "input command: ");
  wrefresh(input_win);

  while (1)
  {

    int ch = wgetch(input_win);
    if (ch == '\n')
    {

      wmove(input_win, 0, 14);
      wclrtoeol(input_win);
      wrefresh(input_win);
      break;
    }
    else if (ch == 127 || ch == KEY_BACKSPACE)
    {
      if (input_len > 0)
      {
        mvwaddch(input_win, 0, 14 + input_len - 1, ' ');
        wmove(input_win, 0, 14 + input_len - 1);
        wrefresh(input_win);
        input[--input_len] = '\0';
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
      if(numTabs!=curTab)
      {
        moveYPos(1);
      }
     
    }
    else if (ch == KEY_UP)
    {
      moveYPos(-1);
    }
    else if (input_len < sizeof(input) - 1)
    {
      curs_set(1);
      input[input_len++] = ch;
      input[input_len] = '\0';

      mvwaddch(input_win, 0, 14 + input_len - 1, ch);
      wrefresh(input_win);
    }
  }

  for (int i = 0; input[i]; i++)
  {
    input[i] = tolower(input[i]);
  }

  if (strncmp(input, "t ", 2) == 0)
  {
    setTuning(input);
  }
  else if (isdigit(input[0]) && input[0] < '7')
  {
    addNote(input);
  }
  else if (strncmp(input, "s ", 2) == 0)
  {
    addTimeSignature(input);
  }
  else if (strcmp(input, "|") == 0)
  {
    for (int j = 0; j < 6; j++)
    {
      mvprintw(j + ypos, xpos, "|");
    }
    moveXPos(2);
  }
  refresh();
}

int main()
{
  initscr();
  cbreak();
  noecho();
  scrollok(stdscr, TRUE);
  curs_set(0);

  WINDOW *input_win = newwin(1, COLS, LINES - 1, 0);
  scrollok(input_win, FALSE);
  keypad(input_win, TRUE);

  mvprintw(LINES - 2, 0, "(? for help)");
  getmaxyx(stdscr, rows, cols);
  for (int i = ypos; i < COM_GAP + NUM_STRINGS; i++)
  {
    mvprintw(i, 1, "%s %s", tuningStrings[0][i - ypos], "|");
    mvprintw(i, 0, " ");
    for (int j = 4; j < cols; j++)
    {
      mvprintw(i, j, "-");
    }
  }
  showPos();
  refresh();
  while (1)
  {

    handleInput(input_win);
  }

endwin();
  return 0;
}
