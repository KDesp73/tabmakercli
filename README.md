Simple Guitar Tab Maker

Use .c file to add more time signatures, tunings and change settings

Manual:

  - To add a note, select string number and type in the notes you want. You can actually add whatever character you want, was going to use a char array for allowed characters but decided not to.
    Example: 1 2h3 3p2 1 1 5 7 9 12 (adds all these characters on string 1)
  - To add chord just type in c followed by the chord of your choice. You can add multiple chords in one line.
    Example: c 01220x 133211 (Adds Am and F chord)
  - To change tuning type in t followed by the tuning name (found in .c file)
    Example: t d# (change tuning to D# standard)
  - To add time signatures type in s followed by the time signature. There's a char array with allowed time signatures
    Example: s 4/4 (adds 4/4 time signature)
  - To Make a divider just type in |
  - You can move the indicator left or right and up or down using arrow keys or touchpad.
  - You can delete something by moving the indicator to the position and pressing the delete button.
    
To-Do:
 *  Add support for more strings
 *  Add Tempo, comments, Parts
 *  Export as txt
 *  Delete Tabs
 *  Copy/Paste whole Tabs
 *  Responsive to resizing
 *  Scrolling
 *  Add multiples of the same note/chord using * character
 *  Add chord based on name
