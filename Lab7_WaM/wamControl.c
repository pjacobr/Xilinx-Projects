#include "wamControl.h"
#include "wamDisplay.h"
#include "supportFiles/display.h"
#include <stdlib.h>
#include <stdio.h>

 uint16_t msPerTick = 1;
 uint16_t maxMissCount = 0;
 uint16_t maxActiveMoles = 1;
 uint8_t gameOverFlag = 0;
 uint8_t levelCounter = 0;
 uint8_t levelFlag = 1;
 int16_t x = 0;
 int16_t y = 0;
 uint8_t z = 0;
 wamDisplay_point_t whackPoint;

 enum wamControl_st_t {
     init_st,
     activate_mole_st,
     adc_settle_st,
     whack_mole_st,
 } wamState = init_st;

 // Call this before using any wamControl_ functions.
 void wamControl_init(){
     wamState = init_st;
 //    wamControl_setMaxActiveMoles(1);
 }

 // Call this to set how much time is consumed by each tick of the controlling state machine.
 // This information makes it possible to set the awake and sleep time of moles in ms, not ticks.
 void wamControl_setMsPerTick(uint16_t setVal){
     msPerTick = setVal;
 }

 // This returns the time consumed by each tick of the controlling state machine.
 uint16_t wamControl_getMsPerTick(){
     return msPerTick;
 }

 // Standard tick function.
 void wamControl_tick(){
     switch (wamState){
     case init_st:

         break;
     case activate_mole_st:
         if (wamDisplay_getActiveMoleCount() < wamControl_getMaxActiveMoles()){
             wamDisplay_activateRandomMole();
         }
         wamDisplay_updateAllMoleTickCounts();
         break;
     case adc_settle_st:
         break;
     case whack_mole_st:
         display_getTouchedPoint(&x, &y, &z);
         whackPoint.x = x;
         whackPoint.y = y;
         if (wamDisplay_whackMole(&whackPoint) != -1){
             if (wamDisplay_getHitScore() != 0 && wamDisplay_getHitScore()%5 == 0){
                 wamDisplay_incrementLevel();
             }

             if (wamDisplay_getLevel() < 5){
                 wamControl_setMaxActiveMoles(wamDisplay_getLevel()+1);
             }
         }
         break;
     }

     switch (wamState){
     case init_st:
         wamState = activate_mole_st;
         break;
     case activate_mole_st:
         if (display_isTouched()){
             display_clearOldTouchData();
             levelFlag = 1;
             wamState = adc_settle_st;
         }
         break;
     case adc_settle_st:
         wamState = whack_mole_st;
         break;
     case whack_mole_st:
//        if (wamDisplay_getHitScore()%5 == 0 && wamDisplay_getHitScore() != 0 && levelFlag){
//            levelFlag = 0;
//            wamDisplay_incrementLevel();
//            if (wamDisplay_getLevel() < 5){
//                wamControl_setMaxActiveMoles(wamDisplay_getLevel()+1);
//            }
//        }
         wamState = activate_mole_st;
         break;
     }
 }

 // Returns a random value that indicates how long the mole should sleep before awaking.
 wamDisplay_moleTickCount_t wamControl_getRandomMoleAsleepInterval(){
     return rand()%((25+wamDisplay_getLevel())) + 5;
 }

 // Returns a random value that indicates how long the mole should stay awake before going dormant.
 wamDisplay_moleTickCount_t wamControl_getRandomMoleAwakeInterval(){
     return rand()%((25+wamDisplay_getLevel())) + 5;
 }

 // Set the maximum number of active moles.
 void wamControl_setMaxActiveMoles(uint16_t count){
     maxActiveMoles = count;
 }

 // Get the current allowable count of active moles.
 uint16_t wamControl_getMaxActiveMoles(){
     return maxActiveMoles;
 }

 // Set the seed for the random-number generator.
 void wamControl_setRandomSeed(uint32_t seed){
     srand(seed);
 }

 // Set the maximum number of misses until the game is over.
 void wamControl_setMaxMissCount(uint16_t missCount){
     maxMissCount = missCount;
 }

 // Use this predicate to see if the game is finished.
 bool wamControl_isGameOver(){
     if (maxMissCount == wamDisplay_getMissScore()){
         return 1;
     }
     return 0;
 }
