/*
 * buttonHandler.c
 *
 *  Created on: Nov 16, 16
 *      Author: pjacobr
 */

#include "buttonHandler.h"
#include "simonDisplay.h"
#include "supportFiles/display.h"
#include "supportFiles/utils.h"
#include <stdio.h>

// How many cycles to wait in the ADC settle state
// Since the period is 50ms, we only wait once.
// This could be eliminated, but remians because the period could change.
#define SETTLED 1
// An invalid region that we initialze our region variable to.
#define INVALID_REGION 5

// A flag for whether the buttonHandler state machine is enabled.
uint8_t enableFlag = 0;
// A variable to increment while waiting for the ADC to settle.
uint8_t settleCounter = 0;
// A flag that says whether the buttons have been drawn already.
// Useful because we don't want to draw the buttons any more than we have to.
uint8_t buttonsDrawn = 0;
// A flag that says whether the screen touch has been released.
uint8_t released = 0;
// A variable to store the region returned by computeRegionNumber.
uint8_t region = INVALID_REGION;

#define TRUE 1
#define FALSE 0

// The buttonHandler state machine states.
enum buttonHandler_st_t {
    init_st,
    draw_buttons_st,
    wait_for_touch_st,
    settle_st,
    is_touched_st,
    untouched_st,
    done_st

} curState = init_st;

// This function decodes the last touched point and returns the region that was touched.
// It will return 0, 1, 2, or 3.
uint8_t buttonHandler_getRegionNumber() {
    int16_t x, y = 0;
    uint8_t z;
    display_getTouchedPoint(&x, &y, &z);
    return simonDisplay_computeRegionNumber(x, y);
}

// Turn on the state machine. Part of the interlock.
void buttonHandler_enable() {
    enableFlag = TRUE;
}

// Turn off the state machine. Part of the interlock.
void buttonHandler_disable() {
    enableFlag = FALSE;
}

// Other state machines can call this function to see if the user has stopped touching the pad.
bool buttonHandler_releaseDetected() {
    // if released flag has been set high, return true and reset the flag.
    if (released) {
        released = FALSE;
        return TRUE;
    }
    // otherwise return false.
    return FALSE;
}

// Standard tick function.
void buttonHandler_tick() {
    // Switch statement for state actions.
    // Most of these do nothing, but are left for good coding practice.
    switch (curState) {
    case init_st:
        break;
    case draw_buttons_st:
        simonDisplay_drawAllButtons();
        break;
    case wait_for_touch_st:
        break;
    case settle_st:
        settleCounter++;
        break;
    case is_touched_st:
        break;
    case untouched_st:
        break;
    case done_st:
        break;
    }

    // state transitions
    switch (curState) {
    case init_st:
        // if the state machine is enabled and the buttons have already been drawn
        // skip straight to waiting for touch
        if (enableFlag && buttonsDrawn) {
            curState = wait_for_touch_st;
        }
        // if enabled but buttons have not been drawn, go to draw buttons state
        else if (enableFlag) {
            curState = draw_buttons_st;
        }
        break;
    case draw_buttons_st:
        // set the buttonsDrawn flag to true
        buttonsDrawn = TRUE;
        // go wait for touch
        curState = wait_for_touch_st;
        break;
    case wait_for_touch_st:
        // wait for the display to be touched, then clear old touch data and
        // go to the ADC settle state
        if (enableFlag) {
            if (display_isTouched()) {
                display_clearOldTouchData();
                curState = settle_st;
            }
        }
        else
        {
            buttonsDrawn = FALSE;
            curState = init_st;
        }

        break;
    case settle_st:
        // wait until the ADC has settled
        if (settleCounter == SETTLED) {
            // reset the settle counter
            settleCounter = FALSE;
            // decode the region touched
            region = buttonHandler_getRegionNumber();
            // draw the square where the region was touched
            simonDisplay_drawSquare(region, FALSE);
            // go to the touched state
            curState = is_touched_st;
        }
        break;
    case is_touched_st:
        // wait until the display is no longer pressed
        if (!display_isTouched()) {
            curState = untouched_st;
        }
        break;
    case untouched_st:
        // draw a black square over the colored sqare
        simonDisplay_drawSquare(region, TRUE);
        // redraw the button for correct region
        simonDisplay_drawButton(region);
        // set the released flag high
        released = TRUE;
        curState = done_st;
        break;
    case done_st:
        // wait till the enable flag goes low and go back to the init state
        if (!enableFlag) {
            // set buttonsDrawn flag low
            buttonsDrawn = FALSE;
            curState = init_st;
        }
        break;
    }
}

/*
 --------------------------------------------------------------------------------
 THIS CODE WAS PROVIDED IN THE LAB DESCRIPTION
 --------------------------------------------------------------------------------
 */

#define RUN_TEST_TERMINATION_MESSAGE1 "buttonHandler_runTest()"
#define RUN_TEST_TERMINATION_MESSAGE2 "terminated."
#define RUN_TEST_TEXT_SIZE 2
// buttonHandler_runTest(int16_t touchCount) runs the test until
// the user has touched the screen touchCount times. It indicates
// that a button was pushed by drawing a large square while
// the button is pressed and then erasing the large square and
// redrawing the button when the user releases their touch.

void buttonHandler_runTest(int16_t touchCountArg) {
    int16_t touchCount = FALSE;          // Keep track of the number of touches.
    display_init();                     // Always have to init the display.
    display_fillScreen(DISPLAY_BLACK);  // Clear the display.
    simonDisplay_drawAllButtons();      // Draw the four buttons.
    buttonHandler_enable();
    while (touchCount < touchCountArg) { // Loop here while touchCount is less than the touchCountArg
        buttonHandler_tick();               // Advance the state machine.
        utils_msDelay(TRUE);           // Wait here for 1 ms.
        if (buttonHandler_releaseDetected()) { // If a release is detected, then the screen was touched.
            touchCount++;                // Keep track of the number of touches.
            printf("button released: %d\n\r", buttonHandler_getRegionNumber()); // Get the region number that was touched.
            buttonHandler_disable(); // Interlocked behavior: handshake with the button handler (now disabled).
            utils_msDelay(TRUE);                     // wait 1 ms.
            buttonHandler_tick();                 // Advance the state machine.
            buttonHandler_enable(); // Interlocked behavior: enable the buttonHandler.
            utils_msDelay(TRUE);                     // wait 1 ms.
            buttonHandler_tick();                 // Advance the state machine.
        }
    }
    display_fillScreen(DISPLAY_BLACK);            // clear the screen.
    display_setTextSize(RUN_TEST_TEXT_SIZE);      // Set the text size.
    display_setCursor(FALSE, display_height() / 2); // Move the cursor to a rough center point.
    display_println(RUN_TEST_TERMINATION_MESSAGE1); // Print the termination message on two lines.
    display_println(RUN_TEST_TERMINATION_MESSAGE2);
}
