#include "globals.h"

// this is the array that will hold our sequence.
// This array will only contain 0, 1, 2, or 3.
uint8_t fullSequence[GLOBALS_MAX_FLASH_SEQUENCE] = {0};
// this is simply the length of the above array
uint16_t length = 0;
// this is the level we're on.
// In my code, length and level will be equal almost always.
uint16_t level;

// this takes in an array and a lenght integer.
// it will set the first llength items in fullSequence to the items in sequence[]
// it will also save the length as a global variable
void globals_setSequence(const uint8_t sequence[], uint16_t llength){
    for (uint8_t i = 0; i <= llength; i++){
        fullSequence[i] = sequence[i];
    }
    length = llength;
}

// This returns the value of the sequence at the index.
uint8_t globals_getSequenceValue(uint16_t index){
    return fullSequence[index];
}

// Retrieve the sequence length.
uint16_t globals_getSequenceLength(){
    return length;
}

// This is the length of the sequence that you are currently working on.
void globals_setSequenceIterationLength(uint16_t llength){
    level = llength - 1;
}

// This is the length of the sequence that you are currently working on (not the maximum length but the interim length as
// the use works through the pattern one color at a time.
uint16_t globals_getSequenceIterationLength(){
    return level;
}