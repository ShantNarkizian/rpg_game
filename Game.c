//Shant Narkizian
//no collaboration
//Game.c

#include "Player.h"
#include "Game.h"
#include "UNIXBOARD.h"
#include <string.h>
#include <stdio.h>
#define ZERO 0
#define FIRST_EXIT 0
#define SECOND_EXIT 1
#define THIRD_EXIT 2
#define FOURTH_EXIT 3 
#define NLL '\0'
#define ON 1
#define SST "RoomFiles/room%d.txt"
#define BINARY_MODE "rb"
#define INCREMENT 1
#define FL_SIZE 15

static int TitleLen;
static int RequiredLen;
static int DescLen;
static int ItemGivenLen;

static int ExitTrash;
static FILE *pFile;
static int i; //for loop control
static int flag = ZERO;

static struct CurrentRoom{
   char title[GAME_MAX_ROOM_TITLE_LENGTH + INCREMENT]; //fits all titles
   char description[GAME_MAX_ROOM_DESC_LENGTH + INCREMENT]; //fits all descriptions 
   char desctrash[GAME_MAX_ROOM_DESC_LENGTH + INCREMENT];
   int itemsgiventrash[INVENTORY_SIZE];
   uint8_t InventoryBuffer[INVENTORY_SIZE];
   int Requireditems[INVENTORY_SIZE]; //required to enter the version of room
   uint8_t exits[INVENTORY_SIZE];
   int RoomNum;
   char filename[FL_SIZE];
}CR;

//function prototype
int Update(void);

int GameGoNorth(void){
  if(GameGetCurrentRoomExits() & GAME_ROOM_EXIT_NORTH_EXISTS){
      CR.RoomNum = CR.exits[FIRST_EXIT]; //get next room #    
      sprintf(CR.filename, SST, CR.RoomNum);
      pFile = fopen(CR.filename, BINARY_MODE); //open file in binary

      if(pFile == NULL){ //make sure file is not NULL
         FATAL_ERROR();
      }
      for(i = ZERO; i < GAME_MAX_ROOM_DESC_LENGTH; i++){ //reser descr
         CR.description[i] = NLL;
      }
      int ret = Update();
      return ret;
   }else{
      return STANDARD_ERROR;
   }
}

int GameGoEast(void){
   if(GameGetCurrentRoomExits() & GAME_ROOM_EXIT_EAST_EXISTS){
      CR.RoomNum = CR.exits[SECOND_EXIT]; //get next room #      
      sprintf(CR.filename, SST, CR.RoomNum);
      pFile = fopen(CR.filename, BINARY_MODE); //open file in binary

      if(pFile == NULL){ //make sure file is not NULL
         FATAL_ERROR();
      }
      for(i = ZERO; i < GAME_MAX_ROOM_DESC_LENGTH; i++){ //reser descr
         CR.description[i] = NLL;
      }
      int ret = Update();
      return ret;
   }else{
      return STANDARD_ERROR;
   }
}

int GameGoSouth(void){
   if(GameGetCurrentRoomExits() & GAME_ROOM_EXIT_SOUTH_EXISTS){
      CR.RoomNum = CR.exits[THIRD_EXIT]; //get next room #      
      sprintf(CR.filename, SST, CR.RoomNum);
      pFile = fopen(CR.filename, BINARY_MODE); //open file in binary

      if(pFile == NULL){ //make sure file is not NULL
         FATAL_ERROR();
      }
      for(i = ZERO; i < GAME_MAX_ROOM_DESC_LENGTH; i++){ //reser descr
         CR.description[i] = NLL;
      }
      int ret = Update();
      return ret;
   }else{
      return STANDARD_ERROR;
   }
}

int GameGoWest(void){
   if(GameGetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS){
      CR.RoomNum = CR.exits[FOURTH_EXIT]; //get next room #      
      sprintf(CR.filename, SST, CR.RoomNum);
      pFile = fopen(CR.filename, BINARY_MODE); //open file in binary

      if(pFile == NULL){ //make sure file is not NULL
         FATAL_ERROR();
      }
      for(i = ZERO; i < GAME_MAX_ROOM_DESC_LENGTH; i++){ //reser descr
         CR.description[i] = NLL;
      }
      int ret = Update();
      return ret;
   }else{
      return STANDARD_ERROR;
   }
}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void){
   CR.RoomNum = STARTING_ROOM; //set current room # to 32
   sprintf(CR.filename, SST, CR.RoomNum);
   pFile = fopen(CR.filename, BINARY_MODE); //open file in binary

   if(pFile == NULL){ //make sure file is not NULL
      FATAL_ERROR();
   }

   TitleLen = fgetc(pFile); //get first number - title length
   TitleLen ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //decrpyt with xor

   fread(CR.title, sizeof(char), TitleLen, pFile); //read in title
   for(i = ZERO; i < TitleLen; i++){
      CR.title[i] ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //update title in enum
   }

   //loop starts here
   while(1){ //loop to go through versions
      RequiredLen = fgetc(pFile); //get number of requirements
      RequiredLen ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //decrpyt with xor

      if(RequiredLen != ZERO){ //if items required is 0 just display text
         fread(CR.Requireditems, sizeof(char), RequiredLen, pFile); //read in required items 
         for(i = ZERO; i < RequiredLen; i++){
            CR.Requireditems[i] ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //update requirements in enum
         }

         for(i = ZERO; i < RequiredLen; i++){ //search inventory for items required 
            int result = FindInInventory(CR.Requireditems[i]);
            if(result == STANDARD_ERROR){
               flag = ON; //set flag to 1 if you dont have necessary items in inventory
            }
         }
      }

      if(flag == ON){ //increment pointer to next version
         flag = ZERO; //reset flag
         DescLen = fgetc(pFile); //get description length 
         DescLen ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //decrpyt with xor

         fread(CR.desctrash, sizeof(char), DescLen, pFile); //read in description to waste 
         for(i = ZERO; i < DescLen; i++){
            CR.desctrash[i] ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //update description in waste
         }

         ItemGivenLen = fgetc(pFile); //get length of given items
         ItemGivenLen ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //decrpyt with xor       

         if(ItemGivenLen != ZERO){
            fread(CR.itemsgiventrash, sizeof(char), ItemGivenLen, pFile); //put items in trash
            for(i = ZERO; i < ItemGivenLen; i++){
               CR.itemsgiventrash[i] ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //update items given in enum
            }
         }

         //increment file pointer past exits 
         ExitTrash = fgetc(pFile);
         ExitTrash = fgetc(pFile);
         ExitTrash = fgetc(pFile);
         ExitTrash = fgetc(pFile);

      }else{ //display version and exit 
         DescLen = fgetc(pFile); //get description length 
         DescLen ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //decrpyt with xor

         fread(CR.description, sizeof(char), DescLen, pFile); //read in description to enum
         for(i = ZERO; i < DescLen; i++){
            CR.description[i] ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //update description in enum
         }

         ItemGivenLen = fgetc(pFile); //get length of given items
         ItemGivenLen ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //decrpyt with xor
         if(ItemGivenLen != ZERO){
            fread(CR.InventoryBuffer, sizeof(char), ItemGivenLen, pFile); //put items in inventoryBuffer
            for(i = ZERO; i < ItemGivenLen; i++){
               CR.InventoryBuffer[i] ^= (DECRYPTION_BASE_KEY + STARTING_ROOM); //update items given in enum
               AddToInventory(CR.InventoryBuffer[i]); //put items in inventory           
            }
         }

         for(i = ZERO; i < INVENTORY_SIZE; i++){
            CR.exits[i] = fgetc(pFile);
            CR.exits[i] ^= (DECRYPTION_BASE_KEY + STARTING_ROOM);
         }
         fclose(pFile);
         return SUCCESS;
      }
   }
   fclose(pFile);
   return STANDARD_ERROR;
}

/**
 * Copies the current room title as a NULL-terminated string into the provided character array.
 * Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title){
   sprintf(title, "%s", CR.title); //null terminated string
   int temp = strlen(title);
   if(title == NULL){
      title = NLL;
      return ZERO;
   }else{
      return temp; //actual length is temp + 1
   }
}


/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc){
   sprintf(desc, "%s", CR.description); //copy description as a string
   int temp = strlen(desc);
   if(desc == NULL){
      desc = NLL;
      return ZERO;
   }else{
      return temp; //actualy value is temp + 1
   }
}

/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
uint8_t GameGetCurrentRoomExits(void){
   uint8_t total = ZERO;
   if(CR.exits[FIRST_EXIT]){ //if true then a west exit exists 
      total ^= GAME_ROOM_EXIT_NORTH_EXISTS;
   }
   if(CR.exits[SECOND_EXIT]){
      total ^= GAME_ROOM_EXIT_EAST_EXISTS;
   }
   if(CR.exits[THIRD_EXIT]){
      total ^= GAME_ROOM_EXIT_SOUTH_EXISTS;
   }
   if(CR.exits[FOURTH_EXIT]){
      total ^= GAME_ROOM_EXIT_WEST_EXISTS;
   }
   return total;
}

int Update(void){
   TitleLen = fgetc(pFile); //get first number - title length
   TitleLen ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //decrpyt with xor

   fread(CR.title, sizeof(char), TitleLen, pFile); //read in title
   for(i = ZERO; i < TitleLen; i++){
      CR.title[i] ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //update title in enum
   }

   while(1){ //loop to go through versions
      RequiredLen = fgetc(pFile); //get number of requirements
      RequiredLen ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //decrpyt with xor

      if(RequiredLen != ZERO){ //if items required is 0 just display text
         fread(CR.Requireditems, sizeof(char), RequiredLen, pFile); //read in required items 
         for(i = ZERO; i < RequiredLen; i++){
           CR.Requireditems[i] ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //update requirements in enum
         }

         for(i = ZERO; i < RequiredLen; i++){ //search inventory for items required 
            int result = FindInInventory(CR.Requireditems[i]);
            if(result == STANDARD_ERROR){
               flag = ON; //set flag to 1 if you dont have necessary items in inventory
            }
         }
      }

      if(flag == ON){ //increment pointer to next version

         flag = ZERO; //reset flag
         DescLen = fgetc(pFile); //get description length 
         DescLen ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //decrpyt with xor

         fread(CR.desctrash, sizeof(char), DescLen, pFile); //read in description to waste 
         for(i = ZERO; i < DescLen; i++){
            CR.desctrash[i] ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //update description in waste
         }

         ItemGivenLen = fgetc(pFile); //get length of given items
         ItemGivenLen ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //decrpyt with xor      

         if(ItemGivenLen != ZERO){
            fread(CR.itemsgiventrash, sizeof(char), ItemGivenLen, pFile); //put items in trash
            for(i = ZERO; i < ItemGivenLen; i++){
               CR.itemsgiventrash[i] ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //update items given in enum
            }
         }

         //increment file pointer past exits 
         ExitTrash = fgetc(pFile);
         ExitTrash = fgetc(pFile);
         ExitTrash = fgetc(pFile);
         ExitTrash = fgetc(pFile);

      }else{ //display version and exit 
         DescLen = fgetc(pFile); //get description length 
         DescLen ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //decrpyt with xor

         fread(CR.description, sizeof(char), DescLen, pFile); //read in description to enum
         for(i = ZERO; i < DescLen; i++){
            CR.description[i] ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //update description in enum
         }

         ItemGivenLen = fgetc(pFile); //get length of given items
         ItemGivenLen ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //decrpyt with xor

         if(ItemGivenLen != ZERO){
            fread(CR.InventoryBuffer, sizeof(char), ItemGivenLen, pFile); //put items in inventoryBuffer
            for(i = ZERO; i < ItemGivenLen; i++){
               CR.InventoryBuffer[i] ^= (DECRYPTION_BASE_KEY + CR.RoomNum); //update items given in enum
               AddToInventory(CR.InventoryBuffer[i]); //put items in inventory
            }
         }

         for(i = ZERO; i < INVENTORY_SIZE; i++){
            CR.exits[i] = fgetc(pFile);
            CR.exits[i] ^= (DECRYPTION_BASE_KEY + CR.RoomNum);
         }
         fclose(pFile);
         return SUCCESS;
      }
   }
}

