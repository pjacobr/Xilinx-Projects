/*
 * ticTacToeControl.c
 * Created on: Oct 24, 2016
 * Author: dylanps
 *
 */

//include the correct files
#include "ticTacToeControl.h"
//functionality of the board
#include "ticTacToeDisplay.h"
//minimax acces to the algorithm
#include "minimax.h"
#include "../Lab_2/buttons.h"
#include <stdio.h>
#include "supportFiles/display.h"

#define TEXT_SIZE 2
//after 10 seconds the game should start
#define START_MESSAGE_DURATION 15 //waits 10 seconds
#define WAIT_TIMER_DURATION 10 //tells the user if the counter has settled
#define ADC_IS_SETTLED 1 //tells of rows on the board
#define ROW_MAX 3 //number of rows on the board
#define COL_MAX 3 //number of columns on the board
#define RIGHT_BOUND 2 //right edge
#define LEFT_BOUND 0 //left edge
#define RESET 0

#define NOT_MODIFIED -1
//Button mask
#define BUTTONS_0_MASK 0x1
//initializes the main board
minimax_board_t myBoard;
//initializes the copy of the main board
minimax_board_t myCopy;

//these will be place holders
//initialized to a high value because 0 could be an important value
uint8_t temp_row = NOT_MODIFIED;
//initialized to a high value because 0 could be an important value
uint8_t temp_col = NOT_MODIFIED;
bool board_initialized = false;
bool display_initialized = false;
bool isValid = false;
bool firstMove = false;
bool player_moved_last = false;
bool computerIsX = false;
bool playerWon = false;
bool displayMessage = false;

// States for the controller state machine.
enum ticTacToe_st_t {
    init_st,                // Start here, stay in this state for just one tick.
    start_game_st, // Wait here until the first touch - clock is disabled until set.
    waiting_for_touch_st,
    computer_turn_st,
    draw_board_st,
    display_untouched_st,
    adc_settle_st,
    check_score_st,
    player_turn_st,
    end_game_st,
    new_game_st
} currentState = init_st;
//counters
int8_t startmessagecounter = RESET;
int8_t adcsettlecounter = RESET;
int8_t waitTimerCounter = RESET;

//tickCounter
int8_t tickCounter = RESET;
//erase at the end of the game
void eraseXO() {
    //printf("Entered here\n\r");
    uint8_t i;
    uint8_t j;
    for (i = 0; i < ROW_MAX; i++)                 //THIS is the row manipulation
            {
        for (j = 0; j < COL_MAX; j++)          //this is the column manipulation
                {
            if (computerIsX)                 //if the computer went first
            {

                if (myBoard.squares[i][j] == MINIMAX_PLAYER_SQUARE) //erase the x
                {
                    //printf("WOOO\n\r");
                    ticTacToeDisplay_drawO(i, j, true);          //true == ERASE
                } else if (myBoard.squares[i][j] == MINIMAX_OPPONENT_SQUARE) {
                    //printf("WOOO2\n\r");
                    ticTacToeDisplay_drawX(i, j, true);
                }

            } else                 //if the player went first
            {
                if (myBoard.squares[i][j] == MINIMAX_PLAYER_SQUARE) {
                    //printf("WOOO3\n\r");
                    ticTacToeDisplay_drawX(i, j, true);
                } else if (myBoard.squares[i][j] == MINIMAX_OPPONENT_SQUARE) {
                    //printf("WOOO4\n\r");
                    ticTacToeDisplay_drawO(i, j, true);
                }
            }
        }
    }

}
//this functions writes a message to the user
//it notifies the
void writeStartMessage() {
    display_setCursor(LEFT_BOUND, LEFT_BOUND);
    display_setTextSize(TEXT_SIZE);
    display_setTextColor(DISPLAY_BLUE);
    display_println(
            "Touch to play as X\n --------OR-------\nWait to play as O");
}

//this function erases the start message
//it clears it from them
void eraseStartMessage() {
    display_setCursor(LEFT_BOUND, LEFT_BOUND);
    display_setTextSize(TEXT_SIZE);
    display_setTextColor(DISPLAY_BLACK);
    display_println(
            "Touch to play as X\n --------OR-------\nWait to play as O");
}
//initializes the display
void initialize_the_display() {
    if (!display_initialized) {
        ticTacToeDisplay_init();
    }
}

void initialize_Board() {
    if (!board_initialized) {
        //printf("starting the board over\n\r");
        minimax_initBoard(&myBoard);
    }
    board_initialized = true;
}
//check to make sure that the player made a correct choice
void player_choice_isValid() {
    //make sure that the square chosen is empty
    if (myBoard.squares[temp_row][temp_col] != MINIMAX_EMPTY_SQUARE) {
        isValid = false;
    } else {
        isValid = true;
    }
}

//Check the clock control TIMER period and set each value to a real value
#define TIC_TAC_TOE_TIMER_PERIOD 50.0E-3//TIME period
//Initialize the counters as global variables.
int8_t autoCounter = 0;
int8_t rateCounter = 0;
int8_t adcCounter = 0;
int8_t secondCounter = 0;

#define RESET 0

// This is a debug state print routine. It will print the names of the states each
// time tick() is called. It only prints states if they are different than the
// previous state.
void ticTacToe_debugStatePrint() {
    static ticTacToe_st_t previousState;
    static bool firstPass = true;
    // Only print the message if:
    // 1. This the first pass and the value for previousState is unknown.
    // 2. previousState != currentState - this prevents reprinting the same state name over and over.

    if (previousState != currentState || firstPass) {
        firstPass = false; // previousState will be defined, firstPass is false.
        previousState = currentState; // keep track of the last state that you were in.
        //printf("secondCounter:%d\n\r", secondCounter);
        switch (currentState) { // This prints messages based upon the state that you were in.
        case init_st:  //initialize the state
            printf("init_st\n\r");
            break;
        case start_game_st: //We have not been touched yet
            printf("start_game_st\n\r");
            break;
        case draw_board_st:
            printf("draw_board_st\n\r");
            break;
        case display_untouched_st:
            printf("display_untouched_st");
            break;
        case waiting_for_touch_st:
            printf("waiting_for_touch_st\n\r");
            break;
        case player_turn_st: //Rate timer running
            printf("player_turn_st\n\r");
            break;
        case adc_settle_st:
            printf("adc_settle_st\n\r");
            break;
        case computer_turn_st: //While the time is running slowing
            printf("computer_turn_st\n\r");
            break;
        case end_game_st: //Rate time running
            printf("end_game_st\n\r");
            break;
        case check_score_st:
            printf("check_score_st\n\r");
            break;
        case new_game_st: //add a second to the clock
            printf("new_game_st\n\r");
            break;
        }
    }
}

/* Clock Control
 * @param
 *  this will
 *  update the current state and make the necessary changes.
 */
void ticTacToeControl_tick() {
    //Make sure to print out what state we are in so that we can see what is going wrong.
    //ticTacToe_debugStatePrint();

    // state action first and then the meely outputs..
    switch (currentState) {
    case init_st:  //Start out
        initialize_Board();
        break;
    case start_game_st:  //NOT TOUCHED YET

        if (!displayMessage) {

            writeStartMessage();
            displayMessage = true;
        }
        startmessagecounter++;
        break;
    case draw_board_st:
        eraseStartMessage();
        ticTacToeDisplay_drawBoardLines();
        firstMove = true;
        break;
    case display_untouched_st:
        //nothing
        break;
    case player_turn_st:
        //calculate the location of the touch and updateds temp row and temp column accordingly
        ticTacToeDisplay_touchScreenComputeBoardRowColumn(&temp_row, &temp_col);

        //check to make sure the choice is valid
        player_choice_isValid();

        if (isValid) {
            myBoard.squares[temp_row][temp_col] = MINIMAX_PLAYER_SQUARE;

            if (computerIsX) {
                ticTacToeDisplay_drawO(temp_row, temp_col, false);
            } else {
                ticTacToeDisplay_drawX(temp_row, temp_col, false);
            }
            //notifies that hte player made the last move
            player_moved_last = true;
        } else {
            //move somewhere else..
        }
        break;
    case adc_settle_st:
        adcsettlecounter++;
        break;
    case computer_turn_st:
        //recursively finds the next best move.
        minimax_computeNextMove(&myBoard, false, &temp_row, &temp_col);
        //if the computer is x
        if (computerIsX) {
            ticTacToeDisplay_drawX(temp_row, temp_col, false);
        } else  //if the player went first and is thus x
        {
            ticTacToeDisplay_drawO(temp_row, temp_col, false);
        }
        //notifies the program that the player was not the last to move.
        player_moved_last = false;
        //fills the empty square with a computer square.
        myBoard.squares[temp_row][temp_col] = MINIMAX_OPPONENT_SQUARE;
        break;
    case end_game_st:
        //do nothing
        break;
    case waiting_for_touch_st:
        waitTimerCounter++;
        break;
    case check_score_st:
        //do nothing
        break;
    case new_game_st:
        //do nothing
        firstMove = true;
        initialize_Board();  //initializes the values of the board to 0

        break;
    default:
        currentState = init_st;
        //printf("ticTacToe state update: hit default\n\r");//Otherwise
        break;
    }
    // Perform state action first
    switch (currentState) {
    case init_st: //Start the state up again
        currentState = start_game_st; //switch states
        break;
    case start_game_st: //never touched melee
        if (startmessagecounter >= START_MESSAGE_DURATION) //if hte counter reaches or exceeds the max wait time
        {
            startmessagecounter = RESET; //reset the start message counter
            currentState = draw_board_st; //switches to the draw board state
        }
        break;
    case draw_board_st: // this state draws the board on the touch screen
        currentState = waiting_for_touch_st;
        break;
    case display_untouched_st:
        if (display_isTouched()) {
            currentState = adc_settle_st;
        }
        break;
    case player_turn_st: //the players turn to move

        temp_row = NOT_MODIFIED; //resets the temp_row to an invalid number
        temp_col = NOT_MODIFIED; //resets the temp_col to can invalid number
        if (!isValid) {
            //breaks out after the if statement
        } else {
            currentState = check_score_st;
            isValid = false;
        }
        break;
    case adc_settle_st:
        if (adcsettlecounter >= ADC_IS_SETTLED) {
            adcsettlecounter = RESET;
            currentState = player_turn_st;
        }
        break;
    case computer_turn_st:

        temp_row = NOT_MODIFIED; //resets the temp row variable
        temp_col = NOT_MODIFIED; //resets the temp column variable

        currentState = check_score_st; //switches to the check score state
        break;
    case end_game_st:
        //winner printed at the top
        if (buttons_read() & BUTTONS_0_MASK) {
            eraseXO();
            board_initialized = false; //initialization reset
            currentState = new_game_st; //switches to the new game state
        }
        break;
    case waiting_for_touch_st: //waiting for touch
        if ((waitTimerCounter >= WAIT_TIMER_DURATION) && !display_isTouched()
                && firstMove) {
            waitTimerCounter = RESET;
            computerIsX = true; // if the computer moves first, it is X
            firstMove = false;
            currentState = computer_turn_st;
        } else if (display_isTouched()) {
            waitTimerCounter = RESET; //resets the wait time counter
            if (firstMove) {
                computerIsX = false; //sets computer is X to false
                firstMove = false;
            }
            display_clearOldTouchData();
            currentState = display_untouched_st;

        }
        break;
    case check_score_st:

        myCopy = myBoard; //makes a copy of the board
        //if either player has a score that is final.
        if (minimax_isGameOver(minimax_computeBoardScore(&myCopy, computerIsX))
                || minimax_isGameOver(
                        minimax_computeBoardScore(&myCopy, !computerIsX))) {
            currentState = end_game_st;
        } else if (player_moved_last) {
            //switches to the computer turn state
            currentState = computer_turn_st;
        } else {
            //switches to the player turn state
            currentState = waiting_for_touch_st;
        }
        break;
    case new_game_st:
        currentState = waiting_for_touch_st;
        break;
    default:
        currentState = init_st;        //switches controlled by current state
        //printf("clockControl_tick state action: hit default\n\r");
        break;
    }

}
