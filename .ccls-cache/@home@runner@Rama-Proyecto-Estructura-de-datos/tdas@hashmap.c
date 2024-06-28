#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    MapPair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

MapPair * createPair( char * key,  void * value) {
    MapPair * new = (MapPair *)malloc(sizeof(MapPair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2) {
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void map_insert(HashMap * map, char * key, void * value) 
{
  long posicion = hash(key, map->capacity);
  long original = posicion;

  while(map->buckets[posicion]!= NULL && map->buckets[posicion]->key != NULL)
    {
      if(strcmp(map->buckets[posicion]->key, key) == 0)
      {
        return;
      }
      posicion = (posicion + 1) % map->capacity;
      if(posicion == original)
      {
        return;
      } 
    }
  map->buckets[posicion] = createPair(key,value);
  map->current = posicion;

  map->size++;
}

void enlarge(HashMap * map) 
{
  enlarge_called = 1; //no borrar (testing purposes)
  MapPair** old = map->buckets;

  map->capacity *= 2;
  map->buckets = (MapPair**)malloc(map->capacity * sizeof(MapPair*));
  map->size = 0;

  for(long i = 0; i < map->capacity / 2; i++)
  {
    if(old[i] != NULL)
    {
      map_insert(map, old[i]->key, old[i]->value);
      free(old[i]);
    }
  }
  free(old);
}



HashMap * map_create(long capacity) 
{
  HashMap *map = (HashMap*)malloc(sizeof(HashMap));
  if(map == NULL)
  {
    free(map);
    return NULL;
  }

  map->buckets = (MapPair**)calloc(capacity, sizeof(MapPair*));
  if(map->buckets == NULL)
  {
    free(map);
    return NULL;
  }
  
  map->size = 0;
  map->current = -1;
  map->capacity = capacity;
  return map;
}

void map_remove(HashMap * map,  char * key)
{
  long posElem = hash(key, map->capacity);
  long posOrig = posElem;

  while(map->buckets[posElem]!=NULL && map->buckets[posElem]->key != NULL)
  {
    if(strcmp(map->buckets[posElem]->key, key) == 0)
    {
      map->buckets[posElem]->key = NULL;
      map->size--;
    }
      posElem = (posElem +1) % map->capacity;
    if(posElem == posOrig)
    {
      return;
    }
  }
  return;
}


MapPair * map_search(HashMap * map,  char * key) 
{   
  long posElem = hash(key, map->capacity);
  long posOrig = posElem;

  while(map->buckets[posElem] != NULL && map->buckets[posElem]->key != NULL)
    {
      if(strcmp(map->buckets[posElem]->key, key) == 0)
      {
        map->current = posElem;
        return map->buckets[posElem];
      }
      posElem = (posElem + 1) % map->capacity;
      if(posElem == posOrig)
      {
        return NULL;
      }
    }
  return NULL;
}

MapPair * marp_first(HashMap * map) 
{
  for(long i = 0; i < map->capacity; i++)
    {
      if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)
      {
        map->current = i;
        return map->buckets[i];
      }    
    }
    return NULL;
}

MapPair * map_next(HashMap * map) 
{
  for(long i = map->current + 1; i < map->capacity; i++)
  {
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)
    {
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;

}
