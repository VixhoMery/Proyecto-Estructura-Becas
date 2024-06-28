#include "extra.h"
#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 500

char **leer_linea_csv(FILE *archivo, char separador) {
  static char linea[MAX_LINE_LENGTH]; 
  static char *campos[MAX_FIELDS];
  char *ptr, *start;
  int index = 0;

  if (fgets(linea, MAX_LINE_LENGTH, archivo) == NULL){
    return NULL; //No hay más líneas
  }

  //Eliminar salto de línea
  linea[strcspn(linea, "\n")] = '\0';

  ptr = start = linea;
  while(*ptr){
    if (index >= MAX_FIELDS - 1) break;

    start = ptr;
    while(*ptr && *ptr != separador) 
      ptr++;

    if(*ptr){
      *ptr = '\0'; 
      ptr++;
    }
    campos[index++] = start;
  }

  campos[index] = NULL; // Fin del arreglo
  return campos;
}