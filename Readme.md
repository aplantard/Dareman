# Dareman

Dareman is a poor attempt at replicating a well-known arcade game. Only you can fix it and make it awesome!


# How to build
Dareman uses CMake and SDL in order to support as many platforms, compilers and IDEs as possible.
Scripts and libraries are provided to build on Windows with Visual Studio but it should take little effort to build it on your favorite operating system.

## Windows / Visual Studio
- Install Visual Studio latest version.
- Install CMake (https://cmake.org/download/) and add cmake to your PATH
- Run GenerateVisualStudioProject.bat
- The solution will be generated in the Build folder
- Build the project
- Binaries will be generated in the Binaries folder
- If you need to add more source files, run GenerateVisualStudioProject.bat again

## linux or macOS
- Install the required packages: cmake, sdl, sdl-image, sdl-ttf
- Generate and build properly, a bat file is provided to generate a simple makefile.
- If you have to modify the CMakeLists.txt, please send us back the modified file to help the next candidates!

## Other OS/IDE combination
- Edit the CMakeLists.txt and/or the generation scripts.
- Please contribute your improvements back to us, so we can support this for the next candidates.

# Test instructions

- Optimize the game and maximize the FPS. There are some very obvious bottlenecks and some more subtle ones
- Fix all the memory leaks
- Implement the game logic parts that have been left out:
	- The player must be able to complete the game with the appropriate conditions
	- Implement and use the A* pathfinding algorithm in Level::ComputeShortetPath
	- Features required to be fully functional: player controls, pickups, ghost chase, flee and respawn logic
	- Visual polish: dareman must be animated, ghost statuses must be displayed
	- Optional: Fully implement the game logic of the original arcade game. Some more information can be found online, for example: http://gameinternals.com/post/2072558330/understanding-pac-man-ghost-behavior
- Refactoring the code, moving parts around and adding new files is encouraged. We left some bad patterns in there intentionally, if something looks wrong to you, show us how you would have made it better
- Document your changes and refactors in this file. Suggestions are encouraged even if they were not implemented.
- Your code must be easily readable and commented appropriately
- Usage of modern C++ and best practices is encouraged

- Please read below in order to properly send us the results back. 95% of candidates don't read below this line, don't be one of them.

# Test results

- First update that I've done is role base. I wanted only one singleton which is the gameEngine and it will have all member necessary to run the game.
- I wanted to have role specified component so I move LoadSprite and CreateSprite functions from Renderer to SpriteManager. I'm still not very happy with the big dependecy between renderer and SpriteManager due to SDL_Renderer. SpriteManager and Sprite class work together. Sprite represente a static sprite it only holds the texture and its size. SpriteManager is in charge of loading and creating all static sprite.
- I added a GameStateMgr to Manage appState.
- I added the GameEngine which will be the singleton that can act as gameEngine. It holds all game elements, the level and all managers. It also provide method to manage game (restart game etc ...)
- I added the corridor teleportation gameplay
- I added a spritesheet class to manage spritesheet. It is in charge of loading spritesheet, selecting sprites in it and rendering those sprite.
- I used spritesheet for dareman and ghosts animations. Animation toggle is based on actor speed instead of actual movement from tile to tile.
- I ordered members in class to minimize structure size.
- I added a collision manager which is responsible for checking collision between ghost and dareman.
- Energizer timing is manage directly in game Engine (It would be better to have its own class).
- Level class is in charge of loading and manage the grid. It owes all method linked to pathfinding in the grid and diretions.
- The class PlayerInputMgr manage all player input. It act as a player controller is mordern engine.
- The renderer class manage all renderering. It create the sdl window, it holds the sdl_renderer and it store fonts. It holds all method that will draw something on the screen and its update method is in charge of calling render method of every class that need to be rendered.
- I created two UI classes which store datas and render UIs. The GameUI class reprensent the HUD at the top of the screen. The other UI class is the modal class that represent all modal that I will present.
- About the game object, I looked at how it was built. As pacdots and energizer were stored directly in tiles, I saw that there was not any gameobject which are not moving. So I decided to store in GameActors class, all the moving behavior. It also store the speed and position of the actor. Dareman and ghost inherit from gameactor.
- I was thinking of adding a gameobject class which gameactors will inherit from it. GameObject would be static object in the scene and I would convert pacdots and energizer to class that will inherit from from gameObject. It would allow to move energizer behaviour out of gameengine class into energizer class.
- Dareman store it's specifications as CanMove method redefinition (dareman cannot go inside ghost house). It manage eatPickup behavior and sprite update as it has its own spritesheet.
- Ghost manage all ghost AI that I will detail below.
- As in the pacman game, ghost have 4 states : Scatter, Chasing, Fleeing and recovering.
- Each ghost leave the ghost house at a specific event. When they leave ghosthouse, they are in scatter state by default.
- In scatter state, each ghost has a target tile in each corner of the maze. first two scatter state is 7 seconds, the next two are 5 seconds and then there is no more scatter switch.
- Ghost switch from chasing to scatter if needed every 20 seconds.
- In chasing mode, all ghosts use the same pathfinding algorithm which is A*, but the target tile is different for each ghost as described here : http://gameinternals.com/post/2072558330/understanding-pac-man-ghost-behavior
- In fleeing mode, the ghost calculate on each intersection the direction which is the farest from dareman. ghosts speed is devided by two in fleeing mode
- In recovering mode, ghosts speed is multiplied by 1.5. Target tile in recovering mode is in the ghost house. When ghosts arrive in ghost house they revive and get back where they stopped in scatter/chasing loop.
- the A* algorithme is stored in Level class as it needs tiles. It takes source and destination tile and calculate path. Ghost call this method with they own target tile to move in the level. The A* algorithme give the full path even if we recalculate on each tiles with multiple direction because it is used only once whe we are in scatter or recovering mode. 
- Assuming you are using git to track your changes, package the test using the following command before sending it back: `git archive --format="zip" --output="dareman_yourname.zip" HEAD`
