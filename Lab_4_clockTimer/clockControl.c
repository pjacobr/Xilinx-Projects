#include "clockControl.h"
#include "clockDisplay.h"
#include <stdio.h>
#include "supportFiles/display.h"


// States for the controller state machine.
enum clockControl_st_t {
    init_st,                 // Start here, stay in this state for just one tick.
    never_touched_st,        // Wait here until the first touch - clock is disabled until set.
    waiting_for_touch_st,    // waiting for touch, clock is enabled and running.
    ad_timer_running_st,     // waiting for the touch-controller ADC to settle.
    auto_timer_running_st,   // waiting for the auto-update delay to expire
                                 // (user is holding down button for auto-inc/dec)
    rate_timer_running_st,   // waiting for the rate-timer to expire to know when to perform the auto inc/dec.
    rate_timer_expired_st,   // when the rate-timer expires, perform the inc/dec function.
    add_second_to_clock_st   // add a second to the clock time and reset the ms counter.
} currentState = init_st;


//Check the clock control TIMER period and set each value to a real value
#define CLOCK_CONTROL_TIMER_PERIOD 50.0E-3//TIME period
#define SECOND_COUNTER_MAX (1000.0E-3 / CLOCK_CONTROL_TIMER_PERIOD)//Update the counter max
#define ADC_COUNTER_MAX_VALUE (50.0E-3 /CLOCK_CONTROL_TIMER_PERIOD)//The ADC_counter max value divided by the timer period
#define AUTO_COUNTER_MAX_VALUE (500.0E-3 /CLOCK_CONTROL_TIMER_PERIOD)
#define RATE_COUNTER_MAX_VALUE (50.0E-3 / CLOCK_CONTROL_TIMER_PERIOD)
//Initialize the counters as global variables.
int autoCounter = 0;
int rateCounter = 0;
int adcCounter = 0;
int secondCounter = 0;

#define RESET 0


// This is a debug state print routine. It will print the names of the states each
// time tick() is called. It only prints states if they are different than the
// previous state.
void debugStatePrint() {
  static clockControl_st_t previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state name over and over.
  if (previousState != currentState || firstPass) {
    firstPass = false;                // previousState will be defined, firstPass is false.
    previousState = currentState;     // keep track of the last state that you were in.
    printf("secondCounter:%d\n\r", secondCounter);
    switch(currentState) {            // This prints messages based upon the state that you were in.
      case init_st:  //initialize the state 
        printf("init_st\n\r");
        break;
      case never_touched_st: //We have not been touched yet
        printf("never_touched_st\n\r");
        break;
      case waiting_for_touch_st: //Waiting on the touch
        printf("waiting_for_touch_st\n\r");
        break;
      case ad_timer_running_st: //While the time is running slowing
        printf("ad_timer_running_st\n\r");
        break;
      case auto_timer_running_st: //Auto time running
        printf("auto_timer_running_st\n\r");
        break;
      case rate_timer_running_st: //Rate timer running
        printf("rate_timer_running_st\n\r");
        break;
      case rate_timer_expired_st: //Rate time running
        printf("rate_timer_expired_st\n\r");
        break;
      case add_second_to_clock_st: //add a second to the clock
          printf("add_second_to_clock_st\n\r");
        break;
     }
  }
}


/* Clock Control
 * @param
 *  this will
 *  update the current state and make the necessary changes.
 */
void clockControl_tick() {
    //Make sure to print out what state we are in so that we can see what is going wrong.
  debugStatePrint();

  // Perform state update next.
  switch(currentState) {
    case init_st://Start out
      break;
    case never_touched_st://NOT TOUCHED YET
      break;
    case waiting_for_touch_st://Waiting to be touched
        adcCounter = RESET;//reset each of the counters
        rateCounter = RESET;
        autoCounter = RESET;
      break;
    case ad_timer_running_st://the state changes
        adcCounter++;//increment the ADC counter to make sure that it gets updated
      break;
    case auto_timer_running_st://while the
        autoCounter++;
      break;
    case rate_timer_running_st://While the rate time is running
        rateCounter++;
      break;
    case rate_timer_expired_st://While the rate timer is expired
        rateCounter = RESET;
      break;
    case add_second_to_clock_st://While touched do this
        if(secondCounter > SECOND_COUNTER_MAX)
        {
            clockDisplay_advanceTimeOneSecond();//Go up by one second and update the display
            secondCounter = RESET;
        }
      break;
    default:
      printf("clockControl_tick state update: hit default\n\r");//Otherwise
      break;
  }
  // Perform state action first.
  switch(currentState) {
    case init_st: //Start the state up again
        currentState = never_touched_st; //switch states
      break;
    case never_touched_st: //never touched melee

        secondCounter++; //increment the counter

        if(display_isTouched()) //Is the display being touched currently?
        {
            display_clearOldTouchData(); //clear the data gathered on teh touch data
            currentState = ad_timer_running_st;
        }
      break;
    case waiting_for_touch_st: //waiting for touch 
        if(!(display_isTouched())) //display is being currently touched
        {
            secondCounter++; //second is incremented
            currentState = add_second_to_clock_st; //go to the add clock
        }
        else
        {
            display_clearOldTouchData(); //clear out the touched data
            currentState = ad_timer_running_st; //ad timer running
        }
      break;
    case ad_timer_running_st: //timer running state
        if(!display_isTouched() && (adcCounter == ADC_COUNTER_MAX_VALUE)) //Check to see if the display is not touched 
        {
            clockDisplay_performIncDec(); //increment!!
            currentState = waiting_for_touch_st; //go to the next state
        }
        else if(display_isTouched() && (adcCounter == ADC_COUNTER_MAX_VALUE)) //check to see if the display is touched and the ADC_COUnter is a the max
        {
            currentState = auto_timer_running_st;
        }
      break;
    case auto_timer_running_st: //auto timer running state
        if(!display_isTouched())
        {
            clockDisplay_performIncDec(); //perform the clock Display
            currentState = waiting_for_touch_st; //waiting for touch
        }
        else if(display_isTouched() && autoCounter == AUTO_COUNTER_MAX_VALUE) //is the display touched? 
        {
            clockDisplay_performIncDec(); //perform the increment and decrement
            currentState = rate_timer_running_st;
        }
      break;
    case rate_timer_running_st: //THis state
        if(!display_isTouched()) //the display is not touched
        {
            currentState = waiting_for_touch_st; //goto the next state
        }
        else if(display_isTouched() && (rateCounter == RATE_COUNTER_MAX_VALUE)) //display is touched and 
        {
            currentState = rate_timer_expired_st; //This is the current state
        }
      break;
    case rate_timer_expired_st: //this state is called
        if(display_isTouched()) //is the display touched 
        {
            clockDisplay_performIncDec(); //This lab is dumb
            currentState = rate_timer_running_st; //rate timer is the next state
        }
        else
        {
            currentState = waiting_for_touch_st; //Current state is the next state
        }
      break;
    case add_second_to_clock_st: //add a second to the clock state!
        if(!display_isTouched()) //is NOT touched
        {
            secondCounter++; //increment the counter
        }
        else
        {
            display_clearOldTouchData(); //clear out the data gathered from the touch screen
            currentState = ad_timer_running_st; //currentState is the ad timer running
        }
        break;
     default:
      printf("clockControl_tick state action: hit default\n\r");
      break;
  }

}
