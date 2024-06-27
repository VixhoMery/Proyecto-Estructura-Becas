#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extra_y_data/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/queue.h"

typedef struct {
    char nombreEstudiante[60];
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
  int apela;
} tipoBeca;

typedef struct {
    char rutEstudiante[12];
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

//Prototipos funciones de inicialización y extras
void inicializarBecas(List* becas);
void mostrarBeca(tipoBeca*, Usuario*);

// Prototipos funciones de usuario
void completarPerfil(Map *estudiantes);
void postularBeca(Map *estudiantes, List *becas, Queue *solicitudes);
void apelar(Usuario*, Queue*);
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
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

void mostrarMenuAdminAlumno(){
  puts("Seleccione el método de ingreso:");
  puts("1) Alumno");
  puts("2) Administrador");
  puts("3) Salir");
}

void mostrarMenuAlumno(){
  puts("1) Ingresar o Completar perfil");
  puts("2) Postular a Beca");
  puts("3) Seguimiento de Apelación");
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
    printf("===================================\n\n");
}

int main() {
  mostrarLogo();
  Map *estudiantes = map_create((int (*)(void*, void*))strcmp);
  List *becas = list_create();
  Queue *solicitudes = queue_create(NULL);
  inicializarBecas(becas);
  
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

//Funcion para inicializar becas
void inicializarBecas(List *listaBecas) {
  FILE *archivo = fopen("extra_y_data/Archivo_CSV_Becas.csv", "r");
  if (archivo == NULL){
    perror("Error al abrir el archivo");
    exit(EXIT_FAILURE);
  }
    
  char **campos;
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    tipoBeca *beca = malloc(sizeof(tipoBeca)); 
  
    strcpy(beca->nombre,campos[0]);
    beca->socioEconomico = atoi(campos[1]);
    beca->puntaje = atoi(campos[2]);
    beca->notasEM = atoi(campos[3]);
    beca->discapacidad = atoi(campos[4]);
    beca->originario = atoi(campos[5]);
  
    list_pushBack(listaBecas, beca);
  }
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
void completarPerfil(Map *estudiantes)
{
  //Solicitar un Rut primero para verificar duplicados.
  char rut[12];
  printf("Ingrese su Rut: ");
  scanf(" %s", rut);

  //Verificar si el estudiante existe en el mapa.
  if(map_search(estudiantes, rut) != NULL)
  {
    //Dependiendo que quieran implementar, se puede mostrar un mensaje de error o si quieren implementar una verificacion que los datos del estudiante esten completamente ingresados.
    printf("El estudiante ya se encuentra registrado, sus datos serán actualizados.\n");
    
    
  }


  //Reservar memoria para un nuevo estudiante
  Usuario *nuevoEstudiante = malloc(sizeof(Usuario));
  if(nuevoEstudiante == NULL){
    printf("Error al reservar memoria para el estudiante.\n");
    return;
  }

  //Completacion de los datos del nuevo estudiante.
  printf("Ingrese su nombre: ");
  scanf(" %[^\n]", nuevoEstudiante->nombreEstudiante);
  strcpy(nuevoEstudiante->rut, rut);
  printf("Ingrese su Porcentaje Socioeconómico: ");
  scanf(" %d", &nuevoEstudiante->socioEconomico);
  printf("Ingrese su historial académico: ");


  //Insertar el uevo estudiante en el mapa.
  map_insert(estudiantes, nuevoEstudiante->rut, nuevoEstudiante);
  printf("Perfil completado exitosamente.\n");
}

void postularBeca(Map *estudiantes, List *becas, Queue *solicitudes)
{
  //Solicitar rut del estudiante.
  char rut[12];
  printf("Ingrese su RUT: ");
  scanf(" %[^\n]s", rut);

  if(map_search(estudiantes, rut) == NULL) {
    printf("Error: No se encuentra al estudiante en el sistema.");
    return;
  }
  
  //Comparar datos del estudiante y becas
  Usuario *estudiante = map_search(estudiantes, rut)->value;
  tipoBeca *aux = list_first(becas);
  int requisitoAprobado = 1;
  
  printf("Becas disponibles para %s", estudiante->rut);
  while(aux != NULL){
    
    if(estudiante->socioEconomico >= aux->socioEconomico)
      requisitoAprobado = 0;
    if(estudiante->puntaje < aux->puntaje)
      requisitoAprobado = 0;
    if(estudiante->notasEM < aux->notasEM)
      requisitoAprobado = 0;
    if(estudiante->discapacidad != aux->discapacidad)
      requisitoAprobado = 0;
    if(estudiante->originario != aux->originario)
      requisitoAprobado = 0;

    if(requisitoAprobado == 1){
      mostrarBeca(aux, estudiante);
      aux = list_next(becas);
    } else {
      aux = list_next(becas);
      requisitoAprobado = 1;
    }
  }

  char opcion;
  char nombreBeca[100];
  printf("Está conforme con su selección de becas (s|n): ");
  scanf("%c", &opcion);
  getchar();

  //opción de postular o apelar
  switch(opcion){
    case 's':
      printf("Ingrese la beca que desea postular: ");
      scanf("%[^\n]s", nombreBeca);
      puts("Postulación realizada y aprobada con éxito!");
      break;
    case 'n':
      apelar(estudiante, solicitudes);
      break;
    default:
      printf("Opción no válida, intente de nuevo");
  }
  

  //Verificar si existe la beca en la lista de becas.
  tipoBeca *becaExiste = NULL;
  tipoBeca* current = list_first(becas);
  while(current != NULL)
  {
    if(strcmp(current->nombre, nombreBeca) == 0){
      becaExiste = current;
      break;
    }
    current = list_next(becas);
  }

  if(becaExiste == NULL)
  {
    printf("No se encuentra la beca en el sistema.");
    return;  
  }

  //Verificar si existe ya una postulacion a la beca por parte del estudiante.
  Queue* temporal= queue_create(NULL);
  Solicitud* solicitudExiste = NULL;
  Solicitud* postulacion;

  while((postulacion = queue_remove(solicitudes)) != NULL)
  {
    if(strcmp(postulacion->rutEstudiante, rut) == 0 & strcmp(postulacion->nombreBeca, nombreBeca) == 0)
    {
      solicitudExiste = postulacion;
    }
    queue_insert(temporal, postulacion);
  }

  while((postulacion = queue_remove(temporal)) != NULL)
  {
    queue_insert(solicitudes, postulacion);
  }
  queue_clean(temporal);

  if(solicitudExiste != NULL)
  {
    printf("Ya existe una postulacion a la beca %s por el rut %s", nombreBeca, rut);
    //Se puede implementar un if else aqui que si desea altiro saber el estado de solicitud, llamando a la otra funcion.
    return;
  }

  //Crear y reservar memoria para la nueva solicitud.
  Solicitud* newSolicitud = malloc(sizeof(Solicitud));
  if(newSolicitud == NULL)
  {
    printf("Error al reservar memoria para la nueva solicitud (linea 312)\n");
    return;
  }

  //Asignar datos a la nueva solicitud.
  strcpy(newSolicitud->rutEstudiante, rut);
  strcpy(newSolicitud->nombreBeca, nombreBeca);
  strcpy(newSolicitud->estado, "En revisión");

  //Insertar la nueva solicitud en la cola de solicitudes.
  queue_insert(solicitudes, newSolicitud);
  printf("Postulación realizada exitosamente.\n");

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

void apelar(Usuario *estudiante, Queue *cola) {
  Solicitud *apelacion = malloc(sizeof(Solicitud));
  if (apelacion == NULL) {
    printf("Error de reserva de memoria");
    return;
  }

  strcpy(apelacion->rutEstudiante, estudiante->rut);
  strcpy(apelacion->estado, "En Revisión");
  queue_insert(cola, apelacion);
  printf("Apelación realizada exitosamente");
  free(apelacion);
}

void seguimientoPostulacion(Queue *solicitudes) {
  //esto debe cambiar por apelación
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
  do{
    limpiarPantalla();
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
        //Agregar un estudiante
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

        printf("Ingrese el nombre del estudiante a agregar: ");
        scanf(" %[^\n]", newEstudiante->nombreEstudiante);
        strcpy(newEstudiante->rut, rut);
        printf("Ingrese la dirección del estudiante a agregar: ");
        scanf(" %[^\n]", newEstudiante->direccion);
        printf("Ingrese el historial académico del estudiante a agregar: ");
        scanf(" %[^\n]", newEstudiante->historialAcademico);

        map_insert(estudiantes, newEstudiante->rut, newEstudiante);
        printf("Estudiante agregado exitosamente.\n");
        break;
      }
      case 2:
        {
        //Actualizar datos
          char rut[12];
          printf("Ingrese el rut del estudiante que desea actualizar sus datos: ");
          scanf(" %s", rut);

          Usuario* estudiante = map_search(estudiantes, rut)->value;
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
            printf("1) Nombre\n");
            printf("2) Dirección\n");
            printf("3) Historial académico\n");
            printf("4) Volver\n");
            printf("Ingrese su opción: ");

            scanf("%d", &opcionDatos);
            switch(opcionDatos)
            {
              case 1:
              {
                printf("Nombre actual del estudiante: %s\n", estudiante->nombreEstudiante);
                printf("Ingrese el nuevo nombre del estudiante: ");
                scanf(" %[^\n]", estudiante->nombreEstudiante);
                break;
              }
              case 2:
              {
                printf("Dirección actual del estudiante: %s\n", estudiante->direccion);
                printf("Ingrese la nueva dirección del estudiante: ");
                scanf(" %[^\n]", estudiante->direccion);
                break;
              }
              case 3:
              {
                printf("Historial académico actual del estudiante: %s\n", estudiante->historialAcademico);
                printf("Ingrese el nuevo historial academico del estudiante: ");
                scanf(" %[^\n]", estudiante->historialAcademico);
                break;
              }
            case 4:
              {
                puts("Volviendo al menú anterior..");
                break;
              }
              default:
                printf("Opcion no valida, intente denuevo.\n");
                break;
            }
            presioneTeclaParaContinuar();
          } while(opcionDatos != 4);
          break;
      }
      case 3:
      {
        //Eliminar estudiante
        char rut[12];
        printf("Ingrese el rut del estudiante a eliminar:");
        scanf(" %s", rut);

        Usuario* estudiante = map_search(estudiantes, rut)->value;
        if(estudiante == NULL)
        {
          printf("No se encontró al estudiante en el sistema.\n");
          break;
        }
        map_remove(estudiantes, rut);
        free(estudiante);
        printf("Estudiante eliminado exitosamente.\n");
        break;
      }
      case 4:
        puts("Volviendo al menu de administrador.....");
        break;
      default:
        printf("Opcion no valida, intente de nuevo.\n");
    }
    presioneTeclaParaContinuar();
  } while(opcion != 4);
  limpiarPantalla();
}

void revisarSolicitudes(Queue *solicitudes)
{
  // Recorre la cola y revisa cada solicitud
  // Aquí debe implementar la lógica para revisar solicitudes
  if(queue_front(solicitudes) == NULL)
  {
    printf("No hay solicitudes pendientes.\n");
    presioneTeclaParaContinuar();
    return;
  }
  Queue* colaTemporal = queue_create(NULL);
  Solicitud* solicitud;
  int opcion;

  while((solicitud = queue_remove(solicitudes)) != NULL)
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
    queue_insert(solicitudes, solicitud);
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
        tipoBeca *nuevaBeca = malloc(sizeof(tipoBeca));
        if(nuevaBeca == NULL)
        {
          printf("Error al reservar memoria para la nueva beca. (Linea 575)\n");
          break;
        }

        printf("Ingrese el nombre de la beca: \n");
        scanf(" %[^\n]s", nuevaBeca->nombre);
        printf("Ingrese el Requisito Socioeconómico de la beca: \n");
        scanf(" %d", &nuevaBeca->socioEconomico);
        printf("Ingrese el Puntaje Ponderado PAES: \n");
        scanf("%d", &nuevaBeca->puntaje);
        printf("Ingrese el Promedio NEM: \n");
        scanf(" %d", &nuevaBeca->notasEM);
        printf("Ingrese parámetro de discapacidad (1 = sí, 0 = no): \n");
        scanf("%d", &nuevaBeca->discapacidad);
        printf("Ingrese parámetro de pueblo originario (1 = sí, 0 = no): \n");
        scanf("%d", &nuevaBeca->originario);

        list_pushBack(becas, nuevaBeca);
        printf("Beca agregada exitosamente.\n");
        break;
      }
      case 2:
      {
        //Actualizar datos de una beca
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
            break;
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
          printf("¿Que datos de la beca desea actualizar?\n");
          puts("1) Nombre.\n");
          puts("2) Requisito Socioeconómico.\n");
          puts("3) Puntaje Ponderado PAES.\n");
          puts("4) NEM.\n");
          puts("5) Discapacidad.\n");
          puts("6) Pueblo Originario.\n");
          puts("7) Volver");
          printf("Ingrese su opción: ");

          scanf("%d", &opcionDatos);
          switch(opcionDatos)
          {
            case 1:
              printf("Nombre actual de la beca: %s\n", beca->nombre);
              printf("Ingrese el nuevo nombre de la beca: ");
              scanf(" %[^\n]", beca->nombre);
              break;
            case 2:
              printf("Requisito Socioeconómico actual de la beca: %d\n", beca->socioEconomico);
              printf("Ingrese el nuevo Requisito Socioeconómico: ");
              scanf(" %d", &beca->socioEconomico);
              break;
            case 3:
              printf("Puntaje Ponderado PAES actual de la beca: %d\n", beca->notasEM);
              printf("Ingrese el nuevo Puntaje Ponderado PAES: ");
              scanf("%d", &beca->notasEM);
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
        }while(opcionDatos != 5);
        break;
      }

      case 3:
      {
        //Eliminar beca
        char nombreBeca[50];
        printf("Ingrese el nombre de la beca que desea eliminar: ");
        scanf(" %[^\n]", nombreBeca);

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
        printf("Beca eliminada exitosamente.\n");
        break;
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

void aprobarRechazarSolicitudes(Queue *solicitudes) {
  // Permite aprobar o rechazar solicitudes en la cola
  // Aquí debe implementar la lógica para aprobar/rechazar solicitudes
  if((queue_front(solicitudes)) == NULL)
  {
    printf("No hay solicitudes pendientes para revisar.\n");
    return;
  }
  Queue* temporalcola = queue_create(NULL);
  Solicitud *solicitud;
  int opcion;

  while((solicitud = queue_remove(solicitudes)) != NULL)
  {
    printf("Revisando solicitud del Rut: %s\n", solicitud->rutEstudiante);
    printf("Beca solicitada: %s\n", solicitud->nombreBeca);
    printf("Estado actual de la solicitud: %s\n", solicitud->estado);
    printf("¿Qué desea hacer con la solicitud?\n");
    printf("1) Aprobar\n");
    printf("2) Rechazar\n");
    printf("Ingrese su opción");
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
      default:
        printf("Opción no valida. La solicitud permanecera en revisión.");
        break;     
    }
    queue_insert(temporalcola, solicitud);
    printf("\n");
  }
  while((solicitud = queue_remove(temporalcola)) != NULL)
  {
    queue_insert(solicitudes, solicitud);
  }
  queue_clean(temporalcola);
  printf("Todas las solicitudes han sido revisadas y aprobadas/rechazadas.\n");
}

void seguimientoBecas(List *becas) {
  // Realiza un seguimiento de las becas
  // Aquí debe implementar la lógica para seguimiento de becas
}