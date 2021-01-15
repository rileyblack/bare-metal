/*
 * Push-Button controlled 7-segment display stopwatch
 */

#include "address_map_arm.h"									// All system addresses mappings

int main(void) {

	volatile int bigmin = 0;									// Position X in X0:00:00
	volatile int smallmin = 0;									// Position X in 0X:00:00
	volatile int bigsec = 0;									// Position X in 00:X0:00
	volatile int smallsec = 0;									// Position X in 00:0X:00
	volatile int bighun = 0;									// Position X in 00:00:X0
	volatile int smallhun = 0;									// Position X in 00:00:0X
	
	volatile int * PBptr = (int*)KEY_BASE;						// Pointer to push button address
	volatile int * TIMERptr = (int*)HPS_TIMER0_BASE;			// Pointer to timer address
	volatile int * LOWHEXptr = (int*)HEX3_HEX0_BASE;			// Pointer to 7-segment display positions X in 00:XX:XX
	volatile int * HIGHHEXptr = (int*)HEX5_HEX4_BASE;			// Pointer to 7-segment display positions X in XX:00:00

	unsigned char lookUpTable[] = { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67 }; // For setting proper segments high for each hex number

	*(LOWHEXptr) = 0x3F3F3F3F;									// Starting 7-segment display at 00:00:00
	*(HIGHHEXptr) = 0x3F3F;										// Starting 7-segment display at 00:00:00

	while (1)													// Starting stopwatch counting
	{
		*(TIMERptr + 2) = 0x00000000;							// Setting timer enable bit to 0 to ensure stopped
		*(TIMERptr) = 0x000055F0;								// Loading timer count down value 
		*(TIMERptr + 2) = 0x00000003;							// Setting timer enable bit to 1 to start count down

		while (1) {												// Counting down timer
			if (*(TIMERptr + 1) <= 0) {							// When timer has expired
				break;											// Time to update display
			}
			if(*(PBptr) == 0x00000002){							// checking If the push button has been clicked
				while(*(PBptr) != 0x00000001){}					// if clicked, wait until it is released to continue
			}
		}

		smallhun++;												// Increment position X in 00:00:0X

		if (smallhun > 9) {										// Check if rollover next position
			smallhun = 0;										// If rollover, reset position X in 00:00:0X
			bighun++;											// and increment position X in 00:00:X0
		}

		if (bighun > 9) {										// Check if rollover next position
			bighun = 0;											// If rollover, reset position X in 00:00:X0
			smallsec++;											// and increment position X in 00:0X:00
		}

		if (smallsec > 9) {										// Check if rollover next position
			smallsec = 0;										// If rollover, reset position X in 00:0X:00
			bigsec++;											// and increment position X in 00:X0:00
		}

		if (bigsec > 5) {										// Check if rollover next position
			bigsec = 0;											// If rollover, reset position X in 00:X0:00
			smallmin++;											// and increment position X in 0X:00:00
		}

		if (smallmin > 9) {										// Check if rollover next position
			smallmin = 0;										// If rollover, reset position X in 0X:00:00
			bigmin++;											// and increment position X in X0:00:00
		}

		if (bigmin > 9) {										// Check if rollover past full display
			bigmin = 0;											// Reset all positions to 0
			smallmin = 0;										// Reset all positions to 0
			bigsec = 0;											// Reset all positions to 0
			smallsec = 0;										// Reset all positions to 0
			bighun = 0;											// Reset all positions to 0
			smallhun = 0;										// Reset all positions to 0
		}

		int dis1 = lookUpTable[smallhun];						// Retrieve hex value to display proper decimal at position X in 00:00:0X
		int dis2 = lookUpTable[bighun];							// Retrieve hex value to display proper decimal at position X in 00:00:X0
		int dis3 = lookUpTable[smallsec];						// Retrieve hex value to display proper decimal at position X in 00:0X:00
		int dis4 = lookUpTable[bigsec];							// Retrieve hex value to display proper decimal at position X in 00:X0:00
		int dis5 = lookUpTable[smallmin];						// Retrieve hex value to display proper decimal at position X in 0x:00:00
		int dis6 = lookUpTable[bigmin];							// Retrieve hex value to display proper decimal at position X in X0:00:00

		*(LOWHEXptr) = (dis4 << 24) | (dis3 << 16) | (dis2 << 8) | dis1; // Concatinating lower 4 position hexadecimals into one value and writing to lower 7-segment display address 
		*(HIGHHEXptr) = (dis6 << 8) | dis5;						// Concatinating upper 2 position hexadecimals into one value and writing to upper 7-segment display address 
	}
}
