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
- I wanted to have role specified component so I move LoadSprite and CreateSprite functions from Renderer to SpriteManager. I'm still not very happy with the big dependecy between renderer and SpriteManager due to SDL_Renderer.
- I added a GameStateMgr to Manage appState.
- I added the GameEngine which will be the singleton that can act as gameEngine.
- I added the corridor teleportation gameplay.
- I used spritesheet for dareman animation. And I based animation on dareman speed not on actual level.
- I ordered members in class to minimize structure size
- Assuming you are using git to track your changes, package the test using the following command before sending it back: `git archive --format="zip" --output="dareman_yourname.zip" HEAD`
