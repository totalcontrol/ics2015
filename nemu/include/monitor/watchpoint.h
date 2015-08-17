#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	char expr[255];//watch expression
	int value;  //old expression value 
	int used;
	
	
	/* TODO: Add more members if necessary */


} WP;
void free_wp(int);
WP* new_wp();

WP* get_wp_head();





#endif
