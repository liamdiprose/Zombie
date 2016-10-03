# Team 124 ENCE Assignment


## Meeting 29 Sep

### Zombie Game
* Zombies slowly approach players (who ever is closer)
* Zombies beside player for more than a tick are hit
* Hits reduce a players health
* Health is shown to the player as a quickening heart beat LED (and maybe sound)
* Zombies move a different times to spread out the work needed by the CPU in a single loop
* Player can run as fast as they can push the navstick
* Extra features: Ckatctus, etc


### Server-Client or Shared-workload
* Cannot strain the communicaton 
* Could utilise "clients" CPU time and RAM
* Server only needs to update zombies around the player

### Technical
* Game is syncronised with a master counter
	* Every process (i.e. heartrate LED, zombie movement) uses master counter to determine when to run
	* -- Do both games need the same time?
	 
* Drawing is completely seperated from game logic
* Zombies are keep as a list (indexed based on when zombie updates), x and y coordinates are saved as value
	* Faster updating

* Or: integers are stored in a two dimentional array where the integer is the time the zombie updates
	* Faster navigation

TODO:
* Determine mathematical functions for determining when things happen based on a continuous counter (Use hardware counter?) and other factors such as speed
  * Heart rate LED
  * Zombie update
  * Player update
  * IR serial
  * Redraws
  * Not screw up

* Investigate drawing of map 
* Tasks
