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
    List *becasEstudiantes;
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
    char motivos[200]; // Razón de apelación
} Apelacion;



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
char *obtenerNombreEstudiante(HashMap *estudiantes, char *rut);

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
void seguimientoBecas(HashMap *estudiantes, List *becas);

//Prototipo ContarEstudiantes
int count_students(HashMap *map);

// Prototipo de función para validar RUT
int validarRUT(const char* rut);

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

// Función para contar el número de elementos en el HashMap
int count_students(HashMap *map) {
    int size = 0;
    MapPair *pair = marp_first(map);
    while (pair != NULL) {
        size++;
        pair = map_next(map);
    }
    return size;
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

// Función auxiliar para obtener el nombre de un estudiante a partir de su rut
char *obtenerNombreEstudiante(HashMap *estudiantes, char *rut) {
    MapPair *pair = map_search(estudiantes, rut);
    if (pair != NULL) {
        Usuario *estudiante = (Usuario *)pair->value;
        return estudiante->nombreEstudiante;
    }
    return "Nombre no encontrado";
}

// Funcion usuarios
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

//Funcion Admins
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
        seguimientoBecas(estudiantes, becas);
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

//Nuestro super login el cual nos permite ingresar como administrador o como usuario. Jamas, pero de los jamases tendria vulnerabilidades XD!
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

//Funcion validarRut para casos
int validarRUT(const char *rut) {
  int sum = 0, factor = 2, length = strlen(rut) - 2;
  for (int i = length - 1; i >= 0; i--) {
    if (rut[i] == '.') continue;
    sum += (rut[i] - '0') * factor;
    factor = (factor == 7) ? 2 : factor + 1;
  }
  int remainder = 11 - (sum % 11);
  char checkDigit = (remainder == 11) ? '0' : (remainder == 10) ? 'K' : '0' + remainder;
  return checkDigit == rut[length + 1];
}


// Función para completar perfil
void completarPerfil(HashMap *estudiantes) {
    Usuario *nuevoUsuario = (Usuario *) malloc(sizeof(Usuario));
    if (nuevoUsuario == NULL) {
        printf("Error al reservar memoria para el usuario.\n");
        return;
    }
    printf("Si su rut termina en K, ingreselo con un 0 al final.\n");
    printf("Ingrese RUT: ");
    scanf("%s", nuevoUsuario->rut);

    // Validación del RUT
    if (!validarRUT(nuevoUsuario->rut)) {
        printf("RUT inválido.\n");
        free(nuevoUsuario);
        return;
    }

    printf("Ingrese nombre: ");
    scanf(" %[^\n]", nuevoUsuario->nombreEstudiante);
    printf("Ingrese nivel socioeconómico, sin el porcentaje (0 al 100): ");
    scanf("%d", &nuevoUsuario->socioEconomico);
    printf("Ingrese puntaje ponderado en la prueba PAES (150 al 1000): ");
    scanf("%d", &nuevoUsuario->puntaje);
    if (nuevoUsuario->puntaje < 150 || nuevoUsuario->puntaje > 1000) {
      printf("Puntaje inválido. Debe estar entre 150 y 1000.\n");
      free(nuevoUsuario);
      return;
    
    }
    printf("Ingrese su NEM (Notas de Enseñanza Media): ");
    scanf("%d", &nuevoUsuario->notasEM);
    if (nuevoUsuario->notasEM < 2 || nuevoUsuario->notasEM > 7) {
      printf("NEM inválido. Debe estar entre 2 y 7.\n");
      free(nuevoUsuario);
      return;
    }
    printf("¿Tiene alguna discapacidad? (1: Sí, 0: No): ");
    scanf("%d", &nuevoUsuario->discapacidad);
    printf("¿Es originario? (1: Sí, 0: No): ");
    scanf("%d", &nuevoUsuario->originario);
    nuevoUsuario->apela = 0;

    // Mostrar los datos ingresados y pedir confirmación
    printf("\nPor favor, confirme los datos ingresados:\n");
    printf("RUT: %s\n", nuevoUsuario->rut);
    printf("Nombre: %s\n", nuevoUsuario->nombreEstudiante);
    printf("Nivel Socioeconómico: %d\n", nuevoUsuario->socioEconomico);
    printf("Puntaje: %d\n", nuevoUsuario->puntaje);
    printf("Notas de enseñanza media: %d\n", nuevoUsuario->notasEM);
    printf("¿Tiene alguna discapacidad? %s\n", nuevoUsuario->discapacidad ? "Sí" : "No");
    printf("¿Es originario? %s\n", nuevoUsuario->originario ? "Sí" : "No");

  char confirmacion[10]; // Aumentado el tamaño para permitir "si" o "no"
  while (1) {
      printf("¿Son correctos estos datos? (si/no): ");
      scanf(" %9s", confirmacion); // Limita la lectura a 9 caracteres para evitar desbordamientos

      if (strcmp(confirmacion, "si") == 0) {
          // Insertar el nuevo usuario en el hashmap
          map_insert(estudiantes, nuevoUsuario->rut, nuevoUsuario);
          printf("Perfil completado exitosamente.\n");
          break; // Salir del bucle si la confirmación es "si"
      } else if (strcmp(confirmacion, "no") == 0) {
          // Liberar la memoria reservada si los datos no son confirmados
          free(nuevoUsuario);
          printf("Ingreso de datos cancelado.\n");
          break; // Salir del bucle si la confirmación es "no"
      } else {
          printf("Por favor, ingrese 'si' o 'no'.\n");
          // Limpiar el búfer de entrada para evitar problemas en la siguiente iteración del bucle
          while (getchar() != '\n');
      }
  }
}


void mostrarBeca(tipoBeca *beca, Usuario *estudiante){
    printf("Nombre Beca: %s\n", beca->nombre);
    printf("Requisito Socioeconómico: %d\n", beca->socioEconomico);
    printf("Requisito Puntaje Ponderado: %d\n", beca->puntaje);
    printf("Requisito de NEM: %d\n", beca->notasEM);
    printf("Discapacidad: %s\n", beca->discapacidad ? "Sí" : "No");
    printf("Pueblo Originario: %s\n", beca->originario ? "Sí" : "No");
    printf("\n\n");
}


void guardarPostulacion(HashMap *estudiantes, Usuario *estudiante, tipoBeca *becaSeleccionada) {
    // Buscar al estudiante en el mapa
    Usuario *usuario = (Usuario *)map_search(estudiantes, estudiante->rut);

    // Verificar si el estudiante existe en el mapa
    if (usuario == NULL) {
        printf("El estudiante con RUT %s no existe.\n", estudiante->rut);
        return;
    }

    // Obtener la lista de becas del estudiante
    List *becasEstudiante = usuario->becasEstudiantes;

    // Si no tiene, crear una nueva lista para almacenar las becas aprobadas
    if (becasEstudiante == NULL) {
        becasEstudiante = list_create();
        usuario->becasEstudiantes = becasEstudiante;
    }

    // Agregar la beca seleccionada a la lista de becas del estudiante
    list_pushBack(becasEstudiante, becaSeleccionada);

    printf("Postulación guardada con éxito para el estudiante %s a la beca %s!\n", estudiante->rut, becaSeleccionada->nombre);
}


// Función principal para postular a becas
void postularBeca(HashMap *estudiantes, List *becas, Queue *solicitudes) {
    // Solicitar rut del estudiante
    char rut[12];
    printf("Ingrese su RUT: ");
    scanf(" %[^\n]s", rut);  // Se usa " %[^\n]s" para permitir espacios en el RUT

    // Verificar si el estudiante está registrado
    if (map_search(estudiantes, rut) == NULL) {
        printf("Error: No se encuentra al estudiante en el sistema.\n");
        return;
    }

    // Obtener datos del estudiante
    Usuario *estudiante = (Usuario *)map_search(estudiantes, rut)->value;

    // Mostrar becas disponibles para el estudiante
    printf("\nBecas disponibles para %s:\n", estudiante->rut);
    int becasEncontradas = 0;
    List *becasApeladas = list_create();  // Lista para almacenar becas a elección
    tipoBeca *aux = (tipoBeca *)list_first(becas);  // Obtener el primer elemento de la lista de becas

    int i = 1;  // Contador para enumerar las becas
    while (aux != NULL) {
        int requisitosCumplidos = 0;

        // Comparar requisitos de beca con datos del estudiante
        if (estudiante->socioEconomico <= aux->socioEconomico)
            requisitosCumplidos++;
        if (estudiante->puntaje >= aux->puntaje)
            requisitosCumplidos++;
        if (estudiante->notasEM >= aux->notasEM)
            requisitosCumplidos++;
        if (estudiante->discapacidad == aux->discapacidad)
            requisitosCumplidos++;
        if (estudiante->originario == aux->originario)
            requisitosCumplidos++;

        if (requisitosCumplidos >= 4) {  // Verifica si los requisitos de la beca son cumplidos
            printf("%d. ", i);  // Mostrar número de beca
            mostrarBeca(aux, estudiante);  // Mostrar detalles de la beca
            becasEncontradas++;
            list_pushBack(becasApeladas, aux);  // Agregar beca a la lista de becas disponibles
            i++;
        }

        aux = (tipoBeca *)list_next(becas);  // Avanzar al siguiente elemento en la lista de becas
    }

    if (becasEncontradas == 0) {
        printf("No hay becas disponibles para usted en este momento.\n");
        return;
    }

    // Confirmación de elección de beca
    char opcion;
    printf("Está conforme con su selección de becas (s/n): ");
    scanf(" %c", &opcion);  // Se usa " %c" para leer un solo carácter
    getchar();  // Limpiar el buffer de entrada

    switch (opcion) {
        case 's': {  // Si el usuario está conforme con la selección
            estudiante->apela = 0;  // Establecer que no está apelando
            printf("Ingrese el número de la beca que desea postular: ");
            int seleccion;
            scanf("%d", &seleccion);
            getchar();  // Limpiar el buffer de entrada

            // Seleccionar la beca del índice especificado por el usuario
            tipoBeca *becaSeleccionada = NULL;
            Node *nodoActual = becasApeladas->head;
            for (int index = 0; nodoActual != NULL; index++) {
                if (index == seleccion - 1) {
                    becaSeleccionada = (tipoBeca *)nodoActual->data;
                    break;
                }
                nodoActual = nodoActual->next;
            }

            if (becaSeleccionada != NULL) {
                // Guardar la postulación de la beca en el sistema
                guardarPostulacion(estudiantes, estudiante, becaSeleccionada);
            } else {
                printf("Selección inválida. Intente de nuevo.\n");
            }
            break;
        }
        case 'n':  // Si el usuario no está conforme con la selección
            apelar(estudiante, solicitudes, becasApeladas);  // Llamar a la función de apelación
            estudiante->apela = 1;  // Establecer que está apelando
            break;
        default:
            printf("Opción no válida, intente de nuevo.\n");
    }
}

void apelar(Usuario *estudiante, Queue *cola, List *becasApeladas) {
    char motivos[200];
    printf("Ingrese los motivos de su apelación (máx. 200 caracteres):\n");
    scanf(" %[^\n]", motivos);

    Apelacion *apelacion = malloc(sizeof(Apelacion));
    if (apelacion == NULL) {
        printf("Error de reserva de memoria.\n");
        return;
    }

    strcpy(apelacion->rutEstudiante, estudiante->rut);
    strcpy(apelacion->estado, "En Revisión");
    apelacion->becasEstudiantes = becasApeladas;
    strcpy(apelacion->motivos, motivos); // Asignar la razón de apelación
    printf("Motivos de apelación: %s\n", motivos);

    queue_insert(cola, apelacion);
    printf("Apelación realizada exitosamente.\n");
}

void seguimientoApelacion(HashMap *mapaEstudiantes) {
    char rut[12];
    printf("Ingrese su rut: ");
    scanf(" %[^\n]", rut);

    // Buscar al estudiante en el mapa
    MapPair *pair = map_search(mapaEstudiantes, rut);
    if (pair == NULL) {
        printf("No se encontró ningún estudiante con el rut ingresado.\n");
        presioneTeclaParaContinuar();
        return;
    }

    Usuario *estudiante = (Usuario *) pair->value;

    printf("Estado de Apelación del estudiante %s: %s\n", estudiante->rut, estudiante->nombreEstudiante);

    switch (estudiante->apela) {
        case 0:
            printf("No hay postulaciones realizadas por el estudiante.\n");
            break;
        case 1:
            printf("Estado: En revisión\n");
            break;
        case 2:
            printf("Estado: Aprobada\n");
            break;
        case 3:
            printf("Estado: Rechazada\n");
            break;
        default:
            printf("Estado desconocido.\n");
            break;
    }

    presioneTeclaParaContinuar();
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
          } while(opcionDatos != 7);
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

void revisarSolicitudes(Queue *solicitudes, HashMap *estudiantes) {
    if(queue_front(solicitudes) == NULL) {
        printf("No hay solicitudes pendientes.\n");
        presioneTeclaParaContinuar();
        return;
    }

    Apelacion* solicitud;
    int opcion;
    char opcionVolver;

    while((solicitud = queue_remove(solicitudes)) != NULL) {
        printf("\nRut del estudiante: %s\n", solicitud->rutEstudiante);

        // Para que aparezca en el menú de estudiante
        Usuario *estudianteActual = map_search(estudiantes, solicitud->rutEstudiante)->value;
        tipoBeca *aux = list_first(solicitud->becasEstudiantes);

        // Imprimir la lista de becas
        int indice = 1;
        while(aux != NULL) {
            printf("%d) Nombre de la beca: %s\n", indice, aux->nombre);
            aux = list_next(solicitud->becasEstudiantes);
            indice++;
        }

        // Mostrar los motivos de apelación si existen
        if (strcmp(solicitud->estado, "En Revisión") == 0) {
            printf("Motivos de apelación: %s\n", solicitud->motivos);
        }

        printf("\nEstado actual de la solicitud: %s\n\n", solicitud->estado);
        printf("¿Qué desea hacer con la solicitud?\n");
        printf("1) Aprobar.\n");
        printf("2) Rechazar.\n");
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                strcpy(solicitud->estado, "Aprobada");
                printf("Solicitud aprobada.\n");
                estudianteActual->apela = 3; // Aprobada
                break;
            case 2:
                strcpy(solicitud->estado, "Rechazada");
                printf("Solicitud rechazada.\n");
                estudianteActual->apela = 2; // Rechazada
                break;
            default:
                printf("Opción no válida\n");
                break;
        }

        printf("¿Desea regresar al menú principal? (s/n): ");
        scanf(" %c", &opcionVolver); // Nota: espacio antes de %c para consumir cualquier carácter de nueva línea pendiente

        if(opcionVolver == 's' || opcionVolver == 'S') {
            printf("Volviendo...\n");
            presioneTeclaParaContinuar();
            return;
        }

        presioneTeclaParaContinuar();
    }
    printf("No hay más solicitudes pendientes.\n");
    presioneTeclaParaContinuar();
}

void mostrarBecas(List *becas) {
    int indice = 1;
    tipoBeca *beca = list_first(becas);
    while (beca != NULL) {
        printf("%d) %s\n", indice, beca->nombre);
        beca = list_next(becas);
        indice++;
    }
}

tipoBeca* seleccionarBeca(List *becas) {
    int indiceSeleccionado;
    mostrarBecas(becas);
    printf("Seleccione el número de la beca: ");
    scanf("%d", &indiceSeleccionado);

    int indiceActual = 1;
    tipoBeca *beca = list_first(becas);
    while (beca != NULL) {
        if (indiceActual == indiceSeleccionado) {
            return beca;
        }
        beca = list_next(becas);
        indiceActual++;
    }
    return NULL; // Si no se encuentra el índice
}


void agregarNuevaBeca(List *becas) {
    tipoBeca *nuevaBeca = malloc(sizeof(tipoBeca));
    if (nuevaBeca == NULL) {
        printf("Error al reservar memoria para la nueva beca.\n");
        return;
    }

    printf("Ingrese el nombre de la beca: \n");
    scanf(" %[^\n]s", nuevaBeca->nombre);
    printf("Ingrese el Requisito Socioeconómico de la beca: \n");
    scanf("%d", &nuevaBeca->socioEconomico);
    printf("Ingrese el Puntaje Ponderado PAES: \n");
    scanf("%d", &nuevaBeca->puntaje);
    printf("Ingrese el Promedio NEM: \n");
    scanf("%d", &nuevaBeca->notasEM);
    printf("Ingrese parámetro de discapacidad (1 = sí, 0 = no): \n");
    scanf("%d", &nuevaBeca->discapacidad);
    printf("Ingrese parámetro de pueblo originario (1 = sí, 0 = no): \n");
    scanf("%d", &nuevaBeca->originario);

    list_pushBack(becas, nuevaBeca);
    printf("Beca agregada exitosamente.\n");
}

void actualizarBeca(List *becas) {
    tipoBeca *beca = seleccionarBeca(becas);
    if (beca == NULL) {
        printf("Selección inválida. No se encontró la beca en el sistema.\n");
        return;
    }

    int opcionDatos;
    do {
        limpiarPantalla();
        printf("¿Qué datos de la beca desea actualizar?\n");
        puts("1) Nombre.");
        puts("2) Requisito Socioeconómico.");
        puts("3) Puntaje Ponderado PAES.");
        puts("4) NEM.");
        puts("5) Discapacidad.");
        puts("6) Pueblo Originario.");
        puts("7) Volver");
        printf("Ingrese su opción: ");

        scanf("%d", &opcionDatos);
        switch(opcionDatos) {
            case 1:
                printf("Nombre actual de la beca: %s\n", beca->nombre);
                printf("Ingrese el nuevo nombre de la beca: ");
                scanf(" %[^\n]s", beca->nombre);
                break;
            case 2:
                printf("Requisito Socioeconómico actual de la beca: %d\n", beca->socioEconomico);
                printf("Ingrese el nuevo Requisito Socioeconómico: ");
                scanf("%d", &beca->socioEconomico);
                break;
            case 3:
                printf("Puntaje Ponderado PAES actual de la beca: %d\n", beca->puntaje);
                printf("Ingrese el nuevo Puntaje Ponderado PAES: ");
                scanf("%d", &beca->puntaje);
                break;
            case 4:
                printf("Promedio NEM actual: %d\n", beca->notasEM);
                printf("Ingrese el Nuevo Promedio NEM: ");
                scanf("%d", &beca->notasEM);
                break;
            case 5:
                printf("Requisito de Discapacidad actual: %d\n", beca->discapacidad);
                printf("Ingrese el Requisito de Discapacidad (no = 0, sí = 1): \n");
                scanf("%d", &beca->discapacidad);
                break;
            case 6:
                printf("Requisito de Pueblo Originario actual: %d\n", beca->originario);
                printf("Ingrese Requisito de Pueblo Originario (no = 0, sí = 1): \n");
                scanf("%d", &beca->originario);
                break;
            case 7:
                puts("Volviendo al menú anterior..");
                break;
            default:
                printf("Opción no válida, intente de nuevo.\n");
                break;
        }
        presioneTeclaParaContinuar();
    } while (opcionDatos != 7);
}


void eliminarBeca(List *becas) {
    tipoBeca *beca = seleccionarBeca(becas);
    if (beca == NULL) {
        printf("Selección inválida. No se encontró la beca en el sistema.\n");
        return;
    }

    tipoBeca *becaEliminada = list_popCurrent(becas);
    if (becaEliminada != NULL) {
        free(becaEliminada);
        printf("Beca eliminada exitosamente.\n");
    } else {
        printf("Error al intentar eliminar la beca.\n");
    }
}

void gestionarBecas(List *becas) {
    int opcion;
    do {
        limpiarPantalla();
        puts("1) Agregar nueva beca.");
        puts("2) Actualizar datos de una beca.");
        puts("3) Eliminar beca del sistema.");
        puts("4) Volver menú anterior.");

        printf("Ingrese su opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                agregarNuevaBeca(becas);
                break;
            case 2:
                actualizarBeca(becas);
                break;
            case 3:
                eliminarBeca(becas);
                break;
            case 4:
                puts("Volviendo al menú de administrador...");
                break;
            default:
                printf("Opción no válida, intente de nuevo.\n");
        }
    } while(opcion != 4);
    limpiarPantalla();
}


void seguimientoBecas(HashMap *estudiantes, List *becas) {
    tipoBeca *beca = (tipoBeca *)list_first(becas);
    int totalEstudiantes = count_students(estudiantes); // Número total de estudiantes en el sistema

    if (totalEstudiantes == 0) {
        printf("No hay estudiantes en el sistema.\n");
        return;
    }

    printf("Total de estudiantes en el sistema: %d\n\n", totalEstudiantes);

    while (beca != NULL) {
        int contador = 0;
        MapPair *pair = marp_first(estudiantes);  // Obtener el primer par del mapa

        while (pair != NULL) {
            Usuario *estudiante = (Usuario *)pair->value;
            // Verificar si el estudiante cumple con los requisitos de la beca
            if (estudiante->socioEconomico >= beca->socioEconomico &&
                estudiante->puntaje >= beca->puntaje &&
                estudiante->notasEM >= beca->notasEM &&
                estudiante->discapacidad == beca->discapacidad &&
                estudiante->originario == beca->originario) {
                contador++;
            }
            pair = map_next(estudiantes);  // Obtener el siguiente par del mapa
        }

        // Calcular el porcentaje de obtención
        double porcentaje = (double)contador / totalEstudiantes * 100;

        // Mostrar los resultados
        printf("Beca: %s\n", beca->nombre);
        printf("Cantidad de usuarios que pudieron acceder: %d\n", contador);
        printf("Porcentaje de obtención: %.2f%%\n\n", porcentaje);

        beca = (tipoBeca *)list_next(becas);  // Avanzar al siguiente elemento de la lista de becas
    }
}
