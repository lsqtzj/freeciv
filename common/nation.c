/********************************************************************** 
 Freeciv - Copyright (C) 1996 - A Kjeldberg, L Gregersen, P Unold
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
***********************************************************************/

/**********************************************************************
   Functions for handling the nations.
***********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "game.h"
#include "government.h"
#include "log.h"
#include "mem.h"
#include "player.h"
#include "tech.h"

#include "nation.h"

struct nation_type *nations = NULL;

/***************************************************************
  Returns 1 if nid is a valid nation id, else 0.
  If returning 0, prints log message with given loglevel
  quoting given func name, explaining problem.
***************************************************************/
static int bounds_check_nation_id(Nation_Type_id nid, int loglevel,
				  const char *func_name)
{
  if (game.nation_count==0) {
    freelog(loglevel, "%s before nations setup", func_name);
    return 0;
  }
  if (nid < 0 || nid >= game.nation_count) {
    freelog(loglevel, "Bad nation id %d (count %d) in %s",
	    nid, game.nation_count, func_name);
    return 0;
  }
  return 1;
}

/***************************************************************
...
***************************************************************/
Nation_Type_id find_nation_by_name(char *name)
{
  int i;

  for(i=0; i<game.nation_count; i++)
     if(!mystrcasecmp(name, get_nation_name (i)))
	return i;

  return -1;
}

/***************************************************************
Returns name of the nation
***************************************************************/
char *get_nation_name(Nation_Type_id nation)
{
  if (!bounds_check_nation_id(nation, LOG_NORMAL, "get_nation_name")) {
    return "";
  }
  return nations[nation].name;
}

/***************************************************************
Returns pointer to the array of the nation leader names, and
sets dim to number of leaders.
***************************************************************/
char **get_nation_leader_names(Nation_Type_id nation, int *dim)
{
  if (!bounds_check_nation_id(nation, LOG_FATAL, "get_nation_leader_names")) {
    exit(1);
  }
  *dim = nations[nation].leader_count;
  return nations[nation].leader_name;
}

/***************************************************************
Returns sex of given leader name. If names is not found,
return 1 (meaning male).
***************************************************************/
int get_nation_leader_sex(Nation_Type_id nation, char *name)
{
  int i;
  
  if (!bounds_check_nation_id(nation, LOG_NORMAL, "get_nation_leader_sex")) {
    return 0;
  }
  for( i=0; i < nations[nation].leader_count; i++ ) {
    if( !strcmp(nations[nation].leader_name[i],name) )
      break;
  }
  if( i <  nations[nation].leader_count )
    return nations[nation].leader_is_male[i];
  else
    return 1;
}

/***************************************************************
checks if given leader name exist for given nation.
***************************************************************/
int check_nation_leader_name(Nation_Type_id nation, char *name)
{
  int i, found=0;
  
  if (!bounds_check_nation_id(nation, LOG_NORMAL, "check_nation_leader_name")) {
    return 1;			/* ? */
  }
  for( i=0; i<nations[nation].leader_count; i++) {
    if( !strcmp(name, nations[nation].leader_name[i]) )
      found = 1;
  }
  return found;
}

/***************************************************************
Returns plural name of the nation.
***************************************************************/
char *get_nation_name_plural(Nation_Type_id nation)
{
  if (!bounds_check_nation_id(nation, LOG_NORMAL, "get_nation_name_plural")) {
    return "";
  }
  return nations[nation].name_plural;
}

/***************************************************************
Returns pointer to a nation 
***************************************************************/
struct nation_type *get_nation_by_plr(struct player *plr)
{
  assert(plr);
  if (!bounds_check_nation_id(plr->nation, LOG_FATAL, "get_nation_by_plr")) {
    exit(1);
  }
  return &nations[plr->nation];
}

struct nation_type *get_nation_by_idx(Nation_Type_id nation)
{
  if (!bounds_check_nation_id(nation, LOG_FATAL, "get_nation_by_idx")) {
    exit(1);
  }
  return &nations[nation];
}

/***************************************************************
Returns pointer to a nation 
***************************************************************/
struct nation_type *alloc_nations(int num)
{
  nations = fc_calloc( num, sizeof(struct nation_type) );
  return nations;
}

void free_nations(int num)
{
  int i, j;

  if(!nations) return;
  for( i = 0; i < num; i++) {
    for( j = 0; j < nations[i].leader_count; j++) {
      free( nations[i].leader_name[j] );
    }
  }
  free(nations);
}

