
/*
 * switches.cpp
 *
 *  Created on: Sep 13, 2016
 *      Author: pjacobr
 */

#include "switches.h"

#define SWITCHES_GPIO_INPUT 0xF
#define SWITCHES_GPIO_DATA_OFFSET 0x0
#define SWITCHES_GPIO_TRI_OFFSET 0x04
#define SWITCHES_GPIO_END_VALUE 0xF

/* reads the 32 bit value from the register and returns it.
* @Param offset:int32_t
* This does not necessarily need to be in the .h file because it is a low level function
* This is connected to the switches side
*/
int32_t switches_readGpioRegister(int32_t offset) {
 //just use the low-level Xilinx call.
    return Xil_In32(XPAR_SLIDE_SWITCHES_BASEADDR + offset);
}

/*
* Writes current value of the switches to the register to keep everything up to date.
* @Param: offset : int32_t
* @Param: value : int32_t; value of the switches
* This one does not need to be in the “.h” because it is a low level function that does should not be seen *by the user.
*/
void switches_writeGpioRegister(int32_t offset, int32_t value)
{
    Xil_Out32(XPAR_SLIDE_SWITCHES_BASEADDR + offset, value);
}
/*
* Reads and returns the current value of the registry (only the last 4 bits) by calling the read GPIO Register
* @Param: None
* returns a 32 bit integer
*/
int32_t switches_read()
{
   return switches_readGpioRegister(SWITCHES_GPIO_DATA_OFFSET);
}
//for this lab we will just assume that it will always work correctly.
// This writes to the register with an offset of 0 and all 1’s as the value
int32_t switches_init()
{
  switches_writeGpioRegister(SWITCHES_GPIO_TRI_OFFSET, SWITCHES_GPIO_INPUT);
  return SWITCHES_INIT_STATUS_OK;
}
/* Runs a test that the switches are correctly connected and working.
*  This test connects each switch to an LED and if the switch is in the ON position it will turn the LED ON.
* If the switch is in the off position is will turn it off. If at any point, all the switches are turned on, the pro-
* gram finishes and the LED’s are all turned off.
* @ Param: None Files that are used: “supportFiles/leds.h”
* First initialize switches, then initialize the LEDs, then allow the LEDs to turn off and on until all are
* turned on.
*/
void switches_runTest(){

      //initialize the switches
      switches_init();

      // This will init the GPIO hardware so you can write to the 4 LEDs  (LD3 - LD0) on the ZYBO board.
      // if printFailedStatusFlag = 1, it will print out an error message if an internal error occurs.
      leds_init(false);

      // This write the lower 4 bits of ledValue to the LEDs.
      // LED3 gets bit3 and so forth.
      // '1' = illuminated.
      // '0' = off.
      while(switches_read() != SWITCHES_GPIO_END_VALUE)
      {
          leds_write(switches_read());
      }
      leds_write(0x0);

}
