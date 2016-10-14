# Team 124 ENCE Assignment

A project by Liam Diprose and Jeremy Craig
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
* Zombies are keep as a booleans in a 2D boolean array

* Or: integers are stored in a two dimentional array where the integer is the time the zombie updates
	* Faster navigation

## Modified Drivers
* Two drivers have been modified, in regards to increasing the board refresh rate
	* ir_uart.h:  changed IR_UART_BAUD_RATE from 2400 to 3000  
	* timer.h:    changed TIMER_CLOCK_RATE from 256 to 1

## Copied Code Snippits
* We have sampled to pieces of code from off the internet, that being the queue in communications.c and the Point struct in point.h

## Known Bugs
* 13.10.2016
	* Client only draws the first zombie on each row
	* IR Communication is not as optimised as desired and impacts screen display
