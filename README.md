# Aquatris
Aquatris is a Tetris game designed for the Mattel Aquarius computer.
The game comes in an 8KB cartridge format and does not require any memory expansion.
Due to the limited space on the cartridge and computer memory, the game does not include any sound effects.

# Controls
Use "C" to move the tetrimino left

Use "B" to move the tetrimino right

Use "V" to speed up the tetrimino's descent

Use "L" to turn the tetrominos

Use "P" to pause the game

# Compiling the code with Z88dk
To compile the code for Aquatris using Z88dk, follow these steps:

1. Install Z88dk on your computer, if it's not already installed. You can download the latest version of Z88dk from the official website: https://www.z88dk.org/

2. Open a terminal or command prompt and navigate to the directory where your Aquatris source code is located.

3. Run the following command to compile the code:

```bash
zcc +aquarius -clib=ansi -lm -create-app -subtype=rom -Cz--fast -o aquatris src\aquatris.c
```

This command tells Z88dk to compile the Aquatris code for the Aquarius computer, using the ANSI library, the math library, and optimizing for speed. The resulting executable will be created in a file named "aquatris" and formatted as a ROM cartridge.

4. If the compilation is successful, you should see a message indicating that the compilation was completed and the location of the generated file. If there are any errors or warnings, you will need to fix them before you can successfully compile the code.

5. Transfer the generated ROM file to your Aquarius computer or an emulator.

6. Run the game on your Aquarius computer and enjoy!
