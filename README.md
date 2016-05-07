# artificial-intelligence-game

##Line of Actions
This is the source code for my implementation of the Lines of Action game.  Use Visual Studios to build and run it.

###Game rules
https://en.wikipedia.org/wiki/Lines_of_Action

###Game AI
I've written the AI using the alpha-beta pruning algorithm.  I have also implemented my own heuristic function that gets called when the game tree is too deep.  This heuristic function takes into account of the number of trees there are in the BFS forest and the number of pieces there are in total for each player.

###Game interface
The game is entirely interacted with in the command line.  In the beginning, the game prompts the player to choose the board size (5x5 or 6x6), and the color to play as (black or white).  Throughout the game, the player will be asked to input their action.  An action is defined by a row, column, and direction.  An example of an action would be: 0 3 d.  This action means to move the piece at row index 0 and column index 3 in the down direction.  
