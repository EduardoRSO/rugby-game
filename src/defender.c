// Standard headers
#include <stdio.h>

// Internal headers
#include "direction.h"
#include "position.h"
#include "spy.h"

// Main header
#include "defender.h"

// New headers
#include <stdlib.h>
#include <time.h>

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

// Private Variables
static bool going_top = true;
static bool going_left = true;
static bool tried_left = false;
static bool backtracking = false;
static position_t defender_last_position;

// Private Functions

bool defender_not_moved_to_position(position_t last, position_t actual){
        return equal_positions(actual, last);
}

void change_defender_direction(){
	if(going_top) going_top = false;
	else going_top = true;
}

direction_t choose_defender_slide(){
	if(going_left) return (direction_t) DIR_LEFT;
	return (direction_t) DIR_RIGHT;
}

direction_t choose_defender_direction(){
	if(going_top) return (direction_t) DIR_UP;
	return (direction_t) DIR_DOWN;
}

direction_t attacker_closest_position(position_t attacker, position_t defender){
	going_left = false;
	if(attacker.i > defender.i){
		if(!backtracking && going_top) going_top = false;
	}
	else{
		if(!backtracking && !going_top) going_top = true;	
	}
	return choose_defender_direction();
}

void force_defender_to_left_direction(position_t defender_position){
	direction_t temp = choose_defender_direction();
	defender_last_position.i = defender_position.i + temp.i;
	defender_last_position.j = defender_position.j + temp.j;
}


/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_defender_strategy(position_t defender_position, Spy attacker_spy) {
	//printf("DEFENDER: (%ld, %ld), (%ld, %ld), going_top: %d, tried_left: %d, going_left: %d\n",defender_last_position.i, defender_last_position.j, defender_position.i, defender_position.j, going_top, tried_left, going_left);
	srand(time(NULL));
	if(rand() % 3 == 1 && !get_spy_number_uses(attacker_spy)){
		position_t attacker_position = get_spy_position(attacker_spy);
		return attacker_closest_position(attacker_position, defender_position);
	}

	if(defender_not_moved_to_position(defender_last_position, defender_position)){
		defender_last_position = defender_position;
		if(tried_left){
			force_defender_to_left_direction(defender_position);
			backtracking = true;
			tried_left = false;
			return choose_defender_direction();
		}
		if(!backtracking) change_defender_direction();
		tried_left = true;
		return choose_defender_slide();
	}else{
		defender_last_position = defender_position;
		backtracking = false;
		tried_left = false;
		return choose_defender_direction();
	}
}

/*----------------------------------------------------------------------------*/
