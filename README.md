# Breakout - Enviro
Final Project for EEP 520 W 2020



## Descirption
This repo contains my final project, a repurposing of [Breakout](https://youtu.be/Up-a5x3coC0) with the Enviro engine. 


### Enviro

Enviro is a *multi-agent,multi-user,multi-everything* simulator, created by Eric Klavins. 

Here is a link to the original repo : [Enviro](https://github.com/klavinslab/enviro)


## Installing the game

Here are the steps to install and play the game:

1. Clone repository into a folder on your local drive
2. Open up a terminal window and move into `/breakout_enviro/` folder
3. Run the docker image : `docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:v1.6 bash`
4. Type and run the command in your terminal window : `esm start`
5. Open up a browser and go to your [localhost address](http://localhost/)
6. Go back to your terminal window run `enviro`
7. The game should now start in your browser. Use the left and right arrows to move the paddle.
7. Have fun?


## Technical Background
The game has 4 major objects:

1. Ball
2. Player
3. Block
4. Walls

These comprise the functionality of the game. A larger description below. 



### The Ball
The ball was the most difficult mechanic to have working as desired. Since the ball needs to bounce around off of walls - creating that behavior was not straight forward. Detecting collisions was ok but calculating the collision angle and have the ball move instantaneously in the correct direction. Even if the angle was calculated, the ball still has lag time to hit, stop, turn and then move in the correct direction after collision. 

A fix for instantaneous turning is to teleport the ball into its current position with the new angle. This way we can completely ignore turning mechanics and have the ball turn instantaneously. 

Calculating the collision angle uses some geometry and some senors. Using 3 sensors, one in the center and one at 30 degrees on the left and right side of the ball, we can determine the angle of the wall in respect to the ball at the time of collision. Think of it as solving for a SAS triangle, using the reflection surface as a third side, then calculating angles. 

It also emits events when the ball collides with a block or "lava"


### The Player
The player is just a flat block that acts as a paddle. It provides no functions other than moving from user input and providing the ball as a surface to bounce off of. It is pretty straightforward. It also has a label that displays the current score to the user. It also listens for events emited by the ball when the ball hits a block or the "lava", to keep score. This is done since the player object displays the score, not the ball. 


### Blocks
Blocks are just agents that are placed on initialization that have collision detection and are removed from the game when they collide with the ball. The deletion is done from the ball object, to make the blocks more simple. When placed via the `config.json file`, the blocks are given a massive mass and friction value so it doesn't move when hit by the ball. 


### Blocks
Blocks are just agents that are placed on initialization that have collision detection and are removed from the game when they collide with the ball. The deletion is done from the ball object, to make the blocks more simple. When placed via the `config.json file`, the blocks are given a massive mass and friction value so it doesn't move when hit by the ball. 


## Known Technical Issues
One of the main issues is that there is no current way to reset the game or end the game. Once you destroy all the blocks, the game will keep running until you restart the engine. I wasn't sure how to pause the game or handle replacing the block agents that were destroyed back into the game without substantially hardcoding it in. 

Another issue is sometimes the ball can get stuck along a wall. It never detects a collision and if it does, it will bounce in the opposite direction, still skimming a wall without bouncing away from it, forever. A current way to stop the behavior is to reset the ball, so let it die. Sorry!




