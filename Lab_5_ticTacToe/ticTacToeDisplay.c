#include <stdint.h>
#include <stdbool.h>
#include "ticTacToeDisplay.h"
#include "supportFiles/display.h"
#include "supportFiles/utils.h"
#include "../Lab_2/switches.h"
#include "../Lab_2/buttons.h"
  
//Define the board parameters and edges
#define SCREEN_LEFT 0 
#define SCREEN_TOP 0
#define SCREEN_RIGHT DISPLAY_WIDTH
#define SCREEN_BOTTOM DISPLAY_HEIGHT

//Board lines 
#define ROW_LINE_ONE (DISPLAY_HEIGHT/3)
#define ROW_LINE_TWO ((2 * DISPLAY_HEIGHT)/ 3)
#define COLUMN_LINE_ONE (DISPLAY_WIDTH/3)
#define COLUMN_LINE_TWO ((2 * DISPLAY_WIDTH) / 3)

//COLUMNS AND ROWS
#define ROW_ZERO 0
#define ROW_ONE 1
#define ROW_TWO 2
#define COLUMN_ZERO 0
#define COLUMN_ONE 1
#define COLUMN_TWO 2

#define ALL_ONES 0xF

#define BUTTON_MASK 0x0000
#define ZERO 0

/* 
		 0 | 1 | 2
	    ___|___|___ ROW 0
	     3 | 4 | 5
		___|___|___ ROW 1
		 6 | 7 | 8
	       |   |    ROW 2
		COL COL COL
         0   1   2		   
	*/
//O's centered in each of the squares
#define RADIUS_CIRCLE (COLUMN_LINE_ONE / 2)/2
#define SQUARE_ZERO_X (COLUMN_LINE_ONE / 2)
#define SQUARE_ZERO_Y (ROW_LINE_ONE / 2)
//square 1
#define SQUARE_ONE_X (COLUMN_LINE_TWO - SQUARE_ZERO_X)
#define SQUARE_ONE_Y SQUARE_ZERO_Y
//square 2
#define SQUARE_TWO_X (SCREEN_RIGHT - SQUARE_ZERO_X)
#define SQUARE_TWO_Y SQUARE_ZERO_Y
//square 3
#define SQUARE_THREE_X SQUARE_ZERO_X
#define SQUARE_THREE_Y (ROW_LINE_TWO - SQUARE_ZERO_Y)
//square 4
#define SQUARE_FOUR_X SQUARE_ONE_X 
#define SQUARE_FOUR_Y SQUARE_THREE_Y
//square 5
#define SQUARE_FIVE_X SQUARE_TWO_X
#define SQUARE_FIVE_Y SQUARE_THREE_Y
//square 6
#define SQUARE_SIX_X SQUARE_ZERO_X
#define SQUARE_SIX_Y (SCREEN_BOTTOM - SQUARE_ZERO_Y)
//square 7
#define SQUARE_SEVEN_X SQUARE_ONE_X
#define SQUARE_SEVEN_Y SQUARE_SIX_Y
//square 8
#define SQUARE_EIGHT_X SQUARE_TWO_X
#define SQUARE_EIGHT_Y SQUARE_SIX_Y

 //X's centered in each of the squares
#define DISTANCE (SQUARE_ZERO_X / 2)// This is the distance from the center that the x's coordinates are away from the center


/*
 * The following syntax follows this pattern
 * if it the coordinate of X it starts with X
 * it then describes the position within the square where the coordinate is located
 * it then describes the Y or X coordinate
 * it then says which square we are taking.
 * X - TOP_RIGHT - X - ZERO
 */

//SQUARE 0
#define X_TOP_RIGHT_X_ZERO SQUARE_ZERO_X + DISTANCE
#define X_TOP_RIGHT_Y_ZERO SQUARE_ZERO_Y - DISTANCE
#define X_TOP_LEFT_X_ZERO SQUARE_ZERO_X - DISTANCE
#define X_TOP_LEFT_Y_ZERO SQUARE_ZERO_Y - DISTANCE
#define X_BOTTOM_LEFT_X_ZERO SQUARE_ZERO_X - DISTANCE
#define X_BOTTOM_LEFT_Y_ZERO SQUARE_ZERO_Y + DISTANCE
#define X_BOTTOM_RIGHT_X_ZERO SQUARE_ZERO_X + DISTANCE
#define X_BOTTOM_RIGHT_Y_ZERO SQUARE_ZERO_Y + DISTANCE
//SQUARE 1
#define X_TOP_RIGHT_X_ONE SQUARE_ONE_X + DISTANCE
#define X_TOP_RIGHT_Y_ONE SQUARE_ONE_Y - DISTANCE
#define X_TOP_LEFT_X_ONE SQUARE_ONE_X - DISTANCE
#define X_TOP_LEFT_Y_ONE SQUARE_ONE_Y - DISTANCE
#define X_BOTTOM_LEFT_X_ONE SQUARE_ONE_X - DISTANCE
#define X_BOTTOM_LEFT_Y_ONE SQUARE_ONE_Y + DISTANCE
#define X_BOTTOM_RIGHT_X_ONE SQUARE_ONE_X + DISTANCE
#define X_BOTTOM_RIGHT_Y_ONE SQUARE_ONE_Y + DISTANCE
//SQUARE 2
#define X_TOP_RIGHT_X_TWO SQUARE_TWO_X + DISTANCE
#define X_TOP_RIGHT_Y_TWO SQUARE_TWO_Y - DISTANCE
#define X_TOP_LEFT_X_TWO SQUARE_TWO_X - DISTANCE
#define X_TOP_LEFT_Y_TWO SQUARE_TWO_Y - DISTANCE
#define X_BOTTOM_LEFT_X_TWO SQUARE_TWO_X - DISTANCE
#define X_BOTTOM_LEFT_Y_TWO SQUARE_TWO_Y + DISTANCE
#define X_BOTTOM_RIGHT_X_TWO SQUARE_TWO_X + DISTANCE
#define X_BOTTOM_RIGHT_Y_TWO SQUARE_TWO_Y + DISTANCE
//SQUARE 3
#define X_TOP_RIGHT_X_THREE SQUARE_THREE_X + DISTANCE
#define X_TOP_RIGHT_Y_THREE SQUARE_THREE_Y - DISTANCE
#define X_TOP_LEFT_X_THREE SQUARE_THREE_X - DISTANCE
#define X_TOP_LEFT_Y_THREE SQUARE_THREE_Y - DISTANCE
#define X_BOTTOM_LEFT_X_THREE SQUARE_THREE_X - DISTANCE
#define X_BOTTOM_LEFT_Y_THREE SQUARE_THREE_Y + DISTANCE
#define X_BOTTOM_RIGHT_X_THREE SQUARE_THREE_X + DISTANCE
#define X_BOTTOM_RIGHT_Y_THREE SQUARE_THREE_Y + DISTANCE
//SQUARE 4
#define X_TOP_RIGHT_X_FOUR SQUARE_FOUR_X + DISTANCE
#define X_TOP_RIGHT_Y_FOUR SQUARE_FOUR_Y - DISTANCE
#define X_TOP_LEFT_X_FOUR SQUARE_FOUR_X - DISTANCE
#define X_TOP_LEFT_Y_FOUR SQUARE_FOUR_Y - DISTANCE
#define X_BOTTOM_LEFT_X_FOUR SQUARE_FOUR_X - DISTANCE
#define X_BOTTOM_LEFT_Y_FOUR SQUARE_FOUR_Y + DISTANCE
#define X_BOTTOM_RIGHT_X_FOUR SQUARE_FOUR_X + DISTANCE
#define X_BOTTOM_RIGHT_Y_FOUR SQUARE_FOUR_Y + DISTANCE
//SQUARE 5
#define X_TOP_RIGHT_X_FIVE SQUARE_FIVE_X + DISTANCE
#define X_TOP_RIGHT_Y_FIVE SQUARE_FIVE_Y - DISTANCE
#define X_TOP_LEFT_X_FIVE SQUARE_FIVE_X - DISTANCE
#define X_TOP_LEFT_Y_FIVE SQUARE_FIVE_Y - DISTANCE
#define X_BOTTOM_LEFT_X_FIVE SQUARE_FIVE_X - DISTANCE
#define X_BOTTOM_LEFT_Y_FIVE SQUARE_FIVE_Y + DISTANCE
#define X_BOTTOM_RIGHT_X_FIVE SQUARE_FIVE_X + DISTANCE
#define X_BOTTOM_RIGHT_Y_FIVE SQUARE_FIVE_Y + DISTANCE
//SQUARE 6
#define X_TOP_RIGHT_X_SIX SQUARE_SIX_X + DISTANCE
#define X_TOP_RIGHT_Y_SIX SQUARE_SIX_Y - DISTANCE
#define X_TOP_LEFT_X_SIX SQUARE_SIX_X - DISTANCE
#define X_TOP_LEFT_Y_SIX SQUARE_SIX_Y - DISTANCE
#define X_BOTTOM_LEFT_X_SIX SQUARE_SIX_X - DISTANCE
#define X_BOTTOM_LEFT_Y_SIX SQUARE_SIX_Y + DISTANCE
#define X_BOTTOM_RIGHT_X_SIX SQUARE_SIX_X + DISTANCE
#define X_BOTTOM_RIGHT_Y_SIX SQUARE_SIX_Y + DISTANCE
//SQUARE 7
#define X_TOP_RIGHT_X_SEVEN SQUARE_SEVEN_X + DISTANCE
#define X_TOP_RIGHT_Y_SEVEN SQUARE_SEVEN_Y - DISTANCE
#define X_TOP_LEFT_X_SEVEN SQUARE_SEVEN_X - DISTANCE
#define X_TOP_LEFT_Y_SEVEN SQUARE_SEVEN_Y - DISTANCE
#define X_BOTTOM_LEFT_X_SEVEN SQUARE_SEVEN_X - DISTANCE
#define X_BOTTOM_LEFT_Y_SEVEN SQUARE_SEVEN_Y + DISTANCE
#define X_BOTTOM_RIGHT_X_SEVEN SQUARE_SEVEN_X + DISTANCE
#define X_BOTTOM_RIGHT_Y_SEVEN SQUARE_SEVEN_Y + DISTANCE
//SQUARE 8
#define X_TOP_RIGHT_X_EIGHT SQUARE_EIGHT_X + DISTANCE
#define X_TOP_RIGHT_Y_EIGHT SQUARE_EIGHT_Y - DISTANCE
#define X_TOP_LEFT_X_EIGHT SQUARE_EIGHT_X - DISTANCE
#define X_TOP_LEFT_Y_EIGHT SQUARE_EIGHT_Y - DISTANCE
#define X_BOTTOM_LEFT_X_EIGHT SQUARE_EIGHT_X - DISTANCE
#define X_BOTTOM_LEFT_Y_EIGHT SQUARE_EIGHT_Y + DISTANCE
#define X_BOTTOM_RIGHT_X_EIGHT SQUARE_EIGHT_X + DISTANCE
#define X_BOTTOM_RIGHT_Y_EIGHT SQUARE_EIGHT_Y + DISTANCE
//Time that the clock will delay between data samples.
#define TIME_DELAY 50



// Inits the tic-tac-toe display, draws the lines that form the board.
void ticTacToeDisplay_init()
{
	display_init();//initialize the display with all standard 
	display_fillScreen(DISPLAY_BLACK);// Start the screen out as black
	//draw the lines on the screen
	//ticTacToeDisplay_drawBoardLines();
	
}
 
// Draws an X at the specified row and column.
// erase == true means to erase the X by redrawing it as background. erase == false, draw the X as foreground.
void ticTacToeDisplay_drawX(uint8_t row, uint8_t column, bool erase)
{   /*
         0 | 1 | 2
        ___|___|___ ROW 0
         3 | 4 | 5
        ___|___|___ ROW 1
         6 | 7 | 8
           |   |    ROW 2
        COL COL COL
         0   1   2
       ** use this diagram to reference which square is being drawn to

    */
    if(erase)
	{ 
		if(row == ROW_ZERO && column == COLUMN_ZERO)//square zero 
		{
			display_drawLine(X_TOP_RIGHT_X_ZERO, X_TOP_RIGHT_Y_ZERO, X_BOTTOM_LEFT_X_ZERO, X_BOTTOM_LEFT_Y_ZERO, DISPLAY_BLACK);// X @ 0,0
			display_drawLine(X_TOP_LEFT_X_ZERO, X_TOP_LEFT_Y_ZERO, X_BOTTOM_RIGHT_X_ZERO, X_BOTTOM_RIGHT_Y_ZERO, DISPLAY_BLACK);// X @ 0,0
			
		}else if(row == ROW_ZERO && column == COLUMN_ONE)//square 1
		{
			display_drawLine(X_TOP_RIGHT_X_ONE, X_TOP_RIGHT_Y_ONE, X_BOTTOM_LEFT_X_ONE, X_BOTTOM_LEFT_Y_ONE, DISPLAY_BLACK);// X @ 0,1
			display_drawLine(X_TOP_LEFT_X_ONE, X_TOP_LEFT_Y_ONE, X_BOTTOM_RIGHT_X_ONE, X_BOTTOM_RIGHT_Y_ONE, DISPLAY_BLACK);// X @ 0,1
			
		}else if(row == ROW_ZERO && column == COLUMN_TWO)//square 2
		{
			display_drawLine(X_TOP_RIGHT_X_TWO, X_TOP_RIGHT_Y_TWO, X_BOTTOM_LEFT_X_TWO, X_BOTTOM_LEFT_Y_TWO, DISPLAY_BLACK);// X @ 0,2
			display_drawLine(X_TOP_LEFT_X_TWO, X_TOP_LEFT_Y_TWO, X_BOTTOM_RIGHT_X_TWO, X_BOTTOM_RIGHT_Y_TWO, DISPLAY_BLACK);// X @ 0,2
			
		}else if(row == ROW_ONE && column == COLUMN_ZERO)//square 3
		{
			display_drawLine(X_TOP_RIGHT_X_THREE, X_TOP_RIGHT_Y_THREE, X_BOTTOM_LEFT_X_THREE, X_BOTTOM_LEFT_Y_THREE, DISPLAY_BLACK);// X @ 1,0
			display_drawLine(X_TOP_LEFT_X_THREE, X_TOP_LEFT_Y_THREE, X_BOTTOM_RIGHT_X_THREE, X_BOTTOM_RIGHT_Y_THREE, DISPLAY_BLACK);// X @ 1,0
			
		}else if(row == ROW_ONE && column == COLUMN_ONE)//square 4
		{
			display_drawLine(X_TOP_RIGHT_X_FOUR, X_TOP_RIGHT_Y_FOUR, X_BOTTOM_LEFT_X_FOUR, X_BOTTOM_LEFT_Y_FOUR, DISPLAY_BLACK);// X @ 1,1
			display_drawLine(X_TOP_LEFT_X_FOUR, X_TOP_LEFT_Y_FOUR, X_BOTTOM_RIGHT_X_FOUR, X_BOTTOM_RIGHT_Y_FOUR, DISPLAY_BLACK);// X @ 1,1
			
		}else if(row == ROW_ONE && column == COLUMN_TWO)//square 5
		{
			display_drawLine(X_TOP_RIGHT_X_FIVE, X_TOP_RIGHT_Y_FIVE, X_BOTTOM_LEFT_X_FIVE, X_BOTTOM_LEFT_Y_FIVE, DISPLAY_BLACK);// X @ 1,2
			display_drawLine(X_TOP_LEFT_X_FIVE, X_TOP_LEFT_Y_FIVE, X_BOTTOM_RIGHT_X_FIVE, X_BOTTOM_RIGHT_Y_FIVE, DISPLAY_BLACK);// X @ 1,2
			
		}else if(row == ROW_TWO && column == COLUMN_ZERO)//square 6
		{
			display_drawLine(X_TOP_RIGHT_X_SIX, X_TOP_RIGHT_Y_SIX, X_BOTTOM_LEFT_X_SIX, X_BOTTOM_LEFT_Y_SIX, DISPLAY_BLACK);// X @ 2,0
			display_drawLine(X_TOP_LEFT_X_SIX, X_TOP_LEFT_Y_SIX, X_BOTTOM_RIGHT_X_SIX, X_BOTTOM_RIGHT_Y_SIX, DISPLAY_BLACK);// X @ 2,0
			
		}else if(row == ROW_TWO && column == COLUMN_ONE)//square 7
		{
			display_drawLine(X_TOP_RIGHT_X_SEVEN, X_TOP_RIGHT_Y_SEVEN, X_BOTTOM_LEFT_X_SEVEN, X_BOTTOM_LEFT_Y_SEVEN, DISPLAY_BLACK);// X @ 2,1
			display_drawLine(X_TOP_LEFT_X_SEVEN, X_TOP_LEFT_Y_SEVEN, X_BOTTOM_RIGHT_X_SEVEN, X_BOTTOM_RIGHT_Y_SEVEN, DISPLAY_BLACK);// X @ 2,1
			
		}else if(row == ROW_TWO && column == COLUMN_TWO)//square 8
		{
			display_drawLine(X_TOP_RIGHT_X_EIGHT, X_TOP_RIGHT_Y_EIGHT, X_BOTTOM_LEFT_X_EIGHT, X_BOTTOM_LEFT_Y_EIGHT, DISPLAY_BLACK);// X @ 2,2
			display_drawLine(X_TOP_LEFT_X_EIGHT, X_TOP_LEFT_Y_EIGHT, X_BOTTOM_RIGHT_X_EIGHT, X_BOTTOM_RIGHT_Y_EIGHT, DISPLAY_BLACK);// X @ 2,2
			
		}
	}	
	else if(!erase)
	{
	    /*
	         0 | 1 | 2
	        ___|___|___ ROW 0
	         3 | 4 | 5
	        ___|___|___ ROW 1
	         6 | 7 | 8
	           |   |    ROW 2
	        COL COL COL
	         0   1   2
	       ** use this diagram to reference which square is being drawn to

	    */
		if(row == ROW_ZERO && column == COLUMN_ZERO)//square zero 
		{
			display_drawLine(X_TOP_RIGHT_X_ZERO, X_TOP_RIGHT_Y_ZERO, X_BOTTOM_LEFT_X_ZERO, X_BOTTOM_LEFT_Y_ZERO, DISPLAY_YELLOW);// O @ 0,0
			display_drawLine(X_TOP_LEFT_X_ZERO, X_TOP_LEFT_Y_ZERO, X_BOTTOM_RIGHT_X_ZERO, X_BOTTOM_RIGHT_Y_ZERO, DISPLAY_YELLOW);// O @ 0,0
			
		}else if(row == ROW_ZERO && column == COLUMN_ONE)//square 1
		{
			display_drawLine(X_TOP_RIGHT_X_ONE, X_TOP_RIGHT_Y_ONE, X_BOTTOM_LEFT_X_ONE, X_BOTTOM_LEFT_Y_ONE, DISPLAY_YELLOW);// O @ 0,1
			display_drawLine(X_TOP_LEFT_X_ONE, X_TOP_LEFT_Y_ONE, X_BOTTOM_RIGHT_X_ONE, X_BOTTOM_RIGHT_Y_ONE, DISPLAY_YELLOW);// O @ 0,1
			
		}else if(row == ROW_ZERO && column == COLUMN_TWO)//square 2
		{
			display_drawLine(X_TOP_RIGHT_X_TWO, X_TOP_RIGHT_Y_TWO, X_BOTTOM_LEFT_X_TWO, X_BOTTOM_LEFT_Y_TWO, DISPLAY_YELLOW);// O @ 0,2
			display_drawLine(X_TOP_LEFT_X_TWO, X_TOP_LEFT_Y_TWO, X_BOTTOM_RIGHT_X_TWO, X_BOTTOM_RIGHT_Y_TWO, DISPLAY_YELLOW);// O @ 0,2
		}else if(row == ROW_ONE && column == COLUMN_ZERO)//square 3
		{
			display_drawLine(X_TOP_RIGHT_X_THREE, X_TOP_RIGHT_Y_THREE, X_BOTTOM_LEFT_X_THREE, X_BOTTOM_LEFT_Y_THREE, DISPLAY_YELLOW);// O @ 1,0
			display_drawLine(X_TOP_LEFT_X_THREE, X_TOP_LEFT_Y_THREE, X_BOTTOM_RIGHT_X_THREE, X_BOTTOM_RIGHT_Y_THREE, DISPLAY_YELLOW);// O @ 1,0
			
		}else if(row == ROW_ONE && column == COLUMN_ONE)//square 4
		{
			display_drawLine(X_TOP_RIGHT_X_FOUR, X_TOP_RIGHT_Y_FOUR, X_BOTTOM_LEFT_X_FOUR, X_BOTTOM_LEFT_Y_FOUR, DISPLAY_YELLOW);// O @ 1,1
			display_drawLine(X_TOP_LEFT_X_FOUR, X_TOP_LEFT_Y_FOUR, X_BOTTOM_RIGHT_X_FOUR, X_BOTTOM_RIGHT_Y_FOUR, DISPLAY_YELLOW);// O @ 1,1
			
		}else if(row == ROW_ONE && column == COLUMN_TWO)//square 5
		{
			display_drawLine(X_TOP_RIGHT_X_FIVE, X_TOP_RIGHT_Y_FIVE, X_BOTTOM_LEFT_X_FIVE, X_BOTTOM_LEFT_Y_FIVE, DISPLAY_YELLOW);// O @ 1,2
			display_drawLine(X_TOP_LEFT_X_FIVE, X_TOP_LEFT_Y_FIVE, X_BOTTOM_RIGHT_X_FIVE, X_BOTTOM_RIGHT_Y_FIVE, DISPLAY_YELLOW);// O @ 1,2
			
		}else if(row == ROW_TWO && column == COLUMN_ZERO)//square 6
		{
			display_drawLine(X_TOP_RIGHT_X_SIX, X_TOP_RIGHT_Y_SIX, X_BOTTOM_LEFT_X_SIX, X_BOTTOM_LEFT_Y_SIX, DISPLAY_YELLOW);// O @ 2,0
			display_drawLine(X_TOP_LEFT_X_SIX, X_TOP_LEFT_Y_SIX, X_BOTTOM_RIGHT_X_SIX, X_BOTTOM_RIGHT_Y_SIX, DISPLAY_YELLOW);// O @ 2,0
			
		}else if(row == ROW_TWO && column == COLUMN_ONE)//square 7
		{
			display_drawLine(X_TOP_RIGHT_X_SEVEN, X_TOP_RIGHT_Y_SEVEN, X_BOTTOM_LEFT_X_SEVEN, X_BOTTOM_LEFT_Y_SEVEN, DISPLAY_YELLOW);// O @ 0,0
			display_drawLine(X_TOP_LEFT_X_SEVEN, X_TOP_LEFT_Y_SEVEN, X_BOTTOM_RIGHT_X_SEVEN, X_BOTTOM_RIGHT_Y_SEVEN, DISPLAY_YELLOW);// O @ 0,0
			
		}else if(row == ROW_TWO && column == COLUMN_TWO)//square 8
		{
			display_drawLine(X_TOP_RIGHT_X_EIGHT, X_TOP_RIGHT_Y_EIGHT, X_BOTTOM_LEFT_X_EIGHT, X_BOTTOM_LEFT_Y_EIGHT, DISPLAY_YELLOW);// O @ 0,0
			display_drawLine(X_TOP_LEFT_X_EIGHT, X_TOP_LEFT_Y_EIGHT, X_BOTTOM_RIGHT_X_EIGHT, X_BOTTOM_RIGHT_Y_EIGHT, DISPLAY_YELLOW);// O @ 0,0
			
		}
	}
}
 
// Draws an O at the specified row and column.
// erase == true means to erase the X by redrawing it as background. erase == false, draw the X as foreground.
void ticTacToeDisplay_drawO(uint8_t row, uint8_t column, bool erase)
{
	/* 
		 0 | 1 | 2
	    ___|___|___ ROW 0
	     3 | 4 | 5
		___|___|___ ROW 1
		 6 | 7 | 8
	       |   |    ROW 2
		COL COL COL
         0   1   2		   
	   ** use this diagram to reference which square is being drawn to

	*/
	if(erase)
	{ 
		if(row == ROW_ZERO && column == COLUMN_ZERO)//**square 0
		{
			display_drawCircle(SQUARE_ZERO_X,SQUARE_ZERO_Y, RADIUS_CIRCLE, DISPLAY_BLACK); 
		}else if(row == ROW_ZERO && column == COLUMN_ONE)//**square 1
		{
			display_drawCircle(SQUARE_ONE_X,SQUARE_ONE_Y, RADIUS_CIRCLE, DISPLAY_BLACK);
		}else if(row == ROW_ZERO && column == COLUMN_TWO)//**square 2
		{
			display_drawCircle(SQUARE_TWO_X,SQUARE_TWO_Y, RADIUS_CIRCLE, DISPLAY_BLACK);
		}else if(row == ROW_ONE && column == COLUMN_ZERO)//**square 3
		{
			display_drawCircle(SQUARE_THREE_X,SQUARE_THREE_Y, RADIUS_CIRCLE, DISPLAY_BLACK);
		}else if(row == ROW_ONE && column == COLUMN_ONE)//**square 4
		{
			display_drawCircle(SQUARE_FOUR_X,SQUARE_FOUR_Y, RADIUS_CIRCLE, DISPLAY_BLACK);
		}else if(row == ROW_ONE && column == COLUMN_TWO)//**square 5
		{
			display_drawCircle(SQUARE_FIVE_X,SQUARE_FIVE_Y, RADIUS_CIRCLE, DISPLAY_BLACK);
		}else if(row == ROW_TWO && column == COLUMN_ZERO)//**square 6
		{
			display_drawCircle(SQUARE_SIX_X,SQUARE_SIX_Y, RADIUS_CIRCLE, DISPLAY_BLACK);
		}else if(row == ROW_TWO && column == COLUMN_ONE)//**square 7
		{
			display_drawCircle(SQUARE_SEVEN_X,SQUARE_SEVEN_Y, RADIUS_CIRCLE, DISPLAY_BLACK);
		}else if(row == ROW_TWO && column == COLUMN_TWO)//**square 8
		{
			display_drawCircle(SQUARE_EIGHT_X,SQUARE_EIGHT_Y, RADIUS_CIRCLE, DISPLAY_BLACK);
		}
	}	
	else if(!erase)
	{
		if(row == ROW_ZERO && column == COLUMN_ZERO) 
		{
			display_drawCircle(SQUARE_ZERO_X,SQUARE_ZERO_Y, RADIUS_CIRCLE, DISPLAY_YELLOW); 
		}else if(row == ROW_ZERO && column == COLUMN_ONE)//**square 1
		{
			display_drawCircle(SQUARE_ONE_X,SQUARE_ONE_Y, RADIUS_CIRCLE, DISPLAY_YELLOW);
		}else if(row == ROW_ZERO && column == COLUMN_TWO)//**square 2
		{
			display_drawCircle(SQUARE_TWO_X,SQUARE_TWO_Y, RADIUS_CIRCLE, DISPLAY_YELLOW);
		}else if(row == ROW_ONE && column == COLUMN_ZERO)//**square 3
		{
			display_drawCircle(SQUARE_THREE_X,SQUARE_THREE_Y, RADIUS_CIRCLE, DISPLAY_YELLOW);
		}else if(row == ROW_ONE && column == COLUMN_ONE)//**square 4
		{
			display_drawCircle(SQUARE_FOUR_X,SQUARE_FOUR_Y, RADIUS_CIRCLE, DISPLAY_YELLOW);
		}else if(row == ROW_ONE && column == COLUMN_TWO)//**square 5
		{
			display_drawCircle(SQUARE_FIVE_X,SQUARE_FIVE_Y, RADIUS_CIRCLE, DISPLAY_YELLOW);
		}else if(row == ROW_TWO && column == COLUMN_ZERO)//**square 6
		{
			display_drawCircle(SQUARE_SIX_X,SQUARE_SIX_Y, RADIUS_CIRCLE, DISPLAY_YELLOW);
		}else if(row == ROW_TWO && column == COLUMN_ONE)//**square 7
		{
			display_drawCircle(SQUARE_SEVEN_X,SQUARE_SEVEN_Y, RADIUS_CIRCLE, DISPLAY_YELLOW);
		}else if(row == ROW_TWO && column == COLUMN_TWO)//**square 8
		{
			display_drawCircle(SQUARE_EIGHT_X,SQUARE_EIGHT_Y, RADIUS_CIRCLE, DISPLAY_YELLOW);
		}
	}
}
 
// After a touch has been detected and after the proper delay, this sets the row and column arguments
// according to where the user touched the board
void ticTacToeDisplay_touchScreenComputeBoardRowColumn(uint8_t* row, uint8_t* column)
{
	int16_t x, y;
	uint8_t z;
    /*
         0 | 1 | 2
        ___|___|___ ROW 0
         3 | 4 | 5
        ___|___|___ ROW 1
         6 | 7 | 8
           |   |    ROW 2
        COL COL COL
         0   1   2
       ** use this diagram to reference which square is being drawn to

    */
	//Check each case and assign the correct value accordingly.

		display_getTouchedPoint(&x, &y, &z);
		// Throws away all previous touch data.
		if(x < COLUMN_LINE_ONE && y < ROW_LINE_ONE) //square 0
		{
			*row = ROW_ZERO;
			*column = COLUMN_ZERO;
		}
		else if(x > COLUMN_LINE_ONE && x < COLUMN_LINE_TWO && y < ROW_LINE_ONE)//square 1
		{
			*row = ROW_ZERO;
			*column = COLUMN_ONE;
		}
		else if(x > COLUMN_LINE_TWO && y < ROW_LINE_ONE)//square 2
		{
			*row = ROW_ZERO;
			*column = COLUMN_TWO;
		}
		else if(x < COLUMN_LINE_ONE && y < ROW_LINE_TWO && y > ROW_LINE_ONE)//square 3
		{
			*row = ROW_ONE;
			*column = COLUMN_ZERO;
		}
		else if(x > COLUMN_LINE_ONE && x < COLUMN_LINE_TWO && y > ROW_LINE_ONE && y < ROW_LINE_TWO)//square 4
		{
			*row = ROW_ONE;
			*column = COLUMN_ONE;
		}
		else if(x > COLUMN_LINE_TWO && y > ROW_LINE_ONE && y < ROW_LINE_TWO)//square 5
		{
			*row = ROW_ONE;
			*column = COLUMN_TWO;
		}
		else if(x < COLUMN_LINE_ONE && y > ROW_LINE_TWO)//square 6
		{
			*row = ROW_TWO;
			*column = COLUMN_ZERO;
		}
		else if(x > COLUMN_LINE_ONE &&  x < COLUMN_LINE_TWO && y > ROW_LINE_TWO)//square 7
		{
			*row = ROW_TWO;
			*column = COLUMN_ONE;
		}
		else if(x > COLUMN_LINE_TWO && y > ROW_LINE_TWO)
		{
			*row = ROW_TWO;
			*column = COLUMN_TWO;
		}

}
/*
#define BUTTONS_BTN0_MASK 0x1
#define BUTTONS_BTN1_MASK 0x2
#define BUTTONS_BTN2_MASK 0x4
#define BUTTONS_BTN3_MASK 0x8
*/
// Runs a test of the display. Does the following.
// Draws the board. Each time you touch one of the screen areas, the screen will paint
// an X or an O, depending on whether switch 0 (SW0) is slid up (O) or down (X).
// When BTN0 is pushed, the screen is cleared. The test terminates when BTN1 is pushed.
void ticTacToeDisplay_runTest()
{
   uint8_t row;
   uint8_t column;

   ticTacToeDisplay_init();//set the screen to black with linees
   while((buttons_read() & BUTTONS_BTN1_MASK) != BUTTONS_BTN1_MASK)
   {
        printf("Done!\n\r");
        if((buttons_read() & BUTTONS_BTN0_MASK) == BUTTONS_BTN0_MASK)
        {
            ticTacToeDisplay_init();
        }
        if(display_isTouched())
        {
            display_clearOldTouchData();
            //SOMEHOW I NEED TO WAIT FOR 50 ms....

            // Returns the x-y coordinate point and the pressure (z).
            utils_msDelay(TIME_DELAY);

            ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column);
            if((switches_read() & BUTTONS_BTN0_MASK) == BUTTONS_BTN0_MASK)
            {
                ticTacToeDisplay_drawO(row, column, false);

            }
            else if((switches_read() & BUTTON_MASK) == ZERO)
            {
                ticTacToeDisplay_drawX(row, column, false);
            }
        }
   }
   while(switches_read() != ALL_ONES)
   {
       printf("Done!\n\r");
   }
}

 
// This will draw the four board lines.
void ticTacToeDisplay_drawBoardLines()
{   /*
         0 | 1 | 2
        ___|___|___ ROW 0
         3 | 4 | 5
        ___|___|___ ROW 1
         6 | 7 | 8
           |   |    ROW 2
        COL COL COL
         0   1   2
       ** use this diagram to reference which square is being drawn to

    */
	//first column 
	display_drawLine(COLUMN_LINE_ONE, SCREEN_TOP, COLUMN_LINE_ONE, SCREEN_BOTTOM, DISPLAY_YELLOW);
	//second column
	display_drawLine(COLUMN_LINE_TWO, SCREEN_TOP, COLUMN_LINE_TWO, SCREEN_BOTTOM, DISPLAY_YELLOW);
	//first row 
	display_drawLine(SCREEN_LEFT, ROW_LINE_ONE, SCREEN_RIGHT, ROW_LINE_ONE, DISPLAY_YELLOW);
	//second row 
	display_drawLine(SCREEN_LEFT, ROW_LINE_TWO, SCREEN_RIGHT, ROW_LINE_TWO, DISPLAY_YELLOW);
}
