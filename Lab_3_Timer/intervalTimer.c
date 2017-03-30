/*
 * intervalTimer.c
 *
 *  Created on: Sep 15, 2016
 *      Author: pjacobr
 */
#include <stdio.h>
#include <xparameters.h>
#include <xil_io.h>
#include "intervalTimer.h"
#include "stdbool.h"

//Redefinition of the register addresses this will allow for the read/write to the registers
#define TIMER_ADDRESS_TMR0 XPAR_AXI_TIMER_0_BASEADDR
#define TIMER_ADDRESS_TMR1 XPAR_AXI_TIMER_1_BASEADDR
#define TIMER_ADDRESS_TMR2 XPAR_AXI_TIMER_2_BASEADDR

//Offsets for the various registers
//TCSR are the main registers
#define TCSR1_OFFSET 0x10
#define TLR1_OFFSET 0x14
#define TCR1_OFFSET 0x18
//Timer 0 register values2
#define TCSR0_OFFSET 0x00
#define TCR0_OFFSET 0x08
#define TLR0_OFFSET 0x04

#define RESET_TCSR0 0x00
#define RESET_TCSR1 0x01
//Set the cascade bit on the register 1 to 1 so that is will automatically overflow
#define CASC_MASK_TCSR0 0x800
#define UDT_MASK_TCSR0 0x02
#define SET_ENT0_BIT 0x080//Set the 7th bit to 1
#define TURN_OFF_ENT0_BIT 0xFFFFFF7F//Set the 7th bit to 0

/*Load value masks for pushing onto the start*/
#define TCSR0_LOAD_0_MASK 0x20
#define TCSR1_LOAD_1_MASK 0x20

#define RESET_REG 0x0
/*Clock frequencies by timer*/
#define CLOCK_0_FREQ XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ// frequency of timer number 0
#define CLOCK_1_FREQ XPAR_AXI_TIMER_1_CLOCK_FREQ_HZ// frequency of timer number 1
#define CLOCK_2_FREQ XPAR_AXI_TIMER_2_CLOCK_FREQ_HZ// frequency of timer number 2

#define UNIT_CONVERTER_0 1.0/CLOCK_0_FREQ //forces the constant ratio to a double
#define UNIT_CONVERTER_1 1.0/CLOCK_1_FREQ //forces the constant ratio to a double
#define UNIT_CONVERTER_2 1.0/CLOCK_2_FREQ //forces the constant ratio to a double
//For shifting the 32 upper bits onto the 64 bit number
#define SHIFT_32_BITS 32;
#define TIMER_DELAY 1000

/*
 * using the "xil_io.h" we can write to the registers
 * helper function that calls the Xil_Out32() function to write the current value
 *  @Param :  register offset
 *  @Param :  value to read to the register
 *  @Param :  timer number
 */
void timer_writeRegister(int32_t offset, int32_t value, int32_t timerNumber)
{
    if(timerNumber == INTERVAL_TIMER_TIMER_0)//Check the timer that we would like to write to
    {
        Xil_Out32(TIMER_ADDRESS_TMR0 + offset, value); // write the value specified
    }
    else if(timerNumber == INTERVAL_TIMER_TIMER_1)//Check the timer: TIMER 1
    {
        Xil_Out32(TIMER_ADDRESS_TMR1 + offset, value);//Write to the address 1
    }
    else if(timerNumber == INTERVAL_TIMER_TIMER_2)//Check for timer 2
    {
        Xil_Out32(TIMER_ADDRESS_TMR2 + offset, value);//write to the base address of timer 2
    }
}

/*
 * using the "xil_io.h" we can read from the registers
 * helper function that calls the Xil_In32() function to read the current value
 *  @Param :  register offset
 *  @Param :  timer number
 */
int32_t timer_readRegister(int32_t offset, int32_t timerNumber) {
 //just use the low-level Xilinx call.

    /*
     * Check specified timer that need to be checked
     * return the read value from the register
     * else return false
     */
    if(timerNumber == INTERVAL_TIMER_TIMER_0)//TIMER 0
    {
        return Xil_In32(TIMER_ADDRESS_TMR0 + offset);
    }
    else if(timerNumber == INTERVAL_TIMER_TIMER_1)//TIMER 1
    {
        return Xil_In32(TIMER_ADDRESS_TMR1 + offset);
    }
    else if(timerNumber == INTERVAL_TIMER_TIMER_2)// TIMER 2
    {
        return Xil_In32(TIMER_ADDRESS_TMR2 + offset);
    }
    else
    {
        return INTERVAL_TIMER_STATUS_FAIL;// FAILED
    }
}

// You must initialize the timers before you use them.
// timerNumber indicates which timer should be initialized.
// returns INTERVAL_TIMER_STATUS_OK if successful, some other value otherwise.
/****************************************
 * It is important to set the CASC bit precisely and to turn off the UDT bit
 * Forcing the values onto the registers checks that it happens
******************************************/
intervalTimer_status_t intervalTimer_init(uint32_t timerNumber)
{
    if(timerNumber == INTERVAL_TIMER_TIMER_0 || timerNumber == INTERVAL_TIMER_TIMER_1 || timerNumber == INTERVAL_TIMER_TIMER_2 )
    {
        timer_writeRegister(TCSR0_OFFSET, RESET_TCSR0, timerNumber);//Write a 0 to the TCSR0 register
        timer_writeRegister(TCSR1_OFFSET, RESET_TCSR1, timerNumber);//Write a 0 to the TCSR1 register

        //Set the CASCADE bit to high
        int32_t value = timer_readRegister(TCSR0_OFFSET, timerNumber) | CASC_MASK_TCSR0;
        timer_writeRegister(TCSR0_OFFSET, value, timerNumber);//set the CASC bit and clear the UDT0 bit in the TCSR0 reg
        //Set the UDT_MASK_TCSR0 to 0
		//Take the current reg value and bitwise and it with the UDT bit to make sure that the bit is set to 0;
		//Then write it back onto the reg
        value = timer_readRegister(TCSR0_OFFSET, timerNumber) & ~(UDT_MASK_TCSR0) ;
        timer_writeRegister(TCSR0_OFFSET, value, timerNumber);

        return INTERVAL_TIMER_STATUS_OK;
    }
    else//if the timer is not 0, 1, or 2 then do not write
    {
        return INTERVAL_TIMER_STATUS_FAIL;
    }



}
/*
* FUNCTION: READ CURRENT VALUE ON INTERVAL TIMER
* @PARAM : timerNumber
* 		Determine which timer would like to be accessed
* take the lower and upper 32 bits from each reg and concatenate them together
* 
*
*/
uint64_t intervalTimer_reader(int32_t timerNumber)
{
 uint32_t counter0_reading = 0;//lower 32 bits
 uint32_t counter1_reading = 0;//higher 32 bits
 uint32_t counter1_reading_2 = 0;//higher 32 bits

 uint64_t timer_reading = 0;//stores the read values

 counter1_reading = timer_readRegister(TCR1_OFFSET, timerNumber);//read from reg 1
 counter0_reading = timer_readRegister(TCR0_OFFSET, timerNumber);//read from reg 0
 counter1_reading_2 = timer_readRegister(TCR1_OFFSET, timerNumber);//read from reg 1
 //Check for the anamolie
 while(counter1_reading != counter1_reading_2)
 {
     counter1_reading = timer_readRegister(TCR1_OFFSET, timerNumber);//read from reg 1
     counter0_reading = timer_readRegister(TCR0_OFFSET, timerNumber);//read from reg 0
     counter1_reading_2 = timer_readRegister(TCR1_OFFSET, timerNumber);//read from reg 1
 }
 /*CONCATENATION
 * push upper bits onto 64 bit number
 * shift bits and then bit wise or with timer_reading
 */
 timer_reading = counter1_reading;//load first 32 bit number onto 64 bit number
 timer_reading = timer_reading << SHIFT_32_BITS; //Shift the bits over 32 bits
 timer_reading = timer_reading | counter0_reading;//Concatenate the last 32 bits onto the number

 //Check the timer input to make sure that it reads in valid timer number
     return timer_reading;
}

// This is a convenience function that initializes all interval timers.
// Simply calls intervalTimer_init() on all timers.
// returns INTERVAL_TIMER_STATUS_OK if successful, some other value otherwise.
intervalTimer_status_t intervalTimer_initAll()
{
    if(intervalTimer_init(INTERVAL_TIMER_TIMER_0)&&//Timer 1 initialized!
    intervalTimer_init(INTERVAL_TIMER_TIMER_1)&&//Timer 2 initialized!
    intervalTimer_init(INTERVAL_TIMER_TIMER_2))//Timer 3 initialized!
    {
        return INTERVAL_TIMER_STATUS_OK;
    }
    else//If not all of the timers were correctly initialized then it returns false
    {
        //printf("InitAll failed!\n\r");
        return INTERVAL_TIMER_STATUS_FAIL;
    }

}



// This function starts the interval timer running.
// timerNumber indicates which timer should start running.
void intervalTimer_start(uint32_t timerNumber)
{

    //Read current value off the register and bit wise or it with the ENT0 bit to set that bit to 1
    int32_t value = (timer_readRegister(TCSR0_OFFSET, timerNumber) | SET_ENT0_BIT);
    timer_writeRegister(TCSR0_OFFSET, value, timerNumber);
    //timer_writeRegister(offset, value, timerNumber);


}

// This function stops the interval timer running.
// timerNumber indicates which timer should stop running.
void intervalTimer_stop(uint32_t timerNumber)
{
    //Read current value off the register and bit wise or it with the ENT0 bit to set that bit to 0
     int32_t value = (timer_readRegister(TCSR0_OFFSET, timerNumber) & TURN_OFF_ENT0_BIT);
     timer_writeRegister(TCSR0_OFFSET, value, timerNumber);
    //timer_writeRegister(offset, value, timerNumber);

}

// This function resets the interval timer.
// timerNumber indicates which timer should reset.
void intervalTimer_reset(uint32_t timerNumber)
{
	/*RESET the lower register*/
    timer_writeRegister(TLR0_OFFSET, RESET_REG, timerNumber);//write a 0 into the TLR0 reg
    int32_t value = (timer_readRegister(TCSR0_OFFSET, timerNumber) | TCSR0_LOAD_0_MASK);
    timer_writeRegister(TCSR0_OFFSET, value, timerNumber);//write a 1 into the LOAD0 bit in TCSR0

    /*RESET the first register*/
    timer_writeRegister(TLR1_OFFSET, RESET_REG, timerNumber);//write a 0 into the TLR1 reg
    value = (timer_readRegister(TCSR1_OFFSET, timerNumber) | TCSR1_LOAD_1_MASK);
    timer_writeRegister(TCSR1_OFFSET, value, timerNumber);//write a 1 into the LOAD1 bit of the TCSR1

    intervalTimer_init(timerNumber);//init

}

// Convenience function for intervalTimer_reset().
// Simply calls intervalTimer_reset() on all timers.
void intervalTimer_resetAll()
{

   intervalTimer_reset(INTERVAL_TIMER_TIMER_0);//resets timer #0 to zero
   intervalTimer_reset(INTERVAL_TIMER_TIMER_1);//resets timer #1 to zero
   intervalTimer_reset(INTERVAL_TIMER_TIMER_2);//resets timer #2 to zero

}


// Runs a test on a single timer as indicated by the timerNumber argument.
// Returns INTERVAL_TIMER_STATUS_OK if successful, something else otherwise.
intervalTimer_status_t intervalTimer_test(uint32_t timerNumber)
{
   printf("start test!\n\r");
   //Getter variables for maintaining the data
   int64_t reset_read_value;
   int64_t stop_value;
   int64_t start_value_1;
   int64_t start_value_2;
   //Checker bools. Allow us to check for any errors along the way.
   bool reset_error = false;
   bool start_error = false;
   bool stop_error = false;
   //Initialize the timer
   intervalTimer_init(timerNumber);


   //start the timer
   intervalTimer_start(timerNumber);
   //read the value from the register
   start_value_1 = intervalTimer_reader(timerNumber);
   int i;

   for (i = 0; i < TIMER_DELAY; i++)
   {

   }
   //stop the timer
   intervalTimer_stop(timerNumber);
   //read from the register
   stop_value = intervalTimer_reader(timerNumber);
   //start the timer
   intervalTimer_start(timerNumber);
   //read from the register
   start_value_2 = intervalTimer_reader(timerNumber);
   //reset
   intervalTimer_reset(timerNumber);
   //read from the register
   reset_read_value = intervalTimer_reader(timerNumber);

   /*
    * Check the functionality of the above tests. We will check the values to make sure they are fitting
    * if start 1 and start 2 are not different then we have a problem
    * if the reset does not make the timer stop totally then we have a problem
    */
	//Check that the two start values are not the same and that neither of them are at 0
   if((start_value_2 < start_value_1) && (start_value_1 != 0) && (start_value_2 != 0))
   {
	   //print out an error message and set the bool to true signalling that an error has occured
       printf("The start values are the same, TEST 1 Failed\n\r");
       start_error = true;

   }
   if(stop_value < 0)/*The stop value should be positive*/
   {
       printf("The stop value is less than 0, TEST 2 Failed\n\r");
       stop_error = true;
   }
   if(reset_read_value != 0)//make sure the reset value is 0 
   {
       printf("The reset value is not 0\n\r");
       reset_error = true;
   }
	/*
	* If any errors occured return a system failure 
	* otherwise return true
	*/
   if(reset_error || stop_error || start_error)
   {
      return INTERVAL_TIMER_STATUS_FAIL;
   }else
   {
      return INTERVAL_TIMER_STATUS_OK;
   }

}

// Convenience function that invokes test on all interval timers.
// Returns INTERVAL_TIMER_STATUS_OK if successful, something else otherwise.
intervalTimer_status_t intervalTimer_testAll()
{   /*Run test on timer number 0*/
    if(!(intervalTimer_test(INTERVAL_TIMER_TIMER_0)))
    {
        return INTERVAL_TIMER_STATUS_FAIL;
    }
    else if(!(intervalTimer_test(INTERVAL_TIMER_TIMER_1)))
    {
        return INTERVAL_TIMER_STATUS_FAIL;
    }
    else if(!(intervalTimer_test(INTERVAL_TIMER_TIMER_2)))
    {
        return INTERVAL_TIMER_STATUS_FAIL;
    }
    else
    {
        return INTERVAL_TIMER_STATUS_OK;
    }

}

// Once the interval timer has stopped running, use this function to
// ascertain how long the timer was running.
// The timerNumber argument determines which timer is read.
double intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber)
{
    if(timerNumber == INTERVAL_TIMER_TIMER_0)//TIMER 0
     {
		 //Cast the 64 bit int and the frequency to a double to ensure proper arithmetic
         return ((double)intervalTimer_reader(timerNumber)/(double)CLOCK_0_FREQ );//Check the timer and return the correct timer value
     }
     else if(timerNumber == INTERVAL_TIMER_TIMER_1)//TIMER 1
     {
		 //Cast the 64 bit int and the frequency to a double to ensure proper arithmetic
         return ((double)intervalTimer_reader(timerNumber)/(double)CLOCK_1_FREQ );//Check the timer and return the correct timer value
     }
     else if(timerNumber == INTERVAL_TIMER_TIMER_2)//TIMER 2
     {
		 //Cast the 64 bit int and the frequency to a double to ensure proper arithmetic 
         return ((double)(intervalTimer_reader(timerNumber)/(double)CLOCK_2_FREQ));//Check the timer and return the correct timer value
     }
     else
     {
         printf("TEST FAILED\n\r");
        return uint64_t(INTERVAL_TIMER_STATUS_FAIL);
     }


}



