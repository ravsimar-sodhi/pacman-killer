# pacman-killer-game

A simple 2D game made with openGL.
To run, enter the following commands on the terminal:
```
cmake CMakeLists.txt 
make
./graphics_asg1 
```

About the game
------------------
The game consists of a the player controlling a ball with the following controls:

A - move left
D - move right
Spacebar - Jump

Arrow keys - to pan the screen
Scroll wheel - to zoom in/out of the screen

Initially the the game consists of a player ball, a pond, a trampoline and flying balls traverse the screen from left to right.
Different colored balls have different points (5, 10 15)
Once the player reaches 100 points, flying balls with inclined slopes on them are also added.
The ball will jump along the angle of reflection if it hits the inclined sloped balls.

After 200 points, some spikes will appear at the left edge of the screen and a penalty of 20 points is set if the ball hits the spikes.
After 250 points, a magnet can appear either from the left edge or the right edge of the screen,
causing a horizontal acceleration to the ball, when it is in the air.
