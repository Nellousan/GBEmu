# GBEmu

## State of The project

This project is done with the SFML lib.

THIS PROJECT IS ON-HOLD\
I started this project before learning proper c++ and i might have to redo a lot of this code.


![Gif](/images/gif1.gif)


At this point, this project can emulate the Gameboy's processor, the LR35902. It cannot run games because GB games needs the Picture Processor Unit (PPU)
to modify registers in the memory to continue execution. As seen in the gif above, the tetris rom is running, but it's looping waiting for the PPU to modify said register,
before continuing.\
\
The emulation is currently ticking too slow to match the actual LR35902 clock speed. This comes from the way i'm preparing the cpu instructions, 
a bunch of string comparison is done and it's slowing down the emulation significantly.\
I was in the process of threadsafing the project before stopping working on it. 
