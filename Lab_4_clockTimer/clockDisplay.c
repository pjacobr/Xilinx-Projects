

#include <stdio.h>
#include <math.h>
#include "clockDisplay.h"
#include "clockControl.h"
#include "supportFiles/display.h"

 //Equilateral triangle that 
#define HALF .5
#define TWO_THIRDS (2.0 / 3.0)//For screen size
#define ONE_THIRDS (1.0 / 3.0)//for screen positioning
#define THREE_EIGHTH .375

#define DISPLAY_TEXT_SIZE 6//Adjust the code here to adjust the display size
#define CHARACTER_HEIGHT DISPLAY_CHAR_HEIGHT
#define CHARACTAR_WIDTH DISPLAY_CHAR_WIDTH
 
#define DIGITS_SIZE 9
#define NUM_CHARS 4
  // SET THE SCREEN TOUCHED AREAS ******************

//horizontal parameters
#define SCREEN_ONE_THIRD_X (DISPLAY_WIDTH / 3)
#define SCREEN_TWO_THIRD_X ((DISPLAY_WIDTH * 2) / 3)

#define SCREEN_CENTER_X (DISPLAY_WIDTH/2)
#define SCREEN_CENTER_Y (DISPLAY_HEIGHT/2)
//Vertical parameters


// ***********************************************
//set the text width based on text size
 #define TEXT_WIDTH (CHARACTAR_WIDTH*DISPLAY_TEXT_SIZE)
 #define TEXT_HEIGHT (CHARACTER_HEIGHT*DISPLAY_TEXT_SIZE)
//Set the starting position of the cursor for writing the text
 #define CURSOR_POSITION_Y (SCREEN_CENTER_Y - (TEXT_HEIGHT / 2))
 #define TEXT_BEGIN (SCREEN_CENTER_X - (TEXT_WIDTH*NUM_CHARS))

 //These are the starting X positions for the triangles. They will be constant and will not change
 #define FIRST_COLUMN_LEFT TEXT_BEGIN//set the column for writing from here
 #define FIRST_COLUMN_CENTER (TEXT_BEGIN + TEXT_WIDTH) // decalate teh size of one character
 #define FIRST_COLUMN_RIGHT (TEXT_BEGIN + (TEXT_WIDTH * 2))

 //These are the starting X positions for the triangles. They will be constant and will not change
 #define SECOND_COLUMN_LEFT (TEXT_BEGIN + (TEXT_WIDTH * 3))//Second trianges most left coordinate
 #define SECOND_COLUMN_CENTER (TEXT_BEGIN + (TEXT_WIDTH * 4))
 #define SECOND_COLUMN_RIGHT (TEXT_BEGIN + (TEXT_WIDTH * 5))

 //These are the starting X positions for the triangles. They will be constant and will not change
 #define THIRD_COLUMN_LEFT (TEXT_BEGIN + (TEXT_WIDTH * 6))//Third triangle most right coordinate
 #define THIRD_COLUMN_CENTER (TEXT_BEGIN + (TEXT_WIDTH * 7))
 #define THIRD_COLUMN_RIGHT (TEXT_BEGIN + (TEXT_WIDTH * 8))

 //THESE are the positions that determine the digit position
 #define HOURS_POS FIRST_COLUMN_LEFT//Hours digits 1
 #define HOURS_POS_1 FIRST_COLUMN_CENTER//hours digits 2

 #define MINUTES_DIG_POS SECOND_COLUMN_LEFT//Minutes first digit
 #define MINUTES_DIG_POS_1 SECOND_COLUMN_CENTER//minutes second digit
//The position of the seconds digits
 #define SECONDS_DIG_POS THIRD_COLUMN_LEFT
 #define SECONDS_DIG_POS_1 THIRD_COLUMN_CENTER


 //Calculate the triangle height. It is an equilateral triangle so I can use the columns already used
// to figure out the height
 #define TRIANGLE_BASE (TEXT_WIDTH * 2)
 #define TRIANGLE_HEIGHT ((TRIANGLE_BASE / 2) * sqrt(3))

 //TRIANGLE columns
 #define FIRST_ROW_BOTTOM (SCREEN_CENTER_Y - TEXT_HEIGHT)
 #define FIRST_ROW_TOP (FIRST_ROW_BOTTOM - TRIANGLE_HEIGHT)

 #define SECOND_ROW_TOP (SCREEN_CENTER_Y + TEXT_HEIGHT)
 #define SECOND_ROW_BOTTOM (SECOND_ROW_TOP + TRIANGLE_HEIGHT)
 
 /*
  *   TRIANGLE coordinates
  *     1. X1: FIRST_COLUMN_LEFT Y1: FIRST_ROW_BOTTOM
  *         X2: FIRST_COLUMN_CENTER Y2: FIRST_ROW_TOP
  *        X3: FIRST_COLUMN_RIGHT Y3: FIRST_ROW_BOTTOM
  *
  *     2. X1: SECOND_COLUMN_LEFT Y1: FIRST_ROW_BOTTOM
  *        X2: SECOND_COLUMN_CENTER Y2: FIRST_ROW_TOP
  *        X3: SECOND_COLUMN_RIGHT Y3: FIRST_ROW_BOTTOM
  *
  *     3. X1: THIRD_COLUMN_LEFT Y1: FIRST_ROW_BOTTOM
  *        X2: THIRD_COLUMN_CENTER Y2: FIRST_ROW_TOP
  *        X3: THIRD_COLUMN_RIGHT Y3: FIRST_ROW_BOTTOM
  *
  *     4. X1: FIRST_COLUMN_LEFT Y1: SECOND_ROW_TOP
  *        X2: FIRST_COLUMN_CENTER Y2: SECOND_ROW_BOTTOM
  *        X3: FIRST_COLUMN_RIGHT Y3: SECOND_ROW_TOP

  *
  *     6. X1: THIRD_COLUMN_LEFT Y1: SECOND_ROW_TOP
  *        X2: THIRD_COLUMN_CENTER Y2: SECOND_ROW_BOTTOM
  *        //X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
  *
  * */
//TRIANGLE 1
#define UPPER_TRIANGLE_LEFT_X_1 FIRST_COLUMN_LEFT
#define UPPER_TRIANGLE_LEFT_Y_1 FIRST_ROW_BOTTOM
#define UPPER_TRIANGLE_LEFT_X_2 FIRST_COLUMN_CENTER
#define UPPER_TRIANGLE_LEFT_Y_2 FIRST_ROW_TOP//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define UPPER_TRIANGLE_LEFT_X_3 FIRST_COLUMN_RIGHT//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define UPPER_TRIANGLE_LEFT_Y_3 FIRST_ROW_BOTTOM//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
//TRIANGLE 2
#define UPPER_TRIANGLE_CENTER_X_1 SECOND_COLUMN_LEFT
#define UPPER_TRIANGLE_CENTER_Y_1 FIRST_ROW_BOTTOM
#define UPPER_TRIANGLE_CENTER_X_2 SECOND_COLUMN_CENTER
#define UPPER_TRIANGLE_CENTER_Y_2 FIRST_ROW_TOP//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define UPPER_TRIANGLE_CENTER_X_3 SECOND_COLUMN_RIGHT//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define UPPER_TRIANGLE_CENTER_Y_3 FIRST_ROW_BOTTOM
//TRIANGLE 3
#define UPPER_TRIANGLE_RIGHT_X_1 THIRD_COLUMN_LEFT
#define UPPER_TRIANGLE_RIGHT_Y_1 FIRST_ROW_BOTTOM
#define UPPER_TRIANGLE_RIGHT_X_2 THIRD_COLUMN_CENTER//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define UPPER_TRIANGLE_RIGHT_Y_2 FIRST_ROW_TOP//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define UPPER_TRIANGLE_RIGHT_X_3 THIRD_COLUMN_RIGHT//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define UPPER_TRIANGLE_RIGHT_Y_3 FIRST_ROW_BOTTOM
//TRIANGLE 4
#define LOWER_TRIANGLE_LEFT_X_1 FIRST_COLUMN_LEFT
#define LOWER_TRIANGLE_LEFT_Y_1 SECOND_ROW_TOP//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define LOWER_TRIANGLE_LEFT_X_2 FIRST_COLUMN_CENTER//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define LOWER_TRIANGLE_LEFT_Y_2 SECOND_ROW_BOTTOM//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define LOWER_TRIANGLE_LEFT_X_3 FIRST_COLUMN_RIGHT
#define LOWER_TRIANGLE_LEFT_Y_3 SECOND_ROW_TOP
//TRIANGLE 5
#define LOWER_TRIANGLE_CENTER_X_1 SECOND_COLUMN_LEFT
#define LOWER_TRIANGLE_CENTER_Y_1 SECOND_ROW_TOP//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define LOWER_TRIANGLE_CENTER_X_2 SECOND_COLUMN_CENTER//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define LOWER_TRIANGLE_CENTER_Y_2 SECOND_ROW_BOTTOM//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define LOWER_TRIANGLE_CENTER_X_3 SECOND_COLUMN_RIGHT
#define LOWER_TRIANGLE_CENTER_Y_3 SECOND_ROW_TOP
//TRIANGLE 6
#define LOWER_TRIANGLE_RIGHT_X_1 THIRD_COLUMN_LEFT
#define LOWER_TRIANGLE_RIGHT_Y_1 SECOND_ROW_TOP//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define LOWER_TRIANGLE_RIGHT_X_2 THIRD_COLUMN_CENTER//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define LOWER_TRIANGLE_RIGHT_Y_2 SECOND_ROW_BOTTOM//X3: THIRD_COLUMN_RIGHT Y3: SECOND_ROW_TOP
#define LOWER_TRIANGLE_RIGHT_X_3 THIRD_COLUMN_RIGHT
#define LOWER_TRIANGLE_RIGHT_Y_3 SECOND_ROW_TOP
 
/*The position of each of the digits in the array*/
#define HOURS_0 0 //[0]
#define HOURS_1 1 //[1]
#define MINUTES_0 3 //[3]
#define MINUTES_1 4 //[4]
#define SECONDS_0 6 //[6]
#define SECONDS_1 7 //[7]

//This
#define FULL_SECONDS_CYCLE 59
#define FULL_MINUTES_CYCLE 59
#define FULL_HOURS_CYCLE 12


//Here are the max and min values of the different time slots that set and reset the clock
#define HOURS_MAX 12 //The MAX HOURS 
#define INC_ROLL_HOURS 1
#define MINS_MAX 59 //MINS MAX
#define INC_ROLL_MINS 0
#define SECS_MAX 59 //SEC MAX
#define INC_ROLL_SECS 0
#define HOURS_MIN 1 //HOURS MIN
#define DEC_ROLL_HOURS 12 
#define MINS_MIN 0 // MINS MIN
#define DEC_ROLL_MINS 59
#define SECS_MIN 0 // MIN SECS
#define DEC_ROLL_SECS 59


//THESE are global constants
//THE HOURS MINS AND SECONDS
static char digits[DIGITS_SIZE];
static unsigned int hours;
static unsigned int minutes;
static unsigned int seconds;


//Check the updates and rollovers
static bool minutesRolled = false;
static bool secondsRolled = false;
//Check the updates
static bool hoursUpdated = false;
static bool minutesUpdated = false;
static bool secondsUpdated = false;

// Called only once - performs any necessary inits.
// This is a good place to draw the triangles and any other
// parts of the clock display that will never change.
void clockDisplay_init()
{
    hours = HOURS_MIN;
    minutes = INC_ROLL_MINS;
    seconds = INC_ROLL_SECS;

    sprintf(digits, "%2hd:%02hd:%02hd", hours, minutes, seconds);
	//printf("%s", digits);
	display_init();
	display_fillScreen(DISPLAY_BLACK);
	display_setTextColor(DISPLAY_GREEN);
	display_setTextSize(DISPLAY_TEXT_SIZE);
	display_setCursor(TEXT_BEGIN, CURSOR_POSITION_Y);
	display_print(digits);
	//TRIANGLE UP #1	THE X AND Y will be

	display_fillTriangle(UPPER_TRIANGLE_LEFT_X_1, UPPER_TRIANGLE_LEFT_Y_1,UPPER_TRIANGLE_LEFT_X_2, UPPER_TRIANGLE_LEFT_Y_2, UPPER_TRIANGLE_LEFT_X_3, UPPER_TRIANGLE_LEFT_Y_3, DISPLAY_GREEN);
	//Triangle UP #2	THE X will be centered below the CENTER
	display_fillTriangle(UPPER_TRIANGLE_CENTER_X_1, UPPER_TRIANGLE_CENTER_Y_1, UPPER_TRIANGLE_CENTER_X_2, UPPER_TRIANGLE_CENTER_Y_2, UPPER_TRIANGLE_CENTER_X_3, UPPER_TRIANGLE_CENTER_Y_3,DISPLAY_GREEN);
	//Triangle UP #3
	display_fillTriangle(UPPER_TRIANGLE_RIGHT_X_1, UPPER_TRIANGLE_RIGHT_Y_1, UPPER_TRIANGLE_RIGHT_X_2, UPPER_TRIANGLE_RIGHT_Y_2, UPPER_TRIANGLE_RIGHT_X_3, UPPER_TRIANGLE_RIGHT_Y_3, DISPLAY_GREEN);
	//Triangle DOWN #1
	display_fillTriangle(LOWER_TRIANGLE_LEFT_X_1, LOWER_TRIANGLE_LEFT_Y_1, LOWER_TRIANGLE_LEFT_X_2, LOWER_TRIANGLE_LEFT_Y_2, LOWER_TRIANGLE_LEFT_X_3, LOWER_TRIANGLE_LEFT_Y_3,DISPLAY_GREEN);
	    //Triangle DOWN #2    THE X will be centered below the CENTER
	display_fillTriangle(LOWER_TRIANGLE_CENTER_X_1, LOWER_TRIANGLE_CENTER_Y_1,LOWER_TRIANGLE_CENTER_X_2, LOWER_TRIANGLE_CENTER_Y_2,LOWER_TRIANGLE_CENTER_X_3, LOWER_TRIANGLE_CENTER_Y_3, DISPLAY_GREEN);
	    //Triangle DOWN #3
    display_fillTriangle(LOWER_TRIANGLE_RIGHT_X_1, LOWER_TRIANGLE_RIGHT_Y_1, LOWER_TRIANGLE_RIGHT_X_2, LOWER_TRIANGLE_RIGHT_Y_2, LOWER_TRIANGLE_RIGHT_X_3, LOWER_TRIANGLE_RIGHT_Y_3, DISPLAY_GREEN);

	
}  


#define ALL_ONES 0xFFFF

//Done with the  increment
void clockDisplay_incHours()
{
    //increment the hours
    hours++;
    //check to see if the hours have passed outside the hours limits
    if(hours > HOURS_MAX)
    {
        hours = INC_ROLL_HOURS;// the hours rolled over and need to be reset

    }
    hoursUpdated = true; //Updated the hours
}
//DONE,
void clockDisplay_incMins()
{
    minutes++; //increment the minutes
        if(minutes > MINS_MAX)
        {
            minutes = INC_ROLL_MINS; //minutes 
            minutesRolled = true;
        }
        minutesUpdated = true;
}
//DONE, decrement mins
void clockDisplay_incSecs()
{
    //decrement the mins
    seconds++;
    //check to see if the mins go below zero
        if(seconds > SECS_MAX)
        {
            seconds = INC_ROLL_SECS;
            secondsRolled = true;
        }
        //update boolean
        secondsUpdated = true;
}
//DONE, decrement mins
void clockDisplay_decHours()
{
    //decrement the mins
    if(hours == HOURS_MIN)
    {
        hours = HOURS_MAX;
    }
    else
    {
        //decrement the hours
        hours--;
    }
    //update boolean
    hoursUpdated = true;
}
//DONE, decrement mins
void clockDisplay_decMins()
{
    //decrement the mins
        if(minutes == MINS_MIN)
        {
            minutes = MINS_MAX;
        }
        else
        {
            //decrement the hours
            minutes--;
        }
        //update boolean
        minutesUpdated = true;
}
//DONE, decrement mins
void clockDisplay_decSecs()
{
    //decrement the mins
            if(seconds == SECS_MIN)
            {
                seconds = SECS_MAX;
            }
            else
            {
                //decrement the hours
                seconds--;
            }
            //update boolean
            secondsUpdated = true;
}

// Updates the time display with latest time, making sure to update only those digits that
// have changed since the last update.
// if forceUpdateAll is true, update all digits.
void clockDisplay_updateTimeDisplay(bool forceUpdateAll)
{
    //check the time that is actual and update the variables
    sprintf(digits, "%2hd:%02hd:%02hd", hours, minutes, seconds);
    //UPDATES ALL THE DIGITS
    if(forceUpdateAll)
    {
        //UPDATES THE HOURS
        display_drawChar(HOURS_POS, CURSOR_POSITION_Y, digits[HOURS_0], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
        display_drawChar(HOURS_POS_1,CURSOR_POSITION_Y, digits[HOURS_1], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
        //UPDATES THE MINUTES
        display_drawChar(MINUTES_DIG_POS, CURSOR_POSITION_Y, digits[MINUTES_0], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
        display_drawChar(MINUTES_DIG_POS_1, CURSOR_POSITION_Y, digits[MINUTES_1], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
        //UPDATES THE SECONDS
        display_drawChar(SECONDS_DIG_POS, CURSOR_POSITION_Y, digits[SECONDS_0], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
        display_drawChar(SECONDS_DIG_POS_1, CURSOR_POSITION_Y, digits[SECONDS_1], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
    }
    else
    {
        if(hoursUpdated)
        {
            //UPDATES THE HOURS
            display_drawChar(HOURS_POS, CURSOR_POSITION_Y, digits[HOURS_0], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
            display_drawChar(HOURS_POS_1,CURSOR_POSITION_Y, digits[HOURS_1], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
            hoursUpdated = false;
        }
        if(minutesUpdated)
        {
             //UPDATES THE MINUTES
            display_drawChar(MINUTES_DIG_POS, CURSOR_POSITION_Y, digits[MINUTES_0], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
            display_drawChar(MINUTES_DIG_POS_1, CURSOR_POSITION_Y, digits[MINUTES_1], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
            minutesUpdated = false;
        }
        if(secondsUpdated)
         {
             //UPDATES THE SECONDS
             display_drawChar(SECONDS_DIG_POS, CURSOR_POSITION_Y, digits[SECONDS_0], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
             display_drawChar(SECONDS_DIG_POS_1, CURSOR_POSITION_Y, digits[SECONDS_1], DISPLAY_GREEN, DISPLAY_BLACK, DISPLAY_TEXT_SIZE);
             secondsUpdated = false;
         }
    }
}
 
// Reads the touched coordinates and performs the increment or decrement,
// depending upon the touched region.
void clockDisplay_performIncDec()
{
	int16_t x;
	int16_t y;
	uint8_t z;

	display_getTouchedPoint(&x, &y, &z);//Get the touched points!!
	//INC HOURS
	if((x < SCREEN_ONE_THIRD_X) && (y < SCREEN_CENTER_Y))//TOP LEFT CORNER
	{
	    clockDisplay_incHours();//go up the hours
	}//INC MINUTES
	else if ((y < SCREEN_CENTER_Y) && (x > SCREEN_ONE_THIRD_X) && (x < SCREEN_TWO_THIRD_X))//TOP CENTER BOX
	{
	    clockDisplay_incMins();
	}//INC SECONDS
	else if((y < SCREEN_CENTER_Y) && (x > SCREEN_TWO_THIRD_X))//TOP RIGHT CORNER
	{
	    clockDisplay_incSecs();
	}//DEC HOURS
	else if((y > SCREEN_CENTER_Y) && (x < SCREEN_ONE_THIRD_X))//BOTTOM LEFT CORNER
	{
	    clockDisplay_decHours();
	}//DEC MINUTES
	else if((y > SCREEN_CENTER_Y) && (x > SCREEN_ONE_THIRD_X) && (x < SCREEN_TWO_THIRD_X))//BOTTOM MIDDLe CORNER
	{
	    clockDisplay_decMins();
	}//DEC SECONDS
	else if((y > SCREEN_CENTER_Y) && (x > SCREEN_TWO_THIRD_X))//BOTTOM RIGHT CORNER
	{
	    clockDisplay_decSecs();
	}
	else
	{
	    //Do nothing
	}
	 clockDisplay_updateTimeDisplay(false);
	
}
 
// Advances the time forward by 1 second and update the display.
void clockDisplay_advanceTimeOneSecond()
{
    //Start the checks at false
	secondsRolled = false;
	minutesRolled = false;
    //increment the seconds by one
	clockDisplay_incSecs();
	//if the seconds were at 59 and rolled over then it is time to increment the clockDisplay
	if(secondsRolled)
	{
	    clockDisplay_incMins();
	}
	//if the minutes were at 59 and rolled over then it is time to increment the clockDisplay
	if(minutesRolled)
	{
	    clockDisplay_incHours();
	}
	//if the seconds were rolled over and the minutes rolled then it is time to update the clockDisplay
	if(secondsRolled && minutesRolled)
	{
	    clockDisplay_updateTimeDisplay(true);
	}
	else
	{
	    clockDisplay_updateTimeDisplay(false);
	}
}  
 
// Run a test of clock-display functions.
void clockDisplay_runTest()
{
    int i; //This is the test
	for(i = 0; i < (FULL_HOURS_CYCLE * FULL_MINUTES_CYCLE * FULL_SECONDS_CYCLE); i++) //Run for the who amount of time
	{
	    clockDisplay_advanceTimeOneSecond();
 	} // When we are out of the test if is good.
	
}               
