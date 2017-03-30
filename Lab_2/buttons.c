/*
 * buttons.c
 *
 *  Created on: Sep 13, 2016
 *      Author: pjacobr
 */
#include "buttons.h"
#include "xparameters.h"
#include "xil_io.h"
#include "stdbool.h"

#define BUTTONS_GPIO_DEVICE_BASE_ADDRESS XPAR_PUSH_BUTTONS_BASEADDR
#define BUTTONS_GPIO_INPUT 0xF
#define BUTTONS_GPIO_DATA_OFFSET 0x0
#define BUTTONS_GPIO_TRI_OFFSET 0x04
#define BUTTONS_GPIO_END_VALUE 0xF
#define PRINT_BTN0 "BTN0"
#define PRINT_BTN1 "BTN1"
#define PRINT_BTN2 "BTN2"
#define PRINT_BTN3 "BTN3"
//These apply to the testing of the function
#define BUTTONS_DISPLAY_UPPER_LIMIT 0
#define BUTTONS_DISPLAY_HEIGHT 80
#define BUTTONS_DISPLAY_WIDTH 120
#define BUTTONS_DISPLAY_TEXT_SIZE 2.5
#define BUTTONS_DISPLAY_CURSOR_UPPER_BNDS 55
#define BUTTONS_DISPLAY_CURSOR_X_BTN3 20
#define BUTTONS_DISPLAY_RECT_X_BTN0 240
#define BUTTONS_DISPLAY_CURSOR_X_BTN0 260
#define BUTTONS_DISPLAY_CURSOR_X_BTN1 180
#define BUTTONS_DISPLAY_RECT_X_BTN1 160
#define BUTTONS_DISPLAY_CURSOR_X_BTN2 100


//Note that I have defined my own BUTTONS_GPIO_DEVICE_BASE_ADDRESS
//Helper function to read GPIO registers.
//Reads the current value of the register and the 4 bit offset. The first 28 bits will be ignored and the lower //4 bits will represent the logic values of the push buttons.
// parameter: 32 bit integer that will determine the offset of the bits.
int32_t buttons_readGpioRegister(int32_t offset)
{
 //just use the low-level Xilinx call.
    return Xil_In32(BUTTONS_GPIO_DEVICE_BASE_ADDRESS + offset);
}

void buttons_writeGpioRegister(int32_t offset, int32_t value)
{
    Xil_Out32(BUTTONS_GPIO_DEVICE_BASE_ADDRESS + offset, value);
}
// Returns the current value of all 4 buttons as the lower 4 bits of the returned value.
// bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.

int32_t buttons_read()
{
    return buttons_readGpioRegister(BUTTONS_GPIO_DATA_OFFSET);
}
// Initializes the button driver software and hardware. Returns one of the defined status values (above).
// I don't relly know what to do for this one... ask a TA or Corbin
int32_t buttons_init()
{
    buttons_writeGpioRegister(BUTTONS_GPIO_TRI_OFFSET,
                                             BUTTONS_GPIO_INPUT);
    return BUTTONS_INIT_STATUS_OK;
}

// Runs a test of the buttons. As you push the buttons, graphics and messages will be written to the LCD
// panel. The test will until all 4 pushbuttons are simultaneously pressed.
void buttons_runTest()
{

   // This will init the GPIO hardware so you can write to the 4 LEDs  (LD3 - LD0) on the ZYBO board.
   // if printFailedStatusFlag = 1, it will print out an error message if an internal error occurs.
   buttons_init();

  //These Boolean values will act as flags to make sure that the rectangle maintain their state if they are
 // already in place. They will be set to false if the BTN is already in place.
   bool btn0_pushed = true;
   bool btn1_pushed = true;
   bool btn2_pushed = true;
   bool btn3_pushed = true;

   display_init();
   //Start the screen out as BLACK
   display_fillScreen(DISPLAY_BLACK);
  //read the buttons at first and see what the value is. Assign it to variable ‘buttons_value’
   int32_t buttons_value = buttons_read();
   //Until all buttons are placed at the same time, the program will continue to run.
   while (buttons_value != BUTTONS_GPIO_END_VALUE)
   {
       //BTN0
             /* displays a rectangle on the display screen that will be 120 X 80 pixels.
* BTN0 will be of color yellow and will have a black text that says BTN0 on it.*
* displays when the button is pushed.
* Each button has a button mask with which it is anded to isolate that buttons position.
*/
       if ((buttons_value & BUTTONS_BTN0_MASK) && btn0_pushed) //if the button has already been pushed
       {
	//Display the box
           display_fillRect(BUTTONS_DISPLAY_RECT_X_BTN0,
                                   BUTTONS_DISPLAY_UPPER_LIMIT,
                                   BUTTONS_DISPLAY_HEIGHT,
                                   BUTTONS_DISPLAY_WIDTH,
                                   DISPLAY_YELLOW);
           display_setCursor(BUTTONS_DISPLAY_CURSOR_X_BTN0,
                                       BUTTONS_DISPLAY_CURSOR_UPPER_BNDS);
           display_setTextColor(DISPLAY_BLACK);
           display_setTextSize(BUTTONS_DISPLAY_TEXT_SIZE);
           display_print(PRINT_BTN0);
           btn0_pushed = false;

       }
       else if (!((buttons_value & BUTTONS_BTN0_MASK))) //if the button has already been pushed
       {
           display_fillRect(BUTTONS_DISPLAY_RECT_X_BTN0,
                                   BUTTONS_DISPLAY_UPPER_LIMIT,
                                   BUTTONS_DISPLAY_HEIGHT,
                                   BUTTONS_DISPLAY_WIDTH,
                                   DISPLAY_BLACK);
           btn0_pushed = true;
       }

       //BTN1
/* displays a rectangle on the display screen that will be 120 X 80 pixels.
* BTN0 will be of color yellow and will have a black text that says BTN0 on it.*
* displays when the button is pushed.
* Each button has a button mask with which it is anded to isolate that buttons position.
*/

       if ((buttons_value & BUTTONS_BTN1_MASK) && btn1_pushed)
        {
                  display_fillRect(BUTTONS_DISPLAY_RECT_X_BTN1,
                                          BUTTONS_DISPLAY_UPPER_LIMIT,
                                          BUTTONS_DISPLAY_HEIGHT,
                                          BUTTONS_DISPLAY_WIDTH,
                                          DISPLAY_GREEN);
                  display_setCursor(BUTTONS_DISPLAY_CURSOR_X_BTN1,
                                              BUTTONS_DISPLAY_CURSOR_UPPER_BNDS);
                  display_setTextColor(DISPLAY_BLACK);
                  display_setTextSize(BUTTONS_DISPLAY_TEXT_SIZE);
                  display_print(PRINT_BTN1);
                  btn1_pushed = false;
       }
       else if (!((buttons_value & BUTTONS_BTN1_MASK))) //if the button has already been pushed
       {
          display_fillRect(BUTTONS_DISPLAY_RECT_X_BTN1,
                                  BUTTONS_DISPLAY_UPPER_LIMIT,
                                  BUTTONS_DISPLAY_WIDTH,
                                  BUTTONS_DISPLAY_WIDTH,
                                  DISPLAY_BLACK);
          btn1_pushed = true;
       }

       //BTN 2
/* displays a rectangle on the display screen that will be 120 X 80 pixels.
* BTN0 will be of color yellow and will have a black text that says BTN0 on it.*
* displays when the button is pushed.
* Each button has a button mask with which it is anded to isolate that buttons position.
*/

       if ((buttons_value & BUTTONS_BTN2_MASK) && btn2_pushed)
       {
                  display_fillRect(BUTTONS_DISPLAY_HEIGHT,
                                          BUTTONS_DISPLAY_UPPER_LIMIT,
                                          BUTTONS_DISPLAY_HEIGHT,
                                          BUTTONS_DISPLAY_WIDTH,
                                          DISPLAY_RED);
                  display_setCursor(BUTTONS_DISPLAY_CURSOR_X_BTN2,
                                              BUTTONS_DISPLAY_CURSOR_UPPER_BNDS);
                  display_setTextColor(DISPLAY_WHITE);
                  display_setTextSize(BUTTONS_DISPLAY_TEXT_SIZE);
                  display_print(PRINT_BTN2);
                  btn2_pushed = false;
       }
       else if (!((buttons_value & BUTTONS_BTN2_MASK))) //if the button has already been pushed
       {
          display_fillRect(BUTTONS_DISPLAY_HEIGHT,
                                  BUTTONS_DISPLAY_UPPER_LIMIT,
                                  BUTTONS_DISPLAY_HEIGHT,
                                  BUTTONS_DISPLAY_WIDTH,
                                  DISPLAY_BLACK);
          btn2_pushed = true;
       }

       //BTN3
/* displays a rectangle on the display screen that will be 120 X 80 pixels.
* BTN3 will be of color BLUE and will have a WHITE text that says BTN3 on it.*
* displays when the button is pushed.
* Each button has a button mask with which it is anded to isolate that buttons position.
*/
       if ((buttons_value & BUTTONS_BTN3_MASK) && btn3_pushed)
       {
display_fillRect(BUTTONS_DISPLAY_UPPER_LIMIT,
                        BUTTONS_DISPLAY_UPPER_LIMIT,
                        BUTTONS_DISPLAY_HEIGHT,
                        BUTTONS_DISPLAY_WIDTH,
                        DISPLAY_BLUE);
             display_setCursor(BUTTONS_DISPLAY_CURSOR_X_BTN3,
                                        BUTTONS_DISPLAY_CURSOR_UPPER_BNDS);
          display_setTextColor(DISPLAY_WHITE);
          display_setTextSize(BUTTONS_DISPLAY_TEXT_SIZE);
          display_print(PRINT_BTN3);
          btn3_pushed = false;

       }
       else if (!((buttons_value & BUTTONS_BTN3_MASK))) //if the button has already been pushed
       {
          display_fillRect(BUTTONS_DISPLAY_UPPER_LIMIT,
                                  BUTTONS_DISPLAY_UPPER_LIMIT,
                                  BUTTONS_DISPLAY_HEIGHT,
                                  BUTTONS_DISPLAY_WIDTH,
                                  DISPLAY_BLACK);
          btn3_pushed = true;
       }
        //At the end of the loop we read the register again to check and see what the updated state is and
        //assign it to button_value
       buttons_value = buttons_read();

   }//END LOOP
 // When loop is ended display a black screen
   display_fillScreen(DISPLAY_BLACK);

}
