# battleship-game

implementation of classic [Battleship Game](https://en.wikipedia.org/wiki/Battleship_(game))

## Design

### Overview

#### Server & Client

The server is able to host multiple games at one time.

Before game, the server expect client to send a JOIN_GAME request with a unique key to join the game.

In the game, the server only expect two commands from the client:

1. place the ship (location, direction)

2. attack the location

The server will in turn gives two kind of responses:

1. placement response (succeed or not)

2. attack response (hit or not; if hit, is there any ship down?)

##### Message Format

All messages have following format:

`MESSAGE_TYPE (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | PAYLOAD`

Following are specifics of all type of messages:

`REQUEST_JOIN_GAME (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)`

`REPLY_JOIN_GAME (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | SUCCEED_OR_NOT (1 Byte)`

`REQUEST_PLACE_A_SHIP (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte) | SHIP_TYPE (4 Byte) | LOCATION (4 Byte) | DIRECTION (1 Byte)`

`REPLY_PLACE_A_SHIP (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | SUCCEED_OR_NOT (1 Byte)`

`REQUEST_READY (1Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)`

`REPLY_START_GAME (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | SUCCEED_OR_NOT (1 Byte)`

`REQUEST_ATTACK (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte) | LOCATION (4 Byte) | DIRECTION (1 Byte)`

`REPLY_ATTACK (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | SUCCEED_OR_NOT (1 Byte) | SINK_SHIP_TYPE_DURING_ATTACK (1 Byte)`


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

1. ship type

2. size

3. remaining life

4. head location

5. direction (vertical or horizontal)
