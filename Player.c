//Shant Narkizian
//no collaboration
//player.c

#include <stdint.h>
#include "Player.h"
#define VALID_MAX 255
#define VALID_MIN 0
#define RESET 0
#define INCREMENT 1

static int inventory[INVENTORY_SIZE];

/**
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERRROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item){
   static int index = RESET; //start at 0
   if(index < INVENTORY_SIZE){
      if(item >= VALID_MIN && item <= VALID_MAX){ //means its in range
         inventory[index] = item;
         int i;
         for(i = index + INCREMENT; i<INVENTORY_SIZE; i++){
            inventory[i] = RESET; //set remaining values to 0
         }
         index++;  //increment counter for next item
         return SUCCESS;
      }
   }
   return STANDARD_ERROR;
}


/**
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item){
   if(item < VALID_MIN && VALID_MAX < item){ //not in range
      return STANDARD_ERROR;
   }

   int i;
   for(i = RESET; i < INVENTORY_SIZE; i++){  //goes through inventory
      if(inventory[i] == item){
         return SUCCESS;
      }
   }
   return STANDARD_ERROR; //if not in inventory
}
