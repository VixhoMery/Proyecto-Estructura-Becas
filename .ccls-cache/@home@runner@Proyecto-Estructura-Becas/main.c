#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extra_y_data/extra.h"
#include "tdas/list.h"
#include "tdas/hashmap.h"
#include "tdas/queue.h"


typedef struct {
    char nombreEstudiante[60];
    char rut[12];
    int socioEconomico;
    int puntaje;
    int notasEM;
    int discapacidad;
    int originario;
    int apela; // 0 = no, 1 = si/en revisión, 2 = rechazada, 3 = aprobada
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
    List *becasEstudiantes;
    char estado[20]; // Estado puede ser "En revisión", "Aprobada", "Rechazada"
} Solicitud;



// Prototipo de funciones
void presioneTeclaParaContinuar();
void limpiarPantalla();
void opcionUsuario(HashMap*, List*, Queue*);
void opcionAdmin(HashMap*, List*, Queue*);
int login(int);

// Prototipos submenús
void mostrarMenuAdminAlumno();
void mostrarMenuAlumno();
void mostrarMenuAdmin();

//Prototipos funciones de inicialización y extras
void inicializarBecas(List*);
void mostrarBeca(tipoBeca*, Usuario*);

// Prototipos funciones de usuario
void completarPerfil(HashMap*);
void postularBeca(HashMap*, List*, Queue*);
void apelar(Usuario*, Queue*, List*);
void seguimientoApelacion(HashMap*);

// Prototipos funciones de administrador
void gestionarEstudiantes(HashMap *);
void revisarSolicitudes(Queue *, HashMap*);
void gestionarBecas(List *);
void aprobarRechazarSolicitudes(Queue *);
void seguimientoBecas(List *);

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
    printf("===================================\n\n");
}

int main() {
  mostrarLogo();
  HashMap *estudiantes = map_create(200);
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

void opcionUsuario(HashMap *estudiantes, List *becas, Queue *solicitudes) {
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
        seguimientoApelacion(estudiantes);
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

void opcionAdmin(HashMap *estudiantes, List *becas, Queue *solicitudes) {
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
        revisarSolicitudes(solicitudes, estudiantes);
        break;
      case 3:
        gestionarBecas(becas);
        break;
      case 4:
        seguimientoBecas(becas);
        break;
      case 5:
        puts("Volviendo al menú principal...");
        break;
      default:
        printf("Opción no válida, intente de nuevo.\n");
    }
    presioneTeclaParaContinuar();
  } while(opcion != 5);
  limpiarPantalla();
}

int login(int n) {
  char contrasena[20];

  printf("Contraseña: ");
  scanf(" %s", contrasena);
  if (strcmp(contrasena, "admin") == 0) {
    n = 1;
  } else {
    n = 0;
  }
  return n;
}


// Funciones para las funcionalidades de estudiante
void completarPerfil(HashMap *estudiantes)
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
  printf("Ingrese su nombre del Estudiante: ");
  getchar(); //limpiar el buffer antes de leer
  scanf(" %[^\n]", nuevoEstudiante->nombreEstudiante);
  strcpy(nuevoEstudiante->rut, rut);
  printf("Ingrese su Porcentaje Socioeconómico: ");
  scanf(" %d", &nuevoEstudiante->socioEconomico);
  printf("Ingrese su Puntaje Ponderado PAES: ");
  scanf("%d", &nuevoEstudiante->puntaje);
  printf("Ingrese su Promedio NEM: ");
  scanf("%d", &nuevoEstudiante->notasEM);
  printf("Posee alguna discapacidad? (sí = 1; no = 0): ");
  scanf("%d", &nuevoEstudiante->discapacidad);
  printf("Es perteneciente a un pueblo originario? (sí = 1; no = 0): ");
  scanf("%d", &nuevoEstudiante->originario);


  char confirmacion[10]; // Aumentado el tamaño para permitir "si" o "no"
  while (1) {
      printf("¿Son correctos estos datos? (si/no): ");
      scanf(" %9s", confirmacion); // Limita la lectura a 9 caracteres para evitar desbordamientos

      if (strcmp(confirmacion, "si") == 0) {
         // Insertar el nuevo usuario en el hashmap
          map_insert(estudiantes, nuevoEstudiante->rut, nuevoEstudiante);
          printf("Perfil completado exitosamente.\n");
          break; // Salir del bucle si la confirmación es "si"
      } else if (strcmp(confirmacion, "no") == 0) {
          // Liberar la memoria reservada si los datos no son confirmados
          free(nuevoEstudiante);
          printf("Ingreso de datos cancelado.\n");
          break; // Salir del bucle si la confirmación es "no"
      } else {

          printf("Por favor, ingrese 'si' o 'no'.\n");
          // Limpiar el búfer de entrada para evitar problemas en la siguiente iteración del bucle
          while (getchar() != '\n');
      }
    }

  //Insertar el uevo estudiante en el mapa.
  map_insert(estudiantes, nuevoEstudiante->rut, nuevoEstudiante);
  printf("Perfil completado exitosamente.\n");
}

void postularBeca(HashMap *estudiantes, List *becas, Queue *solicitudes)
{
  //Solicitar rut del estudiante.
  char rut[12];
  printf("Ingrese su RUT: ");
  getchar(); //limpiar el buffer antes de leer
  scanf(" %[^\n]s", rut);

  if(map_search(estudiantes, rut) == NULL) {
    printf("Error: No se encuentra al estudiante en el sistema.");
    return;
  }
  
  //Comparar datos del estudiante y becas
  Usuario *estudiante = map_search(estudiantes, rut)->value;
  tipoBeca *aux = list_first(becas);
  List *becasApeladas = list_create(); 
  int requisitoAprobado = 0;
  
  printf("Becas disponibles para: %s\n", estudiante->rut);
  while(aux != NULL){
    
    if(estudiante->socioEconomico <= aux->socioEconomico)
      requisitoAprobado++;
    if(estudiante->puntaje >= aux->puntaje)
      requisitoAprobado++;
    if(estudiante->notasEM >= aux->notasEM)
      requisitoAprobado++;
    if(estudiante->discapacidad == aux->discapacidad)
      requisitoAprobado++;
    if(estudiante->originario == aux->originario)
      requisitoAprobado++;

    if(requisitoAprobado >= 4){
      mostrarBeca(aux, estudiante);
      aux = list_next(becas);
      list_pushBack(becasApeladas, aux);
      requisitoAprobado = 0;
    } else {
      aux = list_next(becas);
      requisitoAprobado = 0;
    }
  }

  //Debe cambiar para los datos de la beca (función de análisis de datos del admin)
  char opcion;
  char nombreBeca[100];
  printf("Está conforme con su selección de becas (s|n): ");
  getchar(); //limpiar el buffer antes de leer
  scanf(" %c", &opcion);
  getchar();

  //opción de postular o apelar
  switch(opcion){
    case 's':
      estudiante->apela = 0;
      printf("Ingrese la beca que desea postular: ");
      scanf("%[^\n]s", nombreBeca);
      puts("Postulación realizada y aprobada con éxito!");
      break;
    case 'n':
      apelar(estudiante, solicitudes, becasApeladas);
      estudiante->apela = 1;
      break;
    default:
      printf("Opción no válida, intente de nuevo");
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

void apelar(Usuario *estudiante, Queue *cola, List *becasApeladas) {
  Solicitud *apelacion = malloc(sizeof(Solicitud));
  if (apelacion == NULL) {
    printf("Error de reserva de memoria");
    return;
  }
  
  strcpy(apelacion->rutEstudiante, estudiante->rut);
  strcpy(apelacion->estado, "En Revisión");
  queue_insert(cola, apelacion);
  apelacion->becasEstudiantes = becasApeladas;
  printf("Apelación realizada exitosamente");
}

void seguimientoApelacion(HashMap *mapaEstudiantes) {

  char rut[12];
  printf("Ingrese su rut: ");
  getchar(); //limpiar el buffer antes de leer
  scanf("%[^\n]", rut);
  
  Usuario *estudiante = map_search(mapaEstudiantes, rut)->value;

  printf("Estado de Apelación del estudiante: %s\n", estudiante->nombreEstudiante);
  if(estudiante->apela == 0) {
    printf("No hay postulaciones realizadas por el estudiante");
    presioneTeclaParaContinuar();
    return;
  }

  if(estudiante->apela == 1) {
    printf("Estado: En revisión");
    presioneTeclaParaContinuar();
    return;
  }

  if(estudiante->apela == 2){
    printf("Estado: Aprobada");
    presioneTeclaParaContinuar();
    return;
  }

  if(estudiante->apela == 3){
    printf("Rechazada: Rechazada");
    presioneTeclaParaContinuar();
    return;
  }
}

// Funciones para las funcionalidades de administrador
void gestionarEstudiantes(HashMap *estudiantes)
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
        getchar(); //limpiar el buffer antes de leer
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

        printf("Ingrese su nombre del Estudiante: ");
        getchar(); //limpiar el buffer antes de leer
        scanf(" %[^\n]", newEstudiante->nombreEstudiante);
        strcpy(newEstudiante->rut, rut);
        printf("Ingrese su Porcentaje Socioeconómico: ");
        scanf(" %d", &newEstudiante->socioEconomico);
        printf("Ingrese su Puntaje Ponderado PAES: ");
        scanf("%d", &newEstudiante->puntaje);
        printf("Ingrese su Promedio NEM: ");
        scanf("%d", &newEstudiante->notasEM);
        printf("Posee alguna discapacidad? (sí = 1; no = 0): ");
        scanf("%d", &newEstudiante->discapacidad);
        printf("Es perteneciente a un pueblo originario? (sí = 1; no = 0): ");
        scanf("%d", &newEstudiante->originario);

        //Verificar beca

        map_insert(estudiantes, newEstudiante->rut, newEstudiante);
        printf("Estudiante agregado exitosamente.\n");
        break;
      }
      case 2:
        {
        //Actualizar datos
          char rut[12];
          printf("Ingrese el rut del estudiante que desea actualizar sus datos: ");
          getchar(); //limpiar el buffer antes de leer
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
            printf("2) Nivel Socioeconómico\n");
            printf("3) Puntaje Ponderado PAES\n");
            printf("4) Promedio NEM\n");
            puts("5) Discapacidad");
            puts("6) Originario");
            puts("7) Volver");
            puts("Nota: Los parámtros 5 y 6 se ingresan números");
            printf("Ingrese su opción: ");

            scanf("%d", &opcionDatos);
            switch(opcionDatos)
            {
              case 1:
              {
                printf("Nombre actual del estudiante: %s\n", estudiante->nombreEstudiante);
                printf("Ingrese el nuevo nombre del estudiante: ");
                getchar(); //limpiar el buffer antes de leer
                scanf(" %[^\n]", estudiante->nombreEstudiante);
                break;
              }
              case 2:
              {
                printf("Nivel socioeconómmico actual: %d\n", estudiante->socioEconomico);
                printf("Ingrese el Nuevo Nivel Socioeconómico: ");
                scanf(" %d", &estudiante->socioEconomico);
                break;
              }
              case 3:
              {
                printf("Puntaje Ponderado PAES actual: %d\n", estudiante->puntaje);
                printf("Ingrese el nuevo Puntaje Ponderado PAES: ");
                scanf(" %d", &estudiante->puntaje);
                break;
              }

              case 4: {
                printf("Promedio NEM actual: %d", estudiante->notasEM);
                printf("Ingrese el nuevo Promedio NEM: ");
                scanf("%d", &estudiante->notasEM);
                break;
              }

              case 5: {
                printf("Parámetro de discapacidad: ");
                if (estudiante->discapacidad == 0) printf("No\n");
                else {
                  printf("Sí\n");
                }
                printf("Ingrese el nuevo parámetro (1 = si, 2 = no): ");
                scanf("%d", &estudiante->discapacidad);
              }

              case 6: {
                printf("Parámetro de pueblo originario: ");
                if (estudiante->originario == 0) printf("No\n");
                else {
                  printf("Sí\n");
                }
                printf("Ingrese el nuevo parámetro (1 = si, 2 = no): ");
                scanf("%d", &estudiante->originario);
              }
              
              case 7:
              {
                puts("Volviendo al menú anterior..");
                break;
              }
              default:
                printf("Opcion no valida, intente denuevo.\n");
                break;
            }
            presioneTeclaParaContinuar();
          } while(opcionDatos != 6);
          break;
        }
      
      case 3:
      {
        //Eliminar estudiante
        char rut[12];
        printf("Ingrese el rut del estudiante a eliminar:");
        getchar(); //limpiar el buffer antes de leer
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

void revisarSolicitudes(Queue *solicitudes, HashMap *estudiantes)
{
  if(queue_front(solicitudes) == NULL)
  {
    printf("No hay solicitudes pendientes.\n");
    presioneTeclaParaContinuar();
    return;
  }
  Solicitud* solicitud;
  int opcion;
  char opcionVolver;

  while((solicitud = queue_remove(solicitudes)) != NULL)
  {
    printf("Rut del estudiante: %s\n", solicitud->rutEstudiante);

    //Para que aparezca en el menú de estudiante
    Usuario *estudianteActual = map_search(estudiantes, solicitud->rutEstudiante)->value;
    tipoBeca *aux = list_first(solicitud->becasEstudiantes);

    //Imprimir la lista de becas
    while(aux != NULL) {
      printf("Nombre de la beca: %s\n", aux->nombre);
      list_next(solicitud->becasEstudiantes);
    }
    
    printf("Estado actual de la solicitud: %s\n", solicitud->estado);
    printf("¿Qué desea hacer con la solicitud?\n");
    printf("1) Aprobar.\n");
    printf("2) Rechazar.\n");
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);

    switch(opcion)
    {
      case 1:
        strcpy(solicitud->estado, "Aprobada");
        printf("Solicitud aprobada.\n");
        estudianteActual->apela = 3; //aprobada
        break;

      case 2:
        strcpy(solicitud->estado, "Rechazada");
        printf("Solicitud rechazada.\n");
        estudianteActual->apela = 2; // rechazada
        break;

      default:
        printf("Opción no valida\n");
        break;
    }

    printf("Desea regresar al menú principal? (s|n): ");
    getchar(); //limpiar el buffer antes de leer
    scanf(" %c", &opcionVolver);

    switch(opcionVolver) {
      case 's':
        printf("Volviendo...\n");
        presioneTeclaParaContinuar();
        return;
      case 'n':
        break;
    }
    presioneTeclaParaContinuar();
  }
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
        getchar(); //limpiar el buffer antes de leer
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
        getchar(); //limpiar el buffer antes de leer
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
              getchar(); //limpiar el buffer antes de leer
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
        getchar(); //limpiar el buffer antes de leer
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

void seguimientoBecas(List *becas) {
  // Realiza un seguimiento de las becas
  // Aquí debe implementar la lógica para seguimiento de becas
}