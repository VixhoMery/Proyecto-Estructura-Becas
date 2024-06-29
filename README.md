# Aplicación de gestion de becas "Beca Track"

## Descripción

La aplicación "BecaTrack" es una aplicación diseñada para la gestión de becas en Chile, permitiendo a estudiantes ingresar sus datos personales y academicos, postularse a becas de su interes y realizar un seguimiento en el estado de apelaciones a becas. Los administradores, utilizando una contraseña especifica, pueden gestionar estudiantes, revisar solicitudes de apelaciones por parte de los estudiantes, administrar becas y realizar un seguimiento de las becas otorgadas. Este sistema está enfocado en optimizar el proceso de selección y gestión de becas, centralizando la información tanto de estudiantes como de las becas disponibles.


## Cómo compilar y ejecutar
Dado que el sistema está diseñado para ser accesible y fácil de probar, recomendamos usar Repl.it para una rápida configuración y ejecución. Sigue estos pasos para comenzar:

1. Visita Repl.it.
2. Crea una nueva cuenta o inicia sesión si ya tienes una.
3. Una vez en tu dashboard, selecciona "New Repl" y elige "Import from GitHub".
4. Pega la URL del repositorio: https://github.com/VixhoMery/Proyecto-Estructura-Becas
5. Repl.it clonará el repositorio y preparará un entorno de ejecución.
6. En la pestaña "Configurate Repl", sección "Configure the Compile Command", escriba o pegue de preferencia el siguiente comando "make -s" y presione "confirm and close". Igualmente se puede configurar dentro del archivo oculto de replit en .replit, cambiar el compile a compile = ["make", "-s"]  
7. Presiona el botón "Run" para compilar y ejecutar la aplicación.


## Funcionalidades Principales.
La aplicación cuenta con dos menús, para el estudiante y para el administrador, el administrador cuenta con una contraseña externa con la cual puede acceder a su menú. (Contraseña menú administrador: admin)

### Funcionalidades para el estudiante:
- Ingresar perfiles personales y académicos.
- Postularse a becas de interés.
- Realizar seguimiento del estado de apelaciones a becas.
  
### Funcionalidades para el administrador:
- Gestión de estudiantes.
- Revisar solicitudes de apelación.
- Gestión de becas.
- Seguimiento de becas.

## Funcionando correctamente.
- Funcion para ingresar perfiles personales y académicos por parte de los estudiantes funciona en un estado optimo, se guardan correctamente los datos de los estudiantes en el hashmap.
- Funcion para postularse a becas de interés esta funcionando correctamente.
- La funcion de realizar seguimiento del estado de apelaciones se encuentra operativa, muestra la apelación hecha por el estudiante con su respectivo estado y justificacion.

- Función de gestionar estudiantes funciona correcta y eficientemente en sus operaciones de agregar estudiantes, actualizar datos de estudiantes y eliminar estudiantes de la tabla hash.
- La función para revisar las solicitudes de apelación por parte de los estudiantes se encuentra operativa, muestra la respectiva apelación con los datos del estudiante, los datos de la beca asi como la justificación de la apelación.
- La función para gestionar becas se encuentra operativa, permite agregar nuevas becas al sistema, modificar los requisitos de becas ya existentes, asi como eliminar becas del sistema.

## Problemas conocidos.
- Función de seguimiento de beca en el menú de administrador, no funciona correctamente. 

## A mejorar.
- Implementar una interfaz más agradable visualmente.
- Implementar un archivo csv para cargar los datos de estudiantes ya existentes, asi como guardar datos de estudiantes luego del tiempo de ejecucion del programa.

### Contribuciones de cada integrante.
- Dario Fuentes: Implementación general de cada sub-menu en el menu administrador y menu usuario, implemento logica inicial para funciones de gestionar estudiantes, gestionar becas, revisar solicitudes de apelación.  
- Vicente Mery: Creación del archivo csv, implementación de la logica para cargar las becas en la aplicación, función de postular becas, creacion del menu basico de la aplicación, implementación de tabla hash.
- Vicente Martinez: Planteamiento inicial de los TDAs a utilizar para la aplicación, implementación de la logica para la funcion de seguimiento de becas, mejoro la tabla hash, creo funciones para cada sub-menu de las funcionalidades con tal de ser visualmente mas ordenado, implemento verificaciones al momento de leer los ruts de los estudiantes, mejoro la función de revisar solicitudes de apelación en el menu administrador.
