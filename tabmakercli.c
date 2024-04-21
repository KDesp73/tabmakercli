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
 *  Delete Tabs
 *  Copy/Paste whole Tabs
 */

#include <ncurses.h>
#include <string.h>
#include <ctype.h>


#define NUM_TUNINGS 8  
#define NUM_TIMESIG 25
#define NUM_STRINGS 6
#define NUM_ARGS 10 
#define LEN_ARGS 10
#define COM_GAP 3

int xpos = 7;
int ypos = COM_GAP;
int curTab = 1;
int rows, cols;
int numTabs = 1;


char *tuningNames[NUM_TUNINGS] =
    {"E", "D#", "D", "C#", "C", "B", "A#", "A"};
char *tuningStrings[NUM_TUNINGS][NUM_STRINGS] = {
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
      xpos = 2;      
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
  else if (curTab>1)
  {
    xpos = cols - 1; 
    moveYPos(-1);
    showPos();
  }
}

void setTuning(char tuning[][LEN_ARGS])
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
    mvprintw(LINES - 2, 0, "No tuning found: %s", tuning[1]);
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


void addNote(char notes[][LEN_ARGS])
{ 
  int selectedString = notes[0][0] - '0' - 1;
  for(int i=1; i<NUM_ARGS; i++)
  {
    if(strcmp(notes[i],"")==0)
    {
     break; 
    }
    else
    {                 
      mvprintw(selectedString + ypos,xpos,"%s", notes[i]);
      moveXPos(strlen(notes[i])+1);
    }
  }
}

void addTimeSignature(char* timeSig)
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
        mvprintw(line, column, "/");
        line++;
        i++;
      }
      
      mvprintw(line, column, "%c", timeSig[i]);
     
    }
    moveXPos(3);
  }
}


void addChord(char chords[][LEN_ARGS])
{
 for(int i=1;i<NUM_ARGS;i++)
  {
    if(strcmp(chords[i],"")==0)
    {
      break;
    }
    else 
    {
      for(int j=0; j<NUM_STRINGS; j++)
      {
        mvprintw(j+ypos,xpos,"%c", chords[i][j]);
        
      }
      moveXPos(2);
    }
  }
}
void handleInput(WINDOW *input_win)
{
  char input[NUM_ARGS * LEN_ARGS];
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
    else if (ch == KEY_BACKSPACE)
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
    else if ( ch == KEY_DC)
    {
        moveXPos(-1);
        for (int j = 0; j < NUM_STRINGS; j++)
        {
          mvprintw(j + ypos, xpos, "-");
        }


    }
    else if (input_len < sizeof(input) - 1) 
    {
      curs_set(1);
      input[input_len++] = ch;
      input[input_len] = '\0';

      mvwaddch(input_win, 0, 14 + input_len - 1, ch);
      wrefresh(input_win);
      move(LINES-2, 0); 
      clrtoeol();
      mvprintw(LINES - 2, 0, "(? for help)");
  
   

    }
  }
   if(strlen(input)>1)
  {
      char output[NUM_ARGS][LEN_ARGS];
      int pos=0;
      char *pch;
      pch = strtok(input, " ");

      for (int i = 0; i < NUM_ARGS; i++) 
      {
        for (int j = 0; j < LEN_ARGS; j++) 
        {
            output[i][j] = '\0';         
        }
      }

          
      while (pch != NULL) {
          
            strcpy(output[pos], pch+'\0');
            pos ++;
            pch = strtok(NULL, " ");

        }
     
           

      if (strcmp(output[0], "t") == 0)
      {
        setTuning(output);
      }     
      else if (isdigit(output[0][0]) && output[0][0] - '0' <= NUM_STRINGS && output[0][1]== '\0')
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
      else if (strcmp(input, "|") == 0)
      {
        for (int j = 0; j < NUM_STRINGS; j++)
        {
          mvprintw(j + ypos, xpos, "|");
        }
        moveXPos(2);
      }

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
