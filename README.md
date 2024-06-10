
# Simple Guitar Tab Maker
> [!IMPORTANT]
>!!THIS IS A WIP PROJECT, SOME THINGS STILL DON'T WORK AS INTENDED!!
> For example you cannot import nor you can resize the window or scroll.

![Screenshot_2024-06-10_17-50-07](https://github.com/konLiogka/tabmakercli/assets/78957746/3a1a6366-94b6-49db-82bb-4c099fde9d08)

## Usage:

- `[0-6] [characters]`  
  Add notes on a single string  
  Example: `1 2h3 3p2 1 1 5 7 9 12` (adds all these characters on string 1)  
  To repeat multiple times, type the number of times in front of the string number.  
  Example: `12 3 4` (Result: 3 4 3 4 on string 1)

- `0 [text]`  
  Add comments, bpm, notes, etc., on top of the tab  
  Use the TAB key to delete them.

- `c [chords]`  
  Add multiple chords, either custom or based on a name.  
  Example: `c Am am E 0234xx`  
  To add double-digit frets, use ^  
  Example: `c ^12^10^10`  
  To repeat multiple times, type the number of times in front of c.  
  Example: `c2 Am F` (Result: Am F Am F)

- `t [tuning name]`  
  Change tuning  
  Example: `t d#`

- `s [time signature]`  
  Change time signature  
  Example: `s 4/4`

- `|`  
  Add a bar line

- `x [filepath]`  
  Save file (edit .c file to add custom path)

Use the arrow keys or touchpad to navigate up, down, left, right.  
Press the delete key to delete.

For more settings, use the .c file to add more time signatures, tunings, and change settings.



