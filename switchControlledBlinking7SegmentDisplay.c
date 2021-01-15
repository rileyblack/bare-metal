/*
 * Switch controlled blinking 7-segment display
 */

#include "address_map_arm.h"									// All system addresses mappings

// Retrieves switch positions (hi/lo)
int ReadSwitches(void)
{
	volatile int *SW_ptr = (int*)SW_BASE;						// Create pointer to switch address
    return (*(SW_ptr)%16);										// Return just the first 4 switch bits 2^4 = 16
}

// Displays hexadecimal onto 7-segment display
void DisplayHex(int value)
{
	volatile int *HEX3_HEX0_BASE_ptr = (int*)HEX3_HEX0_BASE;	// Create pointer to 7-segment display
	unsigned char lookUpTable[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00};
	*(HEX3_HEX0_BASE_ptr) = lookUpTable[value];					// Return hexadecimal setting proper segments high
}

// Blinking control flow
int main(void) {
	volatile int DELAY_LENGTH;									// volatile so it survives compilation
	volatile int delay_count;									// Controls delay between blink
	volatile int status;										// Tracks if light is on or off
	
	DELAY_LENGTH = 700000;
	status = 0;													// Starting as off
	
	while (1)
	{
		if (status == 0)										// If off
		{
			status = 1;											// Update tracking variable
			DisplayHex(ReadSwitches());							// Display hexadecimal 
		}
		else{
			DisplayHex(16);										// Turn off display
			status = 0;											// Update tracking variable
		}
		for (delay_count = DELAY_LENGTH; delay_count != 0; --delay_count)
                                                ; // delay loop
        }
}
