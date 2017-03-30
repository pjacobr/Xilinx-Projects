/*
 * main.c
 *
 *  Created on: Sep 13, 2016
 *      Author: pjacobr
 */

#include "buttons.h"
#include "switches.h"



int main()
{

 switches_runTest();
 buttons_runTest();
 //display_fillScreen(DISPLAY_GREEN);



}

void isr_function() {
    // Empty for now.
}
