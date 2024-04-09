






#include <ncurses.h>
#include <string.h>
#include <ctype.h>

// HEADERS 

#define NUM_TUNINGS 8
int xpos=5;
int ypos=1;
int tabNum=1;
int rows, cols;

char *tuningNames[NUM_TUNINGS] =
  {
   "E", 
   "D#",
   "D", 
   "C#", 
   "C",  
   "B", 
   "A#",
   "A"
  };
// Add more tunings as needded, dont forget to also add the name and change NUM_TUNINGS
char *tuningStrings[NUM_TUNINGS][6] = {
    // Standard Tunings
    {"E", "B", "G", "D", "A", "E"}, // E Standard
    {"D#", "A#", "F#", "C#", "G#", "D#"}, // D# Standard 
    {"D", "A", "F", "C", "G", "D"}, // D Standard
    {"C#", "G#", "F#", "C#", "G#", "C#"}, // C# Standard 
    {"C", "G", "E", "B", "F#", "C"}, // C Standard 
    {"B", "F#", "D#", "A#", "F", "B"}, // B Standard
    {"A#", "F", "D", "A", "E", "A#"}, // A# Standard 
    {"A", "E", "C#", "G#", "D#", "A"} // A Standard
};



// MAIN PROGRAM


void movePos()
{
  xpos+=2;
  if(xpos>cols)
  {
    xpos=1;
    ypos+=8;
    tabNum++;
  }
}

void setTuning(char *input){
 
    char tuningSpec[256];
    strcpy(tuningSpec, input + 2);
    int tuningIndex = -1;
    
    for (int i =0; i<NUM_TUNINGS;++i)
    {
      if(strcasecmp(tuningSpec, tuningNames[i]) == 0)
      {
        tuningIndex = i;
        break;
      }
    }
    if(tuningIndex==-1)
    {
      mvprintw(LINES - 2, 0, "Invalid tuning specifier"); 
      refresh();
      return;
    }
   
    for (int i = 2; i < 8; i++) 
    {
      mvprintw(i, 1, " ");
      mvprintw(i, 0, "%s", tuningStrings[tuningIndex][i-2]);
    }
    refresh();
}


void addNote(char input[])
{
   char notes[32];
   strcpy(notes, input+2);
   if(strlen(notes)==1)
   {
     mvprintw(input[0]-'0'+ypos,xpos,"%c", notes[0]);
     movePos();
     refresh();

   }else
   {
     for(int i=0;i<strlen(notes);i++)
     {
      if(notes[i]!=' '){
       
          mvprintw(input[0]-'0'+ypos,xpos,"%c", notes[i]);
        if((i+1)<strlen(notes) && notes[i+1]!= ' ' )
        {
          xpos++;
          mvprintw(input[0]-'0'+ypos,xpos,"%c", notes[i+1]);
          
          i++;
        }

       
        movePos(); 
        refresh();

     

     }
   }
}
}
void handleInput(WINDOW *input_win)
{
    char input[16];
    int input_len = 0;
    mvwprintw(input_win, 0, 0, "input command: ");
    wrefresh(input_win); 
    curs_set(1);

    while (1) 
        {
        int ch = wgetch(input_win); 
        if (ch == '\n') 
        {
            
            wmove(input_win, 0, 14);
            wclrtoeol(input_win);
            wrefresh(input_win);
            break; 
        } else if (ch == 127 || ch == KEY_BACKSPACE) 
        {
            if (input_len > 0) 
            {
                
                mvwaddch(input_win, 0, 14 + input_len - 1, ' ');
                wmove(input_win, 0, 14 + input_len - 1);
                wrefresh(input_win);
                input[--input_len] = '\0'; 
            }
        } else if (input_len < sizeof(input) - 1) 
        { 
            input[input_len++] = ch;
            input[input_len] = '\0'; 

            
            mvwaddch(input_win, 0, 14 + input_len - 1, ch);
            wrefresh(input_win);
        }
    } 
    curs_set(0);
    for (int i = 0; input[i]; i++) 
    {
        input[i] = tolower(input[i]);
    }

    if (strncmp(input, "t ", 2) == 0)
    {
      setTuning(input);
    }else if(isdigit(input[0])&& input[2]< '7')
    {
      addNote(input);
    }else if(strncmp(input, "-",1) == 0)
    {
      movePos();
    }
}

int main() 
{
    
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr,TRUE);
    
    WINDOW *input_win = newwin(1, COLS, LINES - 1, 0);
    scrollok(input_win, FALSE);

    mvprintw(LINES - 2, 0, "(? for help)");
    getmaxyx(stdscr, rows, cols); 
   
    for (int i = 2; i < 8; i++) 
    {
       mvprintw(i,1, "%s %s", tuningStrings[0][i-2],"|");
       for(int j=4;j<cols;j++)
       {
         mvprintw(i,j,"-");
       }
    }

    refresh();   
    while (1) 
    {
        handleInput(input_win);
    }

    endwin();

    return 0;
}

