//
//  HashMap.h
//  TablaHashLab
//
//  Created by Matias Barrientos on 11-08-18.
//  Copyright © 2018 Matias Barrientos. All rights reserved.
//

#ifndef HashMap_h
#define HashMap_h


typedef struct HashMap HashMap;

typedef struct MapPair {
     char * key;
     void * value;
} MapPair;

HashMap * map_create(long capacity);

void map_insert(HashMap * table, char * key, void * value);

void map_remove(HashMap * table, char * key);

MapPair * map_search(HashMap * table, char * key);

MapPair * marp_first(HashMap * table);

MapPair * map_next(HashMap * table);

void enlarge(HashMap * map);


#endif /* HashMap_h */
