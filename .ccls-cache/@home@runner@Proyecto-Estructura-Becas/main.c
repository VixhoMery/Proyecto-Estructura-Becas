#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/queue.h"

typedef struct {
    char nombre[50];
    char rut[12];
    char direccion[100];
    char historialAcademico[500];
} Estudiante;

typedef struct {
    char nombre[50];
    char tipo[20];
    float monto;
    char requisitos[200];
} Beca;

typedef struct {
    char rutEstudiante[12];
    char nombreBeca[50];
    char estado[20]; // Estado puede ser "En revisión", "Aprobada", "Rechazada"
} Solicitud;



// Prototipo de funciones
void presioneTeclaParaContinuar();
void limpiarPantalla();
void opcionUsuario(Map *estudiantes, List *becas, Queue *solicitudes);
void opcionAdmin(Map *estudiantes, List *becas, Queue *solicitudes);
int login(int);

// Prototipos submenús
void mostrarMenuAdminAlumno();
void mostrarMenuAlumno();
void mostrarMenuAdmin();


// Prototipos funciones de usuario
void completarPerfil(Map *estudiantes);
void postularBeca(Map *estudiantes, List *becas, Queue *solicitudes);
void seguimientoPostulacion(Queue *solicitudes);

// Prototipos funciones de administrador
void gestionarEstudiantes(Map *estudiantes);
void revisarSolicitudes(Queue *solicitudes);
void gestionarBecas(List *becas);
void aprobarRechazarSolicitudes(Queue *solicitudes);
void seguimientoBecas(List *becas);

// Función para limpiar la pantalla
void limpiarPantalla() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

//Función presione tecla para Continuar
void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar......");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

void mostrarMenuAdminAlumno(){
  puts("===================================");
  puts(" Bienvenido al Programa de Gestión");
  puts("           de Becas");
  puts("====================================");
  puts("1) Alumno");
  puts("2) Administrador");
  puts("3) Salir");
}

void mostrarMenuAlumno(){
  puts("1) Ingresar o Completar perfil");
  puts("2) Postular a Beca");
  puts("3) Seguimiento de Postulación");
  puts("4) Volver");
}

void mostrarMenuAdmin(){
  puts("1) Gestión de Estudiantes");
  puts("2) Revisar solicitudes");
  puts("3) Gestión de Becas");
  puts("4) Gestionar el proceso de selección y aprobación de becas");
  puts("5) Seguimiento de Becas");
  puts("6) Volver");
}


int main() {
  Map *estudiantes = map_create((int (*)(void*, void*))strcmp);
  List *becas = list_create();
  Queue *solicitudes = queue_create(NULL);
  //Lógica de lectura de archivo, antes de la ejecución del menú

  int opcion;
  int resultadoAdmin, n;
  do {
    mostrarMenuAdminAlumno();
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);
    switch(opcion) {
      case 1:
        opcionUsuario(estudiantes, becas, solicitudes);
        break;
      case 2:
        resultadoAdmin = login(n);
        if (resultadoAdmin == 1) {
          opcionAdmin(estudiantes, becas, solicitudes);
        } else {
          limpiarPantalla();
          puts("Contraseña incorrecta");
        }
        break;
      case 3:
        puts("Saliendo del programa...");
        break;
      default:
        puts("Opción no válida, intente de nuevo.");
    } 
  } while(opcion != 3);
  presioneTeclaParaContinuar();
  limpiarPantalla();
  return 0;
  }

void opcionUsuario(Map *estudiantes, List *becas, Queue *solicitudes) {
  int opcion;

  do {
    limpiarPantalla();
    mostrarMenuAlumno();
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);
    switch(opcion) {
      case 1:
        completarPerfil(estudiantes);
        break;
      case 2:
        postularBeca(estudiantes, becas, solicitudes);
        break;
      case 3:
        seguimientoPostulacion(solicitudes);
        break;
      case 4:
        puts("Volviendo al menú principal...");
        break;
      default:
        printf("Opción no válida, intente de nuevo.\n");
    }
    presioneTeclaParaContinuar();
  } while(opcion != 4);
  limpiarPantalla();
}

void opcionAdmin(Map *estudiantes, List *becas, Queue *solicitudes) {
  int opcion;
  do {
    limpiarPantalla();
    mostrarMenuAdmin();
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);
    switch(opcion) {
      case 1:
        gestionarEstudiantes(estudiantes);
        break;
      case 2:
        revisarSolicitudes(solicitudes);
        break;
      case 3:
        gestionarBecas(becas);
        break;
      case 4:
        aprobarRechazarSolicitudes(solicitudes);
        break;
      case 5:
        seguimientoBecas(becas);
        break;
      case 6:
        puts("Volviendo al menú principal...");
        break;
      default:
        printf("Opción no válida, intente de nuevo.\n");
    }
    presioneTeclaParaContinuar();
  } while(opcion != 6);
  limpiarPantalla();
}

int login(int n) {
  char contrasena[20];

  printf("Contraseña: ");
  scanf("%s", contrasena);
  if (strcmp(contrasena, "admin") == 0) {
    n = 1;
  } else {
    n = 0;
  }
  return n;
}


// Funciones para las funcionalidades de estudiante
void completarPerfil(Map *estudiantes) {
  Estudiante *nuevoEstudiante = malloc(sizeof(Estudiante));
  printf("Ingrese su nombre: ");
  scanf(" %[^\n]", nuevoEstudiante->nombre);
  printf("Ingrese su RUT: ");
  scanf(" %s", nuevoEstudiante->rut);
  printf("Ingrese su dirección: ");
  scanf(" %[^\n]", nuevoEstudiante->direccion);
  printf("Ingrese su historial académico: ");
  scanf(" %[^\n]", nuevoEstudiante->historialAcademico);

  map_insert(estudiantes, nuevoEstudiante->rut, nuevoEstudiante);
  printf("Perfil completado exitosamente.\n");
}

void postularBeca(Map *estudiantes, List *becas, Queue *solicitudes) {
  Solicitud *nuevaSolicitud = malloc(sizeof(Solicitud));
  char rut[12];
  printf("Ingrese su RUT: ");
  scanf(" %s", rut);

  if (map_search(estudiantes, rut) != NULL) {
    printf("Ingrese el nombre de la beca a la que desea postular: ");
    scanf(" %[^\n]", nuevaSolicitud->nombreBeca);
    strcpy(nuevaSolicitud->rutEstudiante, rut);
    strcpy(nuevaSolicitud->estado, "En revisión");

    queue_insert(solicitudes, nuevaSolicitud);
    printf("Postulación realizada exitosamente.\n");
  } else {
    printf("Estudiante no encontrado.\n");
  }
}

void seguimientoPostulacion(Queue *solicitudes) {
  char rut[12];
  printf("Ingrese su RUT: ");
  scanf(" %s", rut);

  Queue *tempQueue = queue_create(NULL);
  int encontrado = 0;
  Solicitud *solicitud;
  while ((solicitud = queue_remove(solicitudes)) != NULL) {
    if (strcmp(solicitud->rutEstudiante, rut) == 0) {
      printf("Solicitud a Beca: %s - Estado: %s\n", solicitud->nombreBeca, solicitud->estado);
      encontrado = 1;
    }
    queue_insert(tempQueue, solicitud);
  }

  while ((solicitud = queue_remove(tempQueue)) != NULL) {
    queue_insert(solicitudes, solicitud);
  }

  if (!encontrado) {
    printf("No se encontraron solicitudes.\n");
  }
  queue_clean(tempQueue);
}

// Funciones para las funcionalidades de administrador
void gestionarEstudiantes(Map *estudiantes) {
  // Similar a completarPerfil, pero con opciones para agregar, actualizar y eliminar
  // Aquí debe implementar la lógica para manejar estudiantes
}

void revisarSolicitudes(Queue *solicitudes) {
  // Recorre la cola y revisa cada solicitud
  // Aquí debe implementar la lógica para revisar solicitudes
}

void gestionarBecas(List *becas) {
  // Permite agregar, actualizar o eliminar becas
  // Aquí debe implementar la lógica para gestionar becas
}

void aprobarRechazarSolicitudes(Queue *solicitudes) {
  // Permite aprobar o rechazar solicitudes en la cola
  // Aquí debe implementar la lógica para aprobar/rechazar solicitudes
}

void seguimientoBecas(List *becas) {
  // Realiza un seguimiento de las becas
  // Aquí debe implementar la lógica para seguimiento de becas
}