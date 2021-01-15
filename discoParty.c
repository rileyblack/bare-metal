/* 
 * Blinking light program for initial configuration
 */

#include "address_map_arm.h"					// All system addresses mappings

volatile int DELAY_LENGTH;						// volatile so it survives compilation

int main(void) {

    volatile int * LED_ptr = (int *)LED_BASE;	// LED address
    volatile int delay_count;					// Controls delay between blink
	volatile int status;						// Tracks if light is on or off
	
	DELAY_LENGTH = 700000;
	status = 0;									// Starting as off
	
    while (1) 
	{
		if (status == 0)						// If off
		{
			status = 1;							// Update tracking variable
			*(LED_ptr) |= 0x1;					// Turn on
		}
		else									// If on
		{
			status = 0;							// Update tracking variable
			*(LED_ptr) &= ~0x1;					// Turn off
		}

        for (delay_count = DELAY_LENGTH; delay_count != 0; --delay_count)
            ;									// delay loop
    }
}
