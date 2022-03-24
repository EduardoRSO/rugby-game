// Standard headers
#include <stdio.h>

// Internal headers
#include "direction.h"
#include "position.h"
#include "spy.h"

// Main header
#include "attacker.h"

// New headers
#include <stdlib.h>
#include <time.h>

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

// Private Variables

static int states[30] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
       	          8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
static int pointer = 0;
static position_t attacker_last_position;
static bool randomize = true;

static int priority_DIR_UP = 8;
static int priority_DIR_UP_RIGHT = 7;
static int priority_DIR_RIGHT = 6;
static int priority_DIR_DOWN_RIGHT = 5;
static int priority_DIR_DOWN = 4;
static int priority_DIR_DOWN_LEFT = 3;
static int priority_DIR_LEFT = 2;
static int priority_DIR_UP_LEFT = 1;

// Private functions

bool attacker_not_moved_to_position(position_t last, position_t actual){
	return equal_positions(actual, last);
}

direction_t attacker_draw_back(){
	if(pointer >= 0){
		if(states[pointer] ==      priority_DIR_UP){ 		return (direction_t) DIR_DOWN;}
		else if(states[pointer] == priority_DIR_UP_RIGHT){	return (direction_t) DIR_DOWN_LEFT;}
		else if(states[pointer] == priority_DIR_RIGHT){		return (direction_t) DIR_LEFT;}
		else if(states[pointer] == priority_DIR_DOWN_RIGHT){	return (direction_t) DIR_UP_LEFT;}
		else if(states[pointer] == priority_DIR_DOWN){		return (direction_t) DIR_UP;}
		else if(states[pointer] == priority_DIR_DOWN_LEFT){	return (direction_t) DIR_UP_RIGHT;}
		else if(states[pointer] == priority_DIR_LEFT){		return (direction_t) DIR_RIGHT;}
		else if(states[pointer] == priority_DIR_UP_LEFT){  	return (direction_t) DIR_DOWN_RIGHT;}
		else /*this should not happen*/				return (direction_t) DIR_LEFT;
	}
	else{
		/*this should not happen*/
		return (direction_t) DIR_LEFT;
	}
}

direction_t choose_attacker_direction(){
	if(pointer >= 0){
		if(states[pointer] == priority_DIR_UP) 			return (direction_t) DIR_UP;
		else if(states[pointer] == priority_DIR_UP_RIGHT) 	return (direction_t) DIR_UP_RIGHT; 
		else if(states[pointer] == priority_DIR_RIGHT) 		return (direction_t) DIR_RIGHT; 
		else if(states[pointer] == priority_DIR_DOWN_RIGHT) 	return (direction_t) DIR_DOWN_RIGHT; 
		else if(states[pointer] == priority_DIR_DOWN) 		return (direction_t) DIR_DOWN; 
		else if(states[pointer] == priority_DIR_DOWN_LEFT) 	return (direction_t) DIR_DOWN_LEFT; 
		else if(states[pointer] == priority_DIR_LEFT) 		return (direction_t) DIR_LEFT; 
		else if(states[pointer] == priority_DIR_UP_LEFT) 	return (direction_t) DIR_UP_LEFT;
	       	else{
			pointer--;
			return attacker_draw_back();
		}	
	}
		/*this should not happen*/				return (direction_t) DIR_LEFT;
}

void  change_directions_priorities(position_t attacker, position_t defender){
        if(attacker.i > defender.i){
		if(attacker.j > defender.j){
			//RUSH
			priority_DIR_UP = 2;
			priority_DIR_UP_RIGHT = 1;
			priority_DIR_RIGHT = 8;
			priority_DIR_DOWN_RIGHT = 7;
			priority_DIR_DOWN = 6;
			priority_DIR_DOWN_LEFT = 5;
			priority_DIR_LEFT = 4;
			priority_DIR_UP_LEFT = 3;
		}else{
			//DIAGONAL ESCAPE and RUSH
			priority_DIR_UP = 1;
			priority_DIR_UP_RIGHT = 5;
			priority_DIR_RIGHT = 7;
			priority_DIR_DOWN_RIGHT = 6 ;
			priority_DIR_DOWN = 4;
			priority_DIR_DOWN_LEFT = 8;
			priority_DIR_LEFT = 3;
			priority_DIR_UP_LEFT = 2;
		
		}
	} else{
		if(attacker.j > defender.j){
			//RUSH
			priority_DIR_UP = 3;
			priority_DIR_UP_RIGHT = 5;
			priority_DIR_RIGHT = 8;
			priority_DIR_DOWN_RIGHT = 7;
			priority_DIR_DOWN = 4;
			priority_DIR_DOWN_LEFT = 6;
			priority_DIR_LEFT = 1;
			priority_DIR_UP_LEFT = 2;	
		}else{
			//DIAGONAL ESCAPE and RUSH
			priority_DIR_UP = 2;
			priority_DIR_UP_RIGHT = 5 ;
			priority_DIR_RIGHT = 7;
			priority_DIR_DOWN_RIGHT = 6 ;
			priority_DIR_DOWN = 4 ;
			priority_DIR_DOWN_LEFT = 1;
			priority_DIR_LEFT = 3;
			priority_DIR_UP_LEFT = 8;
		
		}
	}
        
}


direction_t randomize_starting_position(position_t last, position_t actual, Spy defender_spy){
	if(attacker_not_moved_to_position(last, actual)){
		attacker_last_position = actual;
		randomize = false;
		change_directions_priorities(actual, get_spy_position(defender_spy));
		return choose_attacker_direction();
	}else{
		srand(time(NULL));
		int random_number = rand() % 2;
		attacker_last_position = actual;
		if(random_number) return (direction_t) DIR_UP_RIGHT;
	       	else		  return (direction_t) DIR_DOWN_RIGHT;
	}
}

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_attacker_strategy(position_t attacker_position, Spy defender_spy) {
//	printf("ATTACKER: (%ld, %ld), (%ld, %ld), pointer: %d, spy %ld, randomize %d\n",attacker_last_position.i, attacker_last_position.j, attacker_position.i, attacker_position.j, pointer, get_spy_number_uses(defender_spy), randomize);
	if(randomize){
		return randomize_starting_position(attacker_last_position, attacker_position, defender_spy);
	}
	if(attacker_not_moved_to_position(attacker_last_position,attacker_position)){
		states[pointer]--;
		return choose_attacker_direction();
	}else{
		pointer++;
		attacker_last_position = attacker_position;
		return choose_attacker_direction();
	}
}

/*----------------------------------------------------------------------------*/
