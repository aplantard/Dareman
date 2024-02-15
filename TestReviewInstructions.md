# Packaging the test

- Checkout the test_master branch which is the one that should be used to send to candidates. test_master is a degraded version of master (which one day should be expanded to contain a good solution to this test for reference)
- Package the test to be sent to the candidate by running the following command: `git archive --format="zip" --output="dareman.zip" test_master`
- The dareman already packaged is stored in https://drive.google.com/drive/folders/1jfEayuhT1NZ9C7LUW2Sk9S83r1iN-1U2, please update it whenever necessary

# Reviewing the test

- Upload the candidate's test results in the archive: https://drive.google.com/drive/folders/1PuMdQvQprTXl6_-OVZv21gm0ep12pUTh
- Read the candidate's comments in Readme.md or in his e-mail
- Check that the game runs at the desired framerate of 100 FPS
- Check that there are no memory leaks. The sprites allocated with LoadSprite should all be deleted. Check that the sprites are pooled and not loaded for each instance.
- Check that the game plays as expected. See Readme.md for mandatory features. Bonus points for:
	- Player and ghosts can wrap around the level
	- Full ghost AI with chase and scatter, unique ghost personnalities
	- Extra features: fruits, counting points...
	- Can play several games in a row without exiting the program
- Check the A* implementation and that the ghosts' AI uses it
	- Check that A* is only called when necessary, i.e. when the ghosts need to recompute their paths
	- An important bit of detail is that ghosts may only turn when they have a choice, so when they are on a tile with at least 3 possibilities, and ghosts should never be able to turn back
	- Check that the A* only operates on intersection tiles and makes a minimal graph to compute the problem on, rather than all tiles.
	- A* only needs to return one direction, the first one, since it will be called again at every intersection
- Check the actual code
	- Diff the code from the master branch to the test_master branch to see what was refactored, evaluate their relevace
	- Added code should match the existing style
	- Evaluate added code readability, usage of modern idioms, comments
	- Movement code should not be repeated, it should be shared between ghosts and dareman
	- Unifying all the game objects (including non moving ones) is better
	- Extracting things from the main() method
	- Elegant handling of the necessary globals and singletons (window, renderer, level). The original code intentionally has them all different.
	- Bonus point for using vector math instead of the somewhat horrible current logic with switches on Direction

# Parts to add in this test

- Data structures: there should be a non optimal data structure that has a high performance impact but not easily noticeable (probably in the level)
- Data oriented programming: there should be an obvious "column first" loop that has a high performance impact, see if the candidate catches it
- Inconsistent coding style to see if the candidate sees it and fixes it