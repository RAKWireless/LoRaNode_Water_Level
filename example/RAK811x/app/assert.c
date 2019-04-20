/*
 *  Copyright 2008-2015, RAKWireless Ltd.
 *  All Rights Reserved.
 */

/** assert.c: Assert Functions
 */
#include "app.h"

void _assert(const char *filename, int lineno, 
	    const char* fail_cond)
{
    e_printf("\n\n\r*************** PANIC *************\n\r");
    e_printf("Filename  : %s ( %d )\n\r", filename, lineno);
    e_printf("Condition : %s\n\r", fail_cond);
    e_printf("***********************************\n\r");
    for( ;; );
}
