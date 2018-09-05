#include "nil_RingBuff.h"

int RB_MainCount = 0;	// Used in read/write buffer
int RB_ReadCount = 0; // Used in read buffer

int RB_PreviousCount = 0;	// Used in write buffer
int RB_BufferPlaceholder = 0;	// Used in read/write buffer
int RB_LoopCount = 0;	// This can be either 1 or 0
int RB_State = 0;	// Used in write buffer
int RB_tmpLoopState = 0;	// Used in write buffer
char RB_WriteArray[Buffer_Size] = {'\0'};	// Used in write buffer

void WriteRingBuffer(char* string){

	int RB_DataCount = 0; // Used in write buffer
	int RB_Write = 1;	// Used in write buffer

	// Ring buffer write data into buffer
	while (RB_Write){

		if (RB_MainCount == Buffer_Size - 1){	// This checks that when the ring buffer reaches the max it returns to 0
			RB_MainCount = 0;
		}

		else {

			RB_WriteArray[RB_MainCount] = string[RB_DataCount];	// Puts data into write array

			if ((RB_MainCount == RB_PreviousCount) && (RB_tmpLoopState == 1)){	// This check is to see if the buffer looped while writing data in
				RB_LoopCount = 1;	// If looped then set loop to 1, this will be used when
				RB_State = 0;	// Set loop state so that the new count value can be entered into previousCount;
			}

			if (RB_State == 0) {	// To check if the ring buffer overlaps when data is written in
				RB_PreviousCount = RB_MainCount;	// Previous count is equal to current count
				RB_State = 1;	// Make state = 1 so that it only gets triggered at the beginning and end of data(for next time when it writes data in)
				RB_tmpLoopState = 0;	// This is just sets the temporary loop state to 0, otherwise the buffer looped will not work correctly
			}

			RB_MainCount++;	// Increments count
			RB_DataCount++;	// Increments a count used when writing in data into the ring buffer
			RB_tmpLoopState = 1;

			if (string[RB_DataCount] == '\0') {
				RB_Write = 0;	// This will exit the while loop
				RB_State = 0;	// Set loop state so that the new count value can be entered into previousCount

				if (RB_MainCount == 0){
					RB_BufferPlaceholder = Buffer_Size - 1;	// Set placeholder of last character received, cannot use -1 here because of 0
				} else {
					RB_BufferPlaceholder = RB_MainCount; // Set placeholder of last character received
				}

			}
		}
	} // End of write ring buffer

}





char *ReadRingBuff(){
	int tmpReadCount = 0;
	int counter_for_loop = 0;
	int counter_for_loop_state = 0;
	int read = 1;	// Used in read buffer

	/* This for loop just clears the array, otherwise old values are still seen */
	for (int a = 0; a < Buffer_Size; a++){
		readArray[a] = '\0';
	}

	if (RB_LoopCount == 1) {	// loop this works
		RB_ReadCount = RB_BufferPlaceholder;
	}

	while (read){

		if (RB_ReadCount == Buffer_Size - 1){	// This checks that when the ring buffer reaches the max it returns to 0
			RB_ReadCount = 0;
		}

		else if (RB_LoopCount == 0) {	// If loop is 0 means that the buffer did not loop when writing

			readArray[tmpReadCount] = RB_WriteArray[RB_ReadCount];	// Reads the data from the writeArray into readArray

			tmpReadCount++;
			RB_ReadCount++;

			if (RB_ReadCount == RB_MainCount){	// Checks if read count matches the write count
				RB_ReadCount = RB_BufferPlaceholder;	// read count is now switched to current reading position for next time
				read = 0;	// Read is 0 so that it can exit the while loop. Will need to maybe add a timeout also.
			}

		}

		else if (RB_LoopCount == 1) {	// If loop is 1 means that the buffer did loop when writing

			if (counter_for_loop_state == 0){	// Counter state for loop so that the counter value is updated only the first time
				counter_for_loop = RB_MainCount;
				counter_for_loop_state = 1;
			}

			if (counter_for_loop == Buffer_Size - 1){	// This checks that when the ring buffer reaches the max it returns to 0
					counter_for_loop = 0;
			}

			readArray[tmpReadCount] = RB_WriteArray[counter_for_loop]; // Reads characters from write array into read array

			tmpReadCount++;
			counter_for_loop++;

			if (counter_for_loop == RB_BufferPlaceholder){	// Checks if read count matches the write count
				read = 0;
				RB_LoopCount = 0;	// Reset loop state
				counter_for_loop_state = 0;
			}

		}

	}	// End of read ring buffer
	return readArray;
}
