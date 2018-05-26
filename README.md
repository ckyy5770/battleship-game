# battleship-game

implementation of classic [Battleship Game](https://en.wikipedia.org/wiki/Battleship_(game))

## Dependencies

1. asio for networking

2. tclap for command parsing

3. opengl and glfw for graphics

## A Glance of Game

![battleship-gameplay](https://github.com/iloveyoukcl5770/battleship-game/raw/master/imgs/battleship_gameplay_screenshot.png)

## Game Play

for now, this implementation of the game is just for testing different battleship shooting strategies, i.e., it can only be played by two computers.

I implemented 4 kind of shooting strategy, they are:

1. random shooting

2. DFS based attack

3. probability based attack

4. combination of 2 and 3.

## Design (VERY incomplete, consider diving into code instead)

### Overview

#### Client

Client will connect directly with each other and there is no need for a central server.

There is a difference between clients though. Some of the clients are meant to listen on specific port and wait for some of other clients to connect them. So we define two types of client: kListener and kInitiator. ~~In the following conversations, the kInitiator will talk first all the time.~~

After connection established,

1. both clients will send a INFO_GAME_ID to each other and try to verify if they have the same game id.

2. after that, the client will place its ships locally and send a INFO_READY when finishing up.

3. then they will try to read from the socket and expecting a INFO_READY from the other side.

4. upon receiving the ready signal from the other, they will send a INFO_ROLL with a random number from 0 to 99 to decide which one should attack first. (higher one attack first, and if the numbers are equal, re-roll and send again).

##### Message Format

All messages have following format:

`MESSAGE_TYPE (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | PAYLOAD`

Following are specifics of all type of messages:

`REQUEST_ATTACK (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte) | LOCATION (4 Byte) | DIRECTION (1 Byte)`

`REPLY_ATTACK (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | SUCCEED_OR_NOT (1 Byte) | SINK_SHIP_TYPE_DURING_ATTACK (1 Byte) | ATTACKER_WIN_OR_NOT (1 Byte)`


`INFO_GAME_ID (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | GAME_ID (4 Byte)`

`INFO_READY (1Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | GAME_ID (4 Byte)`

`INFO_ROLL (1Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | GAME_ID (4 Byte) | RANDOM_NUMBER (4 Byte)`



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
