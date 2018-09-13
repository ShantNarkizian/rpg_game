//Shant Narkizian
//no collaboration
//rpg.c

// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>

//CMPE13 Support Library
#include "UNIXBOARD.h"
#include "Game.h"
#include "Player.h"

// **** Set any macros or preprocessor directives here ****
#define CLR_SCREEN "\033[2J\n"
#define CLR_CURSOR "\033[H"
#define TITLE_COLORS "\033[41m\x1b[34m%s\033[0m\n"
#define DESCR_COLORS "\033[0m%s\n"
#define GO_TO_BOTTOM "\033[44B"
#define UP_3LINES "\033[3A"
#define N_L "\033[E"
#define RIGHT_7LINES "\033[7C"
#define NORTH_RED "\033[31mNORTH"
#define EAST_RED "\033[31mEAST"
#define SOUTH_RED "\033[31mSOUTH"
#define WEST_RED "\033[31mWEST"

#define NORTH_GR "\033[32mNORTH"
#define EAST_GR "\033[32mEAST"
#define SOUTH_GR "\033[32mSOUTH"
#define WEST_GR "\033[32mWEST"

#define DIR_STR "\033[0mEnter Direction to Travel (n,e,s,w) or q to quit followed by enter:"
#define RIGHT_11 "\033[11C"
#define RIGHT_7 "\033[7C"
#define BLACK "\033[0m"

// **** Declare any data types here ****

// **** Define any global or external variables here ****
static char titleA[GAME_MAX_ROOM_TITLE_LENGTH + 1];
static char descrA[GAME_MAX_ROOM_DESC_LENGTH + 1];
static uint8_t exits;
static char input;

// **** Declare any function prototypes here ****
void update2(void);

int main()
{

    /******************************** Your custom code goes below here ********************************/

   int v = GameInit();
   if(v == STANDARD_ERROR){
      FATAL_ERROR();
   }
   GameGetCurrentRoomTitle(titleA);
   GameGetCurrentRoomDescription(descrA);
   exits = GameGetCurrentRoomExits();
   printf(CLR_SCREEN); //clear the screen
   printf(CLR_CURSOR); //to top left corner
   printf(TITLE_COLORS, titleA); //print title
   printf(DESCR_COLORS, descrA); //print description

   printf(GO_TO_BOTTOM); //move to bottom
   printf(UP_3LINES);  //up 3 lines
   printf(RIGHT_7LINES); //right 7 lines
   printf(NORTH_RED);
   printf(N_L); //next line
   printf(WEST_RED);
   printf(RIGHT_11); //to the right
   printf(EAST_RED);
   printf(N_L); //next line
   printf(RIGHT_7); //to the right
   printf(SOUTH_GR);
   printf(N_L); //next line   
   printf(DIR_STR);
   printf(BLACK);

   while(TRUE){
      input = getchar();
      if(input == 'n'){
         GameGoNorth();
         update2();
      }else if(input == 'e'){
         GameGoEast();
         update2();
      }else if(input == 's'){
         GameGoSouth();
         update2();
      }else if(input == 'w'){
         GameGoWest();
         update2();
      }else if(input == 'q'){
         break;
      }
   }

    /**************************************************************************************************/
}

void update2(void){
   GameGetCurrentRoomTitle(titleA);
   GameGetCurrentRoomDescription(descrA);
   exits = GameGetCurrentRoomExits();
   printf(CLR_SCREEN); //clear the screen
   printf(CLR_CURSOR); //to top left corner
   printf(TITLE_COLORS, titleA); //print title  
   printf(DESCR_COLORS, descrA); //print description

   printf(GO_TO_BOTTOM); //move to bottom
   printf(UP_3LINES);  //up 3 lines
   printf(RIGHT_7LINES); //right 7 lines

   if(exits & GAME_ROOM_EXIT_NORTH_EXISTS){
      printf(NORTH_GR); //print in green
   }else{
      printf(NORTH_RED); //print in red
   }

   printf(N_L); //next line
   if(exits & GAME_ROOM_EXIT_WEST_EXISTS){
      printf(WEST_GR); //print in green
   }else{
      printf(WEST_RED); //print in red
   }
   printf(RIGHT_11); //to the right
   if(exits & GAME_ROOM_EXIT_EAST_EXISTS){
      printf(EAST_GR); //print in green
   }else{
      printf(EAST_RED); //print in red
   }
   printf(N_L); //next line
   printf(RIGHT_7LINES); //to the right
   if(exits & GAME_ROOM_EXIT_SOUTH_EXISTS){
      printf(SOUTH_GR);
   }else{
      printf(SOUTH_RED);
   }
   printf(N_L); //next line   
   printf(DIR_STR);
}