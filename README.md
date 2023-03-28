# Battleship

I recreated the classic 2-player game of Battleship, employing knowledge of C++ data structures, Object-Oriented programming, pointer and destructors, and algorithm-based thinking. 

<h2>Gameplay</h2>
<b>Objective:</b> try to sink all enemy ships in a vast ocean (10x10 board) through strategic prediction before your ships are sunk.

<b>Initial Planning Phase:</b>
- Players are given the opportunity to place their fleet
- Players can choose a position and direction of each ship (denoted by different letters on the board)
<b>Attack Phase: </b>
- Players input a coordinate they wish to attack
- Successes are marked by an X, while misses are marked by an O

<h2>Features</h2>
This game features the ability to compete against an Awful Player, Human Player, Mediocre Player, and a Good Player. Provided with basic skeleton code, I designed the Game and Board functionalities, which features a recursive algorithm to place ships, exception handling of invalid inputs, and orderly display of user interface. I also designed player logic for each of the different levels.

The different players adhere to the following logic:

<i>Awful Player: </i>Places ships horizontally aligned to the left; attacks row by row, column by column

<i>Human Player: </i>Places ships and attacks according to human input

<i>Mediocre Player: </i>Block half the board and recursively place ships (backtrack if necessary); attacks vertically and horizontally upon a hit

<i>Good Player: </i>Block half the board and recursively place ships (backtrack if necessary); upon a hit, check all 4 sides systematically, continue attacking in that direction until a miss or invalid square
