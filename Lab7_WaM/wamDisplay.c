#include "supportFiles/display.h"
#include "wamDisplay.h"
#include "wamControl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Lab_2/switches.h"

// X coordinate for the score
#define SCORE_X_POS (DISPLAY_HEIGHT-15)
// hit text y coordinate
#define HIT_TEXT_Y_OFFSET 0
// miss text y coordinate
#define MISS_TEXT_Y_OFFSET 90
// level text y coordinate
#define LEVEL_TEXT_Y_OFFSET 200
// y coordinate for how tall the mole board is
// basically display height minus the score screen height
#define MOLE_BOARD_HEIGHT (DISPLAY_HEIGHT-30)
// size for the title text ("whack a mole")
#define TITLE_TEXT_SIZE 3
// size for all sub text
#define SUB_TEXT_SIZE 2
// size of the char arrays to store scores in
#define STRING_SIZE 10
// text to show on the title screen
#define WAM_TITLE_TEXT "Whack-A-Mole!"
// title scren sub text
#define WAM_TITLE_SUB_TEXT "touch screen to start"
// game over screen title text
#define GAME_OVER_TEXT "GAME OVER"
// final level string
#define FINAL_LEVEL_TEXT "Final Level:%d"
// hit count string
#define HIT_TEXT "Hit:%d"
// miss count string
#define MISS_TEXT "Miss:%d"
// level count string
#define LEVEL_TEXT "Level:%d"
// how big each mole circle is
// this is the mole itself, not the mole hole
#define MOLE_RADIUS 20
// a value used to say there wasn't a mole at the location specified
#define INVALID_INDEX_VALUE 66
// value for four hole board
#define FOUR_MOLE_BOARD 4
// value for six hole board
#define SIX_MOLE_BOARD 6
// value for nine hole board
#define NINE_MOLE_BOARD 9
// there and 3 rows and 3 columns
// the following values show what touch points are valid for each row and column
// the top row's min and max y values
#define TOP_ROW_MIN_Y 10
#define TOP_ROW_MAX_Y 70
// the left most columns x range
#define LEFT_COLUMN_MIN_X 10
#define LEFT_COLUMN_MAX_X 70
// the middle column's x range
#define MIDDLE_COLUMN_MIN_X 130
#define MIDDLE_COLUMN_MAX_X 190
// the right most column's x range
#define RIGHT_COLUMN_MIN_X 250
#define RIGHT_COLUMN_MAX_X 310
// the middle row's y range
#define MIDDLE_ROW_MIN_Y 95
#define MIDDLE_ROW_MAX_Y 125
// the bottom row's y range
#define BOTTOM_ROW_MIN_Y 140
#define BOTTOM_ROW_MAX_Y 200
// the following are used to decode a mole hole number to an index in the
// mole info array
// mole 1 (top-left) is actually index 0 in the array
#define MOLE_1_INDEX 0
#define MOLE_2_INDEX 1
#define MOLE_3_INDEX 2
#define MOLE_4_INDEX 3
#define MOLE_5_INDEX 4
#define MOLE_6_INDEX 5
#define MOLE_7_INDEX 6
#define MOLE_8_INDEX 7
#define MOLE_9_INDEX 8
// value to return if the whack was not on a valid mole
#define WHACK_FAILED -1
// row-column values used when drawing the holes
#define COLUMN_1_X 40
#define ROW_1_Y 40
#define COLUMN_2_X 160
#define ROW_2_Y 105
#define COLUMN_3_X 280
#define ROW_3_Y 170

// how big the mole hole is
#define HOLE_RADIUS 30
// these are used to show which hole is being selected
#define HOLE_1 1
#define HOLE_2 2
#define HOLE_3 3
#define HOLE_4 4
#define HOLE_5 5
#define HOLE_6 6
#define HOLE_7 7
#define HOLE_8 8
#define HOLE_9 9
// x y coordinates of the title text
#define WAM_TITLE_TEXT_X 45
#define WAM_TITLE_TEXT_Y 100
// x y coordinates of the title screen sub text
#define WAM_TITLE_SUB_TEXT_X 30
#define WAM_TITLE_SUB_TEXT_Y 130
// x y coordinates of the game over title text
#define GAME_OVER_TEXT_X 80
#define GAME_OVER_TEXT_Y 50
// x y coordinates of the hit text score
#define HIT_TEXT_X 125
#define HIT_TEXT_Y 90
// x y coordinates of the miss text score
#define MISS_TEXT_X 115
#define MISS_TEXT_Y 110
// x y coordinates of the final level game over screen text
#define FINAL_LEVEL_TEXT_X 85
#define FINAL_LEVEL_TEXT_Y 130

 #define FOUR_HOLE_MASK 0x4
 #define SIX_HOLE_MASK 0x6
 #define MAX_STR 255
 #define SCORE_X_POS (DISPLAY_HEIGHT-15)
 #define HIT_TEXT_Y_OFFSET 0
 #define MISS_TEXT_Y_OFFSET 90
 #define LEVEL_TEXT_Y_OFFSET 200
 #define MOLE_BOARD_HEIGHT (DISPLAY_HEIGHT-30)
 #define TITLE_TEXT_SIZE 3
 #define SUB_TEXT_SIZE 2
 #define STRING_SIZE 10
 #define WAM_TITLE_TEXT "Whack-A-Mole!"
 #define WAM_TITLE_SUB_TEXT "touch screen to start"
 #define GAME_OVER_TEXT "GAME OVER"
 #define FINAL_LEVEL_TEXT "Final Level:%d"
 #define HIT_TEXT "Hit:%d"
 #define MISS_TEXT "Miss:%d"
 #define LEVEL_TEXT "Level:%d"

 // the number of mole holes on the current game board
 wamDisplay_moleCount_e numHoles = wamDisplay_moleCount_9;
 uint8_t numMoleHoles = 0;
 uint16_t hitCount = 0;
 uint16_t missCount = 0;
 uint16_t levelCount = 0;
 wamDisplay_point_t moleXY;
 uint16_t activeMoleCount = 0;

 char hitStr[STRING_SIZE];
 char missStr[STRING_SIZE];
 char levelStr[STRING_SIZE];
 char finalLevelStr[STRING_SIZE];

 /********************** typedefs **********************/
 // This keeps track of all mole information.
 typedef struct {
         wamDisplay_point_t origin;  // This is the origin of the hole for this mole.
         // A mole is active if either of the tick counts are non-zero. The mole is dormant otherwise.
         // During operation, non-zero tick counts are decremented at a regular rate by the control state machine.
         // The mole remains in his hole until ticksUntilAwake decrements to zero and then he pops out.
         // The mole remains popped out of his hole until ticksUntilDormant decrements to zero.
         // Once ticksUntilDomant goes to zero, the mole hides in his hole and remains dormant until activated again.
         wamDisplay_moleTickCount_t ticksUntilAwake;  // Mole will wake up (pop out of hole) when this goes from 1 -> 0.
         wamDisplay_moleTickCount_t ticksUntilDormant; // Mole will go dormant (back in hole) this goes 1 -> 0.
 } wamDisplay_moleInfo_t;

 // This will contain pointers to all of the mole info records.
 // This will ultimately be treated as an array of pointers.
 static wamDisplay_moleInfo_t** wamDisplay_moleInfo;

 // Allocates the memory for wamDisplay_moleInfo_t records.
 // Computes the origin for each mole assuming a simple row-column layout:
 // 9 moles: 3 rows, 3 columns, 6 moles: 2 rows, 3 columns, 4 moles: 2 rows, 2 columns
 // Also inits the tick counts for awake and dormant.

 void drawMole(wamDisplay_point_t origin, uint8_t erase){
     uint32_t color = DISPLAY_YELLOW;

     if (erase){
         color = DISPLAY_BLACK;
     }

     display_fillCircle(origin.x, origin.y, 20, color);
 }

 uint16_t decodeTouchData(uint16_t x, uint16_t y){
     uint16_t index = 66;

     if (numMoleHoles == 4){
         if (y > 10 && y < 70){
             if (x > 10 && x < 70){
                 index = 0;
             }
             else if (x > 250 && x < 310){
                 index = 1;
             }
         }
         else if (y > 140 && y < 200){
             if (x > 10 && x < 70){
                 index = 2;
             }
             else if (x > 250 && x < 310){
                 index = 3;
             }
         }
     }

     else if (numMoleHoles == 6){
         if (y > 10 && y < 70){
             if (x > 10 && x < 70){
                 index = 0;
             }
             else if (x > ((DISPLAY_WIDTH/2)-30) && x < ((DISPLAY_WIDTH/2)+30)){
                 index = 1;
             }
             else if (x > 250 && x < 310){
                 index = 2;
             }
         }

         else if (y > 140 && y < 200){
             if (x > 10 && x < 70){
                 index = 3;
             }
             else if (x > ((DISPLAY_WIDTH/2)-30) && x < ((DISPLAY_WIDTH/2)+30)){
                 index = 4;
             }
             else if (x > 250 && x < 310){
                 index = 5;
             }
         }
     }

     else {
         if (y > 10 && y < 70){
             if (x > 10 && x < 70){
                 index = 0;
             }
             else if (x > ((DISPLAY_WIDTH/2)-30) && x < ((DISPLAY_WIDTH/2)+30)){
                 index = 1;
             }
             else if (x > 250 && x < 310){
                 index = 2;
             }
         }

         if (y > 95 && y < 125){
             if (x > 10 && x < 70){
                 index = 3;
             }
             else if (x > ((DISPLAY_WIDTH/2)-30) && x < ((DISPLAY_WIDTH/2)+30)){
                 index = 4;
             }
             else if (x > 250 && x < 310){
                 index = 5;
             }
         }

         else if (y > 140 && y < 200){
             if (x > 10 && x < 70){
                 index = 6;
             }
             else if (x > ((DISPLAY_WIDTH/2)-30) && x < ((DISPLAY_WIDTH/2)+30)){
                 index = 7;
             }
             else if (x > 250 && x < 310){
                 index = 8;
             }
         }
     }
     return index;
 }

 wamDisplay_moleIndex_t wamDisplay_whackMole(wamDisplay_point_t* whackOrigin){
     uint16_t index = decodeTouchData(whackOrigin->x, whackOrigin->y);
     if (index == 66){
         return -1;
     }

     if (wamDisplay_moleInfo[index]->ticksUntilAwake == 0 && wamDisplay_moleInfo[index]->ticksUntilDormant != 0){
         wamDisplay_moleInfo[index]->ticksUntilDormant = 0;
         drawMole(wamDisplay_moleInfo[index]->origin, 1);
         hitCount++;
         activeMoleCount--;
         wamDisplay_drawScoreScreen();
         return index;
     }

     return -1;
 }

 void wamDisplay_resetAllScoresAndLevel(){
     hitCount = 0;
     missCount = 0;
     levelCount = 0;
     activeMoleCount = 0;
    for (int i = 0; i<numMoleHoles; i++){
        wamDisplay_moleInfo[i]->ticksUntilAwake = 0;
        wamDisplay_moleInfo[i]->ticksUntilDormant = 0;
//        wamDisplay_moleInfo[i]->or

    // When you are done, you must return the memory to the system or you will create a memory leak.
    // First deallocate all of the structs.
    for (uint16_t l=0; l<numMoleHoles; l++) {
        free(wamDisplay_moleInfo[l]);   // This deallocates the memory.
        wamDisplay_moleInfo[l] = NULL;  // This step is not necessary but will keep you from reusing deallocated memory.
    }
    // Next, deallocate the array that contains the pointers to the structs.
    free(wamDisplay_moleInfo);   // Deallocates the container of arrays.
    wamDisplay_moleInfo = NULL;  // Also keeps you from reusing the deallocated memory.

//    for (int i = 0; i<numMoleHoles; i++){
//        wamDisplay_moleInfo[i]->ticksUntilAwake = 0;
//        wamDisplay_moleInfo[i]->ticksUntilDormant = 0;
//    }
 }
 }

 void wamDisplay_incrementMissScore(){
     missCount++;
 }

 uint16_t wamDisplay_getLevel(){
     return levelCount;
 }

 void wamDisplay_incrementLevel(){
     levelCount++;
 }

 uint16_t wamDisplay_getMissScore(){
     return missCount;
 }

 void wamDisplay_setMissScore(uint16_t misses){
     missCount = misses;
 }

 uint16_t wamDisplay_getHitScore(){
     return hitCount;
 }

 void wamDisplay_setHitScore(uint16_t hits){
     hitCount = hits;
 }

 void getMoleCoordinates(uint16_t index) {
     if (numMoleHoles == 4){
         switch (index){
         case 1:
             moleXY.x = 40;
             moleXY.y = 40;
             break;
         case 2:
             moleXY.x = (DISPLAY_WIDTH - 40);
             moleXY.y = 40;
             break;
         case 3:
             moleXY.x = 40;
             moleXY.y = 170;
             break;
         case 4:
             moleXY.x = (DISPLAY_WIDTH - 40);
             moleXY.y = 170;
             break;
         }
     }
     else if (numMoleHoles == 6){
         switch (index){
         case 1:
             moleXY.x = 40;
             moleXY.y = 40;
             break;
         case 2:
             moleXY.x = (DISPLAY_WIDTH/2);
             moleXY.y = 40;
             break;
         case 3:
             moleXY.x = (DISPLAY_WIDTH - 40);
             moleXY.y = 40;
             break;
         case 4:
             moleXY.x = 40;
             moleXY.y = 170;
             break;
         case 5:
             moleXY.x = (DISPLAY_WIDTH/2);
             moleXY.y = 170;
             break;
         case 6:
             moleXY.x = (DISPLAY_WIDTH - 40);
             moleXY.y = 170;
             break;
         }
     }
     else {
         switch (index){
         case 1:
             moleXY.x = 40;
             moleXY.y = 40;
             break;
         case 2:
             moleXY.x = (DISPLAY_WIDTH/2);
             moleXY.y = 40;
             break;
         case 3:
             moleXY.x = (DISPLAY_WIDTH - 40);
             moleXY.y = 40;
             break;
         case 4:
             moleXY.x = 40;
             moleXY.y = 105;
             break;
         case 5:
             moleXY.x = (DISPLAY_WIDTH/2);
             moleXY.y = 105;
             break;
         case 6:
             moleXY.x = (DISPLAY_WIDTH - 40);
             moleXY.y = 105;
             break;
         case 7:
             moleXY.x = 40;
             moleXY.y = 170;
             break;
         case 8:
             moleXY.x = (DISPLAY_WIDTH/2);
             moleXY.y = 170;
             break;
         case 9:
             moleXY.x = (DISPLAY_WIDTH - 40);
             moleXY.y = 170;
             break;
         }
     }
 }

 void drawHole(uint8_t hole){
         switch (hole){
         case 1:
             display_fillCircle(40, 40, 30, DISPLAY_BLACK);
             break;
         case 2:
             display_fillCircle(DISPLAY_WIDTH/2, 40, 30, DISPLAY_BLACK);
             break;
         case 3:
             display_fillCircle(DISPLAY_WIDTH - 40, 40, 30, DISPLAY_BLACK);
             break;
         case 4:
             display_fillCircle(40, 105, 30, DISPLAY_BLACK);
             break;
         case 5:
             display_fillCircle(DISPLAY_WIDTH/2, 105, 30, DISPLAY_BLACK);
             break;
         case 6:
             display_fillCircle(DISPLAY_WIDTH - 40, 105, 30, DISPLAY_BLACK);
             break;
         case 7:
             display_fillCircle(40, 170, 30, DISPLAY_BLACK);
             break;
         case 8:
             display_fillCircle(DISPLAY_WIDTH/2, 170, 30, DISPLAY_BLACK);
             break;
         case 9:
             display_fillCircle(DISPLAY_WIDTH - 40, 170, 30, DISPLAY_BLACK);
             break;
         }
 }

 void drawMoleHoles(){
     printf("%d\n", numMoleHoles);
     drawHole(1);
     drawHole(3);
     drawHole(7);
     drawHole(9);

     if (numMoleHoles >= 6){
         drawHole(2);
         drawHole(8);
     }

     if (numMoleHoles == 9){
         drawHole(4);
         drawHole(5);
         drawHole(6);
     }

 }

 void wamDisplay_computeMoleInfo() {
     /*-----------------------------------------------------------------------------------------------------------------------*/
     wamDisplay_moleInfo = (wamDisplay_moleInfo_t**) malloc(numMoleHoles * sizeof(wamDisplay_moleInfo_t*));
     for (int i=0; i<numMoleHoles; i++) {
         // Second, allocate an instance of myStruct and point to it.
         wamDisplay_moleInfo[i] = (wamDisplay_moleInfo_t*) malloc(sizeof(wamDisplay_moleInfo_t));
     }
     for (int i=0; i<numMoleHoles; i++) {
         getMoleCoordinates(i+1);
         wamDisplay_moleInfo[i]->origin = moleXY;
         wamDisplay_moleInfo[i]->ticksUntilAwake = 0;
         wamDisplay_moleInfo[i]->ticksUntilDormant = 0;
     }
     /*-----------------------------------------------------------------------------------------------------------------------*/
 }

 void wamDisplay_selectMoleCount(wamDisplay_moleCount_e moleCount){
     switch(moleCount){
     case wamDisplay_moleCount_4:
         numMoleHoles = 4;
         break;
     case wamDisplay_moleCount_6:
         numMoleHoles = 6;
         break;
     default:
         numMoleHoles = 9;
         break;
     }

     wamDisplay_computeMoleInfo();
 }

 void wamDisplay_updateAllMoleTickCounts(){
     for (int i = 0; i<numMoleHoles; i++){
         if (wamDisplay_moleInfo[i]->ticksUntilAwake > 0){
             wamDisplay_moleInfo[i]->ticksUntilAwake--;
             if (wamDisplay_moleInfo[i]->ticksUntilAwake == 0){
                 drawMole(wamDisplay_moleInfo[i]->origin, 0);
             }
         }
         else if (wamDisplay_moleInfo[i]->ticksUntilDormant > 0){
             wamDisplay_moleInfo[i]->ticksUntilDormant -= 1;
             if (wamDisplay_moleInfo[i]->ticksUntilDormant == 0){
                 drawMole(wamDisplay_moleInfo[i]->origin, 1);
                 missCount++;
                 wamDisplay_drawScoreScreen();
                 activeMoleCount--;
             }
         }
     }
 }

 bool wamDisplay_activateRandomMole(){
     uint8_t index = rand()%(numMoleHoles);
     if (wamDisplay_moleInfo[index]->ticksUntilAwake == 0 && wamDisplay_moleInfo[index]->ticksUntilDormant == 0){
         wamDisplay_moleInfo[index]->ticksUntilAwake = wamControl_getRandomMoleAwakeInterval();
         wamDisplay_moleInfo[index]->ticksUntilDormant = wamControl_getRandomMoleAsleepInterval();
         activeMoleCount++;
         return 1;
     }
     else {
         return 0;
     }
 }

 uint16_t wamDisplay_getActiveMoleCount(){
     return activeMoleCount;
 }

 void wamDisplay_init(){
 //    display_init();
 //    display_fillScreen(DISPLAY_BLACK);
     display_setTextColor(DISPLAY_WHITE, DISPLAY_BLACK);
 }

 void wamDisplay_drawSplashScreen(){
     display_fillScreen(DISPLAY_BLACK);
     display_setTextSize(TITLE_TEXT_SIZE);
     display_setCursor(45, 100);
     display_println(WAM_TITLE_TEXT);
     display_setTextSize(SUB_TEXT_SIZE);
     display_setCursor(30, 130);
     display_println(WAM_TITLE_SUB_TEXT);
 }

 void wamDisplay_drawGameOverScreen(){
     display_fillScreen(DISPLAY_BLACK);
     display_setTextSize(TITLE_TEXT_SIZE);
     display_setCursor(80, 50);
     display_println(GAME_OVER_TEXT);
     display_setTextSize(SUB_TEXT_SIZE);
     display_setCursor(125, 90);
     display_println(hitStr);
     display_setCursor(115, 110);
     display_println(missStr);
     sprintf(finalLevelStr, FINAL_LEVEL_TEXT, levelCount);
     display_setCursor(85, 130);
     display_println(finalLevelStr);
 }

 void wamDisplay_drawScoreScreen(){
     display_setTextSize(SUB_TEXT_SIZE);
     sprintf(hitStr, HIT_TEXT, hitCount);
     sprintf(missStr, MISS_TEXT, missCount);
     sprintf(levelStr, LEVEL_TEXT, levelCount);
     display_setCursor(HIT_TEXT_Y_OFFSET, SCORE_X_POS);
     display_println(hitStr);
     display_setCursor(MISS_TEXT_Y_OFFSET, SCORE_X_POS);
     display_println(missStr);
     display_setCursor(LEVEL_TEXT_Y_OFFSET, SCORE_X_POS);
     display_println(levelStr);
 }

 void wamDisplay_drawMoleBoard(){
     wamDisplay_drawScoreScreen();
     display_fillRect(0, 0, DISPLAY_WIDTH, MOLE_BOARD_HEIGHT, DISPLAY_GREEN);
     drawMoleHoles();
 }

 uint8_t splashFlag = 0;
 uint8_t boardFlag = 0;
 uint8_t overFlag = 0;


 void wamDisplay_runMilestone1_test(){
     wamDisplay_init();
 //    wamDisplay_drawSplashScreen();
     while (1){
         if (!splashFlag){
             wamDisplay_drawSplashScreen();
             splashFlag = 1;
         }
         if (display_isTouched()){
             splashFlag = 0;
             while (1){
                 missCount++;
                 wamDisplay_drawScoreScreen();
                 if (!boardFlag){
                     wamDisplay_drawMoleBoard();
                     wamDisplay_drawScoreScreen();
                     boardFlag = 1;
                     missCount++;
                 }
                 if (display_isTouched()){
                     boardFlag = 0;
                     while (1){
                         if (!overFlag){
                             wamDisplay_drawGameOverScreen();
                             overFlag = 1;
                         }
                         if (display_isTouched()){
                             overFlag = 0;
                             hitCount++;
                             missCount++;
                             levelCount++;
                             break;
                         }
                     }
                     break;
                 }
             }
         }

     }
 }


