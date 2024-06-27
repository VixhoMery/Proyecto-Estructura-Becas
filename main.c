#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extra_y_data/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/queue.h"

typedef struct {
    char nombreEstudiante[50];
    char rut[12];
    int socioEconomico;
    int puntaje;
    int notasEM;
    int discapacidad;
    int originario;
} Usuario;

typedef struct {
  char nombre[100];
  int socioEconomico;
  int puntaje;
  int notasEM;
  int discapacidad;
  int originario;
} tipoBeca;

typedef struct {
    char rutEstudiante[12];
    char nombreBeca[50];
    char estado[20]; // Estado puede ser "En revisión", "Aprobada", "Rechazada"
} Solicitud;



// Prototipo de funciones
void presioneTeclaParaContinuar();
void limpiarPantalla();
void opcionUsuario(Map *estudiantes, List *becas, Queue *apelaciones);
void opcionAdmin(Map *estudiantes, List *becas, Queue *apelaciones);
int login(int);
void mostrarLogo();
// Prototipos submenús
void mostrarMenuAdminAlumno();
void mostrarMenuAlumno();
void mostrarMenuAdmin();

//Prototipos funciones de inicialización y extras
void inicializarBecas(List* becas);
void mostrarBeca(tipoBeca*, Usuario*);

// Prototipos funciones de usuario
void completarPerfil(Map *estudiantes);
void postularBeca(Map *estudiantes, List *becas, Queue *solicitudes);
void seguimientoApelaciones(Queue *apelaciones);

// Prototipos funciones de administrador
void gestionarEstudiantes(Map *estudiantes);
void revisarApelaciones(Queue *apelaciones);
void gestionarBecas(List *becas);
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
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

void mostrarMenuAdminAlumno(){
  mostrarLogo();
  puts("Seleccione el método de ingreso:");
  puts("1) Alumno");
  puts("2) Administrador");
  puts("3) Salir");
}

void mostrarMenuAlumno(){
  printf("=========================================\n");
  printf("|             MENU ALUMNO               |\n");
  printf("=========================================\n");
  puts("Seleccione una opción:");
  puts("1) Ingresar o Completar perfil");
  puts("2) Postular a Beca");
  puts("3) Seguimiento de apelación");
  puts("4) Volver");
}

void mostrarMenuAdmin(){
  printf("=========================================\n");
  printf("|             MENU ADMINISTRADOR        |\n");
  printf("=========================================\n");
  puts("1) Gestión de Estudiantes");
  puts("2) Revisar solicitudes de apelación");
  puts("3) Gestión de Becas");
  puts("4) Seguimiento de Becas");
  puts("5) Volver");
}

void mostrarLogo() {
    printf("==========================================================\n");
    printf(" ____                   _____               _    \n");
    printf("| __ )  ___  ___ __ _  |_   _| __ __ _  ___| | __\n");
    printf("|  _ \\ / _ \\/ __/ _` |   | || '__/ _` |/ __| |/ /\n");
    printf("| |_) |  __/ (_| (_| |   | || | | (_| | (__|   < \n");
    printf("|____/ \\___|\\___\\__,_|   |_||_|  \\__,_|\\___|_|\\_\\\n");
    printf("                                                 \n");
    printf("==========================================================\n");
    printf("       Bienvenido a BECA TRACK\n");
    printf("==========================================================\n\n");
}

int rut_equal(void *key1, void *key2){
  return strcmp((char*)key1, (char*)key2) ==0;
}

int main() {
  
  Map *estudiantes = map_create(rut_equal);
  List *becas = list_create();
  Queue *apelaciones = queue_create(NULL);
  inicializarBecas(becas);
  
  int opcion;
  int resultadoAdmin, n;
  do {
    mostrarMenuAdminAlumno();
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);
    switch(opcion) {
      case 1:
        opcionUsuario(estudiantes, becas, apelaciones);
        break;
      case 2:
        resultadoAdmin = login(n);
        if (resultadoAdmin == 1) {
          opcionAdmin(estudiantes, becas, apelaciones);
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

//Funcion para inicializar becas
void inicializarBecas(List *listaBecas) {
  FILE *archivo = fopen("extra_y_data/Archivo_CSV_Becas.csv", "r");
  if (archivo == NULL){
    perror("Error al abrir el archivo");
    exit(EXIT_FAILURE);
  }
    
  char **campos;
  while ((campos = leer_linea_csv(archivo, ',')) != NULL)
  {
    tipoBeca *beca = malloc(sizeof(tipoBeca)); 
    if(beca == NULL)
    {
      printf("Error al asignar memoria para la beca\n");
      break;
    }
    
    strcpy(beca->nombre,campos[0]);
    beca->socioEconomico = atoi(campos[1]);
    beca->puntaje = atoi(campos[2]);
    beca->notasEM = atoi(campos[3]);
    beca->discapacidad = atoi(campos[4]);
    beca->originario = atoi(campos[5]);
  
    list_pushBack(listaBecas, beca);
  }
}

void opcionUsuario(Map *estudiantes, List *becas, Queue *apelaciones) {
  int opcion;

  do {
    limpiarPantalla();
    mostrarMenuAlumno();
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);
    switch(opcion) {
      case 1:
        limpiarPantalla();
        completarPerfil(estudiantes);
        break;
      case 2:
        limpiarPantalla();
        postularBeca(estudiantes, becas, apelaciones);
        break;
      case 3:
        limpiarPantalla();
        seguimientoApelaciones(apelaciones);
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
        limpiarPantalla();
        gestionarEstudiantes(estudiantes);
        break;
      case 2:
        limpiarPantalla();
        revisarApelaciones(solicitudes);
        break;
      case 3:
        limpiarPantalla();
        gestionarBecas(becas);
        break;
      case 4:
        limpiarPantalla();
        seguimientoBecas(becas);
        break;
      case 5:
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
void completarPerfil(Map *estudiantes)
{
  printf("INGRESAR O COMPLETAR PERFIL\n\n");

  
  //Solicitar un Rut primero para verificar duplicados.
  char rut[12];
  printf("Ingrese su Rut: ");
  scanf(" %s", rut);

  //Verificar si el estudiante existe en el mapa.
  if(map_search(estudiantes, rut) != NULL)
  {
    printf("El estudiante ya se encuentra registrado.\n");
    return;
  }


  //Reservar memoria para un nuevo estudiante
  Usuario *nuevoEstudiante = malloc(sizeof(Usuario));
  if(nuevoEstudiante == NULL){
    printf("Error al reservar memoria para el estudiante.\n");
    return;
  }
  
  strcpy(nuevoEstudiante->rut, rut);
  //Completacion de los datos del nuevo estudiante.
  char discapacidad[10];
  char puebloOriginario[10];
  
  printf("Ingrese su nombre: ");
  scanf(" %[^\n]", nuevoEstudiante->nombreEstudiante);
  printf("Ingrese su nivel socioeconómico: ");
  scanf("%d", &nuevoEstudiante->socioEconomico);
  printf("Ingrese su puntaje ponderado obtenido en la prueba PAES: ");
  scanf("%d", &nuevoEstudiante->puntaje);
  printf("Ingrese su puntaje NEM: ");
  scanf("%d", &nuevoEstudiante->notasEM);
  printf("¿Posee usted alguna discapacidad?: ");
  scanf(" %s", discapacidad);
  printf("¿Pertenece usted a un pueblo originario?: ");
  scanf(" %s", puebloOriginario);

  //Asignar valores a las variables de discapacidad y originario.
  if(strcmp(discapacidad, "si") == 0)
  {
    nuevoEstudiante->discapacidad = 1;
  }
  else
  {
    nuevoEstudiante->discapacidad = 0;
  }

  if(strcmp(puebloOriginario, "si") == 0)
  {
    nuevoEstudiante->originario = 1;
  }
  else
  {
    nuevoEstudiante->originario = 0;
  }

  // Mostrar los datos ingresados y pedir confirmación
  printf("\nPor favor, confirme los datos ingresados:\n");
  printf("RUT: %s\n", nuevoEstudiante->rut);
  printf("Nombre: %s\n", nuevoEstudiante->nombreEstudiante);
  printf("Nivel Socioeconómico: %d\n", nuevoEstudiante->socioEconomico);
  printf("Puntaje PAES ponderado: %d\n", nuevoEstudiante->puntaje);
  printf("Puntaje NEM: %d\n", nuevoEstudiante->notasEM);
  printf("Discapacidad: %s\n", nuevoEstudiante->discapacidad ? "Sí" : "No");
  printf("Pueblo Originario: %s\n", nuevoEstudiante->originario ? "Sí" : "No");

  char confirmacion[3];
  printf("¿Son correctos estos datos? (si/no): ");
  scanf(" %s", confirmacion);

  if (strcmp(confirmacion, "si") == 0) {
    // Insertar el nuevo estudiante en el mapa.
    map_insert(estudiantes, nuevoEstudiante->rut, nuevoEstudiante);
    printf("Perfil completado exitosamente.\n");
  } else {
    // Liberar la memoria reservada si los datos no son confirmados
    free(nuevoEstudiante);
    printf("Ingreso de datos cancelado.\n");
  }
}

void postularBeca(Map *estudiantes, List *becas, Queue *apelaciones)
{
  printf("=========================================\n");
  printf("|             MENU ALUMNO               |\n");
  printf("=========================================\n");
  printf("POSTULAR A BECA\n\n");
  
  //Solicitar rut del estudiante.
  char rut[12];
  printf("Ingrese su RUT: ");
  scanf(" %[^\n]s", rut);

  //Verificar si el estudiante existe en el mapa
  MapPair * pairEstudiante = map_search(estudiantes, rut);
  if(pairEstudiante == NULL) {
    printf("Error: No se encuentra al estudiante en el sistema.");
    return;
  }
  
  //Comparar datos del estudiante y becas
  Usuario *estudiante = pairEstudiante->value;
  tipoBeca *aux = list_first(becas);
  int requisitoAprobado = 0;
  
  printf("Becas disponibles para %s", estudiante->rut);
  while(aux != NULL){
    requisitoAprobado = 1;
    
    if(estudiante->socioEconomico != aux->socioEconomico)
      requisitoAprobado = 0;
    if(estudiante->puntaje != aux->puntaje)
      requisitoAprobado = 0;
    if(estudiante->notasEM != aux->notasEM)
      requisitoAprobado = 0;
    if(estudiante->discapacidad != aux->discapacidad)
      requisitoAprobado = 0;
    if(estudiante->originario != aux->originario)
      requisitoAprobado = 0;

    if(requisitoAprobado == 1){
      mostrarBeca(aux, estudiante);
    }
    aux = list_next(becas);
  }
  //Opcion para aceptar, rechazar o seleccionar por nombre de la beca
  printf("Seleccione una opción: \n");
  printf("1) Aceptar lista de becas mostradas.\n");
  printf("2) Rechazar lista de becas mostradas.\n");
  printf("3) Seleccionar una beca por nombre.\n");

  int opcion;
  scanf("%d", &opcion);

  switch(opcion)
  {
    case 1:
    {
      //Postular lista de  becas mostradas.
      printf("Ingrese el nombre de la beca a la que desea postular: ");
      char nombreBecaLista[100];
      scanf(" %[^\n]", nombreBecaLista);

      // Verificar si la beca está en la lista de becas disponibles
      tipoBeca *becaDisponible = list_first(becas);
      int encontrada = 0;
      while (becaDisponible != NULL) {
        if (strcmp(becaDisponible->nombre, nombreBecaLista) == 0) {
          //realizarPostulacion(estudiante, becaDisponible, apelaciones);
          encontrada = 1;
          break;
        }
        becaDisponible = list_next(becas);
      }

      if (!encontrada) {
        printf("No se encuentra la beca en la lista de becas disponibles o no cumple con los requisitos.\n");
      }
      break;
        
    }
  } 
}

void mostrarBeca(tipoBeca *beca, Usuario *estudiante){
  printf("Nombre Beca: %s\n", beca->nombre);
  printf("Requisito Socioeconómico: %d\n", beca->socioEconomico);
  printf("Requisito Puntaje Ponderado: %d\n", beca->puntaje);
  printf("Requisito de NEM: %d\n", beca->notasEM);
  if (beca->discapacidad == 0){
    printf("Discapacidad: No\n");
  } else{
    printf("Discapacidad: Sí\n");  
  }

  if(beca->originario == 0){
    printf("Pueblo Originario: No\n");
  } else{
    printf("Pueblo Originario: Si\n");
  }
  printf("\n\n");
}


void seguimientoApelaciones(Queue *solicitudes) {
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
void gestionarEstudiantes(Map *estudiantes)
{
  // Similar a completarPerfil, pero con opciones para agregar, actualizar y eliminar
  int opcion;
  do {
    limpiarPantalla();
    printf("GESTION DE ESTUDIANTES\n\n");
    puts("1) Agregar estudiante al sistema.");
    puts("2) Actualizar datos de un estudiante.");
    puts("3) Eliminar estudiante.");
    puts("4) Volver.");

    printf("Ingrese su opción: ");
    scanf("%d", &opcion);

    switch(opcion)
    {
      case 1:
      {
        // Agregar un estudiante
        limpiarPantalla();
        printf("AGREGAR ESTUDIANTE\n\n");

        char rut[12];
        printf("Ingrese el rut del estudiante a agregar: ");
        scanf(" %s", rut);

        if(map_search(estudiantes, rut) != NULL)
        {
          printf("El estudiante ya se encuentra registrado.\n");
          break;
        }

        Usuario* newEstudiante = malloc(sizeof(Usuario));
        if(newEstudiante == NULL)
        {
          printf("Error al reservar memoria para el estudiante.\n");
          break;
        }
        strcpy(newEstudiante->rut, rut);

        char discapacidad[10];
        char puebloOriginario[10];
        printf("Ingrese el nombre del estudiante a agregar: ");
        scanf(" %[^\n]", newEstudiante->nombreEstudiante);
        printf("Ingrese el nivel socioeconómico del estudiante: ");
        scanf("%d", &newEstudiante->socioEconomico);
        printf("Ingrese el puntaje ponderado obtenido en la prueba PAES: ");
        scanf("%d", &newEstudiante->puntaje);
        printf("Ingrese el puntaje NEM: ");
        scanf("%d", &newEstudiante->notasEM);
        printf("¿El estudiante posee alguna discapacidad? (si/no): ");
        scanf(" %s", discapacidad);
        printf("¿Pertenece a un pueblo originario? (si/no): ");
        scanf(" %s", puebloOriginario);

        // Asignar valores a las variables de discapacidad y originario.
        newEstudiante->discapacidad = (strcmp(discapacidad, "si") == 0) ? 1 : 0;
        newEstudiante->originario = (strcmp(puebloOriginario, "si") == 0) ? 1 : 0;

        map_insert(estudiantes, newEstudiante->rut, newEstudiante);
        printf("Estudiante agregado exitosamente.\n");
        presioneTeclaParaContinuar();
        break;
      }
      case 2:
      {
        // Actualizar datos
        limpiarPantalla();
        printf("ACTUALIZAR DATOS DE UN ESTUDIANTE\n\n");

        char rut[12];
        printf("Ingrese el rut del estudiante que desea actualizar sus datos: ");
        scanf(" %s", rut);

        Usuario* estudiante = map_search(estudiantes, rut) != NULL ? map_search(estudiantes, rut)->value : NULL;
        if(estudiante == NULL)
        {
          printf("No se encontró el estudiante en el sistema.\n");
          break;
        }
        int opcionDatos;
        do
        {
          limpiarPantalla();
          printf("¿Que datos del estudiante desea actualizar?\n");
          printf("1) Nombre.\n");
          printf("2) Rut.\n");
          printf("3) Nivel Socioeconómico.\n");
          printf("4) Puntaje ponderado PAES.\n");
          printf("5) Puntaje NEM.\n");
          printf("6) Discapacidad.\n");
          printf("7) Pueblo originario.\n");
          printf("8) Volver\n");
          printf("Ingrese su opción: ");

          scanf("%d", &opcionDatos);
          switch(opcionDatos)
          {
            case 1:
              printf("Nombre actual del estudiante: %s\n", estudiante->nombreEstudiante);
              printf("Ingrese el nuevo nombre del estudiante: ");
              scanf(" %[^\n]", estudiante->nombreEstudiante);
              printf("Nombre actualizado exitosamente.\n");
              break;
            case 2:
              // Update Rut logic here
              break;
            case 3:
              // Update Nivel Socioeconómico logic here
              break;
            case 4:
              // Update Puntaje ponderado PAES logic here
              break;
            case 5:
              // Update Puntaje NEM logic here
              break;
            case 6:
              // Update Discapacidad logic here
              break;
            case 7:
              // Update Pueblo originario logic here
              break;
            case 8:
              puts("Volviendo al menú anterior..");
              break;
            default:
              printf("Opción no válida, intente de nuevo.\n");
              break;
          }
          presioneTeclaParaContinuar();
        } while(opcionDatos != 8);
        break;
      }
      case 3:
      {
        // Eliminar estudiante
        limpiarPantalla();
        printf("ELIMINAR ESTUDIANTE\n\n");

        char rut[12];
        printf("Ingrese el rut del estudiante a eliminar: ");
        scanf(" %s", rut);

        Usuario* estudiante = map_search(estudiantes, rut) != NULL ? map_search(estudiantes, rut)->value : NULL;
        if(estudiante == NULL)
        {
          printf("No se encontró al estudiante en el sistema.\n");
          break;
        }

        char confirmacion[10];
        printf("Se encontró al estudiante %s en el sistema, ¿confirma la eliminación del estudiante? (si/no): ", estudiante->nombreEstudiante);
        scanf(" %s", confirmacion);

        if(strcmp(confirmacion, "si") == 0)
        {
          map_remove(estudiantes, rut);
          free(estudiante);
          printf("Estudiante eliminado exitosamente.\n");
        }
        else
        {
          printf("Eliminación del estudiante cancelada.\n");
        }
        presioneTeclaParaContinuar();
        break;
      }
      case 4:
        puts("Volviendo al menu de administrador.....");
        break;
      default:
        printf("Opción no válida, intente de nuevo.\n");
        presioneTeclaParaContinuar();
        break;
    }
  } while(opcion != 4);
  limpiarPantalla();
}


void revisarApelaciones(Queue *apelaciones)
{

  printf("REVISAR SOLICITUDES DE APELACION\n\n");
  // Recorre la cola y revisa cada solicitud
  // Aquí debe implementar la lógica para revisar solicitudes
  if(queue_front(apelaciones) == NULL)
  {
    printf("No hay solicitudes pendientes.\n");
    presioneTeclaParaContinuar();
    return;
  }
  Queue* colaTemporal = queue_create(NULL);
  Solicitud* solicitud;
  int opcion;

  while((solicitud = queue_remove(apelaciones)) != NULL)
  {
    printf("Rut del estudiante: %s\n", solicitud->rutEstudiante);
    printf("Solicitud de beca: %s\n", solicitud->nombreBeca);
    printf("Estado actual de la solicitud: %s\n", solicitud->estado);
    printf("¿Qué desea hacer con la solicitud?\n");
    printf("1) Aprobar.\n");
    printf("2) Rechazar.\n");
    printf("3) Dejar en revisión.\n");
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);

    switch(opcion)
    {
      case 1:
        strcpy(solicitud->estado, "Aprobada");
        printf("Solicitud aprobada.\n");
        break;

      case 2:
        strcpy(solicitud->estado, "Rechazada");
        printf("Solicitud rechazada.\n");
        break;

      case 3:
        printf("Solicitud dejada en revisión.\n");
        break;

      default:
        printf("Opción no valida\n");
        break;
    }
    queue_insert(colaTemporal, solicitud);
    presioneTeclaParaContinuar();
  }
  while((solicitud = queue_remove(colaTemporal)) != NULL)
  {
    queue_insert(apelaciones, solicitud);
  }
  queue_clean(colaTemporal);
}

void gestionarBecas(List *becas)
{
  // Permite agregar, actualizar o eliminar becas
  // Aquí debe implementar la lógica para gestionar becas
  int opcion;
  do
  {
    limpiarPantalla();
    printf("GESTION DE BECAS\n\n");
    puts("1) Agregar nueva beca.");
    puts("2) Actualizar datos de una beca.");
    puts("3) Eliminar beca del sistema.");
    puts("4) Volver menu anterior.");

    printf("Ingrese su opción: ");
    scanf("%d", &opcion);

    switch(opcion)
    {
      case 1: 
      {
        //Agregar nueva beca
        limpiarPantalla();
        printf("AGREGAR NUEVA BECA\n\n");
        
        tipoBeca *nuevaBeca = malloc(sizeof(tipoBeca));
        if(nuevaBeca == NULL)
        {
          printf("Error al reservar memoria para la nueva beca. (Linea 575)\n");
          break;
        }

        printf("Ingrese el nombre de la beca: ");
        scanf(" %[^\n]s", nuevaBeca->nombre);
        printf("Ingrese el Requisito Socioeconómico de la beca: ");
        scanf(" %d", &nuevaBeca->socioEconomico);
        printf("Ingrese el Puntaje Ponderado PAES: ");
        scanf("%d", &nuevaBeca->puntaje);
        printf("Ingrese el Promedio NEM: ");
        scanf(" %d", &nuevaBeca->notasEM);

        char discapacidad[10];
        char puebloOriginario[10];

        
        printf("¿La beca requiere discapacidad? (sí, no): ");
        scanf(" %s", discapacidad);
        printf("¿La beca requiere ser de un pueblo originario? (sí, no): ");
        scanf(" %s", puebloOriginario);

        nuevaBeca->discapacidad = (strcmp(discapacidad, "si") == 0) ? 1 : 0;
        nuevaBeca->originario = (strcmp(puebloOriginario, "si") == 0) ? 1 : 0;

        list_pushBack(becas, nuevaBeca);
        printf("Beca agregada exitosamente.\n");
        presioneTeclaParaContinuar();
        break;
      }
      case 2:
      {
        //Actualizar datos de una beca
        limpiarPantalla();
        printf("ACTUALIZAR DATOS DE UNA BECA\n\n");
        
        char nombreBeca[50];
        printf("Ingrese el nombre de la beca que desea actualizar: ");
        scanf(" %[^\n]s", nombreBeca);
        
        tipoBeca *beca = NULL;
        tipoBeca *auxCurrent = list_first(becas);
        while(auxCurrent != NULL)
        {
          if(strcmp(auxCurrent->nombre, nombreBeca) == 0)
          {
            beca = auxCurrent;
            //break;
          }
          auxCurrent = list_next(becas);
        }

        if(beca == NULL)
        {
          printf("No se encontró la beca en el sistema.\n");
          break;
        }
        

        int opcionDatos;
        do{
          limpiarPantalla();
          printf("¿Que datos de la beca desea actualizar?\n\n");
          puts("1) Nombre.");
          puts("2) Requisito Socioeconómico.");
          puts("3) Puntaje Ponderado PAES.");
          puts("4) NEM.");
          puts("5) Discapacidad.");
          puts("6) Pueblo Originario.");
          puts("7) Volver.\n");
          printf("Ingrese su opción: ");

          scanf("%d", &opcionDatos);
          switch(opcionDatos)
          {
            case 1:
              printf("Nombre actual de la beca: %s\n", beca->nombre);
              printf("Ingrese el nuevo nombre de la beca: ");
              scanf(" %[^\n]", beca->nombre);
              printf("Nombre de la beca actualizado exitosamente.\n");
              break;
            case 2:
              printf("Requisito Socioeconómico actual de la beca: %d\n", beca->socioEconomico);
              printf("Ingrese el nuevo Requisito Socioeconómico: ");
              scanf(" %d", &beca->socioEconomico);
              printf("Requisito Socioeconómico actualizado exitosamente.\n");
              break;
            case 3:
              printf("Puntaje Ponderado PAES actual de la beca: %d\n", beca->puntaje);
              printf("Ingrese el nuevo Puntaje Ponderado PAES: ");
              scanf("%d", &beca->puntaje);
              printf("Puntaje Ponderado PAES actualizado exitosamente.\n");
              break;
            case 4:
              printf("Promedio NEM actual: %d\n", beca->notasEM);
              printf("Ingrese el Nuevo Promedio NEM: ");
              scanf(" %d", &beca->notasEM);
              break;
            case 5:
              if (beca->discapacidad == 0) 
                  printf("Requisito de Discapacidad: No\n");
              else printf("Requisito de Discapacidad: Sí\n");
              printf("Ingrese el Requisito de Discapacidad (no = 0, si = 1): \n");
              scanf("%d", &beca->discapacidad);
              break;
            case 6:
              if (beca->originario == 0) 
                printf("Requisito de Pueblo Originario: No\n");
              else printf("Requisito de Pueblo Originario: Sí\n");
              printf("Ingrese Requisito de Pueblo Originario (no = 0, si = 1): \n");
              scanf("%d", &beca->originario);
              break;
            case 7:
              puts("Volviendo al menú anterior..");
              break;
            default:
              printf("Opcion no valida, intente denuevo.\n");
              break;
          }
          presioneTeclaParaContinuar();
        }while(opcionDatos != 7);
        break;
      }
      case 3:
      {
        //Eliminar beca
        limpiarPantalla();
        printf("ELIMINAR UNA BECA\n\n");
        
        char nombreBeca[50];
        printf("Ingrese el nombre de la beca que desea eliminar: ");
        scanf(" %[^\n]", nombreBeca);

        /*
        tipoBeca *beca = NULL;
        tipoBeca *auxCurrent = list_first(becas);
        while(auxCurrent != NULL)
        {
          if(strcmp(auxCurrent->nombre, nombreBeca) == 0)
          {
            beca = auxCurrent;
            break;
          }
          auxCurrent = list_next(becas);
        }
        if(beca == NULL)
        {
          printf("No se encontró la beca en el sistema.\n");
          break;
        }
        //list_remove(becas, beca);
        free(beca);
        */
        char confirmacion[10];
        printf("Beca ""%s"" se encuentra en el sistema, ¿Confirma la eliminacion de la beca? (si/no): ", nombreBeca);
        scanf(" %s", confirmacion);
        printf("Beca eliminada exitosamente!!\n");
        //break;
        presioneTeclaParaContinuar();
      }
      case 4:
        puts("Volviendo al menu de administrador...");
        break;

      default:
        printf("Opcion no valida, intente denuevo.\n");
    } 
  } while(opcion != 4);
  limpiarPantalla();
}

void seguimientoBecas(List *becas){
  printf("=========================================\n");
  printf("|             MENU ADMINISTRADOR        |\n");
  printf("=========================================\n");
  printf("SEGUIMIENTO DE BECAS\n\n");
  // Realiza un seguimiento de las becas
  // Aquí debe implementar la lógica para seguimiento de becas
}