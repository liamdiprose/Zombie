# IR quick documentation


## Timekeeping

* Cleint waits on the servers communication, this will set inherintly set the clients loop rate to the servers,
* Server is independant of client, if cleint disconnects, server will (either pause with an additional condition) not break

## Initiation

1. Server waiting for client to respond to broadcasts
2. Client responds to a server broadcast
3. Game is setup, client given game info

## Game loop

1. Client waits for server to send player 1 position, and relevant zombies (see [Update](#update-procedure) )
2. Client draws zombies/player
3.  TODO


## Update Procedure

(Client awaiting server to start sending game data)
(Server set up the game, counting down to game start)

1. Server sends announcement, containing:
    * Annoucement Magic Number
    * What will be sent, any number of:
        * State message: P1 lose, P2 lose, etc
        * Player position
        * Zombies position
2. (On client) If announcement contains player position, receive and update player position
3. Client send own player position as response
4. (On client) If announcement contains zombies, receive and place zombies until "finish" received

If errors in checksum is found, discard and dont retry? Ensure display doesnt stop

