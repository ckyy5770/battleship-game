# battleship-game

implementation of classic [Battleship Game](https://en.wikipedia.org/wiki/Battleship_(game))

## Design

### Overview

#### Server

The server is able to host multiple games at one time.

In the game, the server only expect two commands from the client:

1. place the ship (location, direction)

2. attack the location

The server will in turn gives two kind of responses:

1. placement response (succeed or not)

2. attack response (hit or not)

#### Client

in the game, the client should only send two commands:

1. place the ship (location, direction)

2. attack the location

### Classes

#### Board

Class board is a game board for one player. The board should keep a record of:

1. ships on the board and all their information (locations, damaged parts)

2. locations where the opponent attacked

for one location (x, y) on the 10 * 10 board, it only has two categories of states:

1. occupied by a ship or not

2. has been attacked or not

we then use two bit flags to indicate each of these two states of the location:

1. OCCUPIED := 1 << 0;

2. ATTACKED := 1 << 1;

we also need a way to refer to corresponding ship when a location is being attack, so we store the ship pointers into a 10 * 10 array and the array index indicates the location.

#### Ship

Class ship includes all information about a ship on board:

1. size

2. remaining life

3. head location

4. direction (vertical or horizontal)
