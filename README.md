# Console_snake_game
Snake game made with `ncurses` library for university project. We were tasked to edit only `snake.cpp` and `snake.h` files.
For test purposes, the game uses only one of 3 windows to display the game. They can be switched using *tab button*. 
## Game mechanics
### Controls
You can start and restart the game by clicking *r button*. This generates the snake, which can be moved using *arrows*. 
The player controls the head of snake and the rest of the body follows its trail. 
*Switching windows*, clicking *p button* or *h button* (which also displays information shown in the screenshot below) pauses the game.
### Aim of the game
The aim of the game is to gain as much food (it generates one on the map randomly) as possible which increases the game's level, speed of snake's movement and its lenght.
The snake can't hit map's edges as it will appear at the another end.
### Lose conditions
Player loses when snake collides with itself. The game then shows your result - how much food have you gathered.

<p align="center"> <img src="https://user-images.githubusercontent.com/97179185/211585183-68c801a6-f89c-4cd6-86c2-e514655c33bc.png">
</p>
<p align="center">
<img src="https://user-images.githubusercontent.com/97179185/211587862-3129764a-e2aa-43f9-b928-d78da4cf70e5.png">
</p>



