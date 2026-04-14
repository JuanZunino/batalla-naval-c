#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<time.h>
#include <stdbool.h>
#define fila 10
#define columna 10
#define cantidadBarcos 5

// celda de matriz
typedef struct{
    char imprimir;
    bool hayBarco;
    bool yaDispararon;
    int idBarco;
}celda;

// registro jugador
typedef struct{
     int dni;
     char Nombre[20];
     char Apellido[20];
     int victorias;
     celda Mapa[fila][columna];             //matrices de tipo celda
     celda mapaAtaque[fila][columna];
     int cantBarcos;
     int vidasBarcos[cantidadBarcos]; // llevo un conteo de la cantidad de vidas (tamaño) que les queda a cada tipo de barco
}jugador;

typedef struct{
    char nombre[20];
    int tamanio;
}Barco;

typedef struct Nodo{
    jugador dato;
    struct Nodo* siguiente;
}Nodo;

void inicializarMapa(celda Mapa[fila][columna],celda mapaAtaque[fila][columna]);
void cargarBarco(jugador* jugador1,Barco tipoBarco[cantidadBarcos]);
void cargarbarcoMaquina(jugador* maquina, Barco tipoBarco[cantidadBarcos]);
void cargarDatosjugador(jugador* j, bool esMaquina);
void imprimirBarcos(celda Mapa[fila][columna]);
void cargarTodo(jugador* jugador1, jugador* jugador2);
void disparoMaquina(jugador* atacante, jugador* defensor,Barco tipoBarco[cantidadBarcos]);
void guardarHistorialPartida(jugador ganador, char* perdedor, char* modo);
void disparos(jugador* atacante, jugador* defensor,Barco tipoBarco[cantidadBarcos]);
void unJugador(Nodo** L,jugador* jugador1, jugador* maquina,Barco tipoBarco[cantidadBarcos]);
void dosjugadores(Nodo** L, jugador* jugador1, jugador* jugador2,Barco tipoBarco[cantidadBarcos]);
void insertarOrdenado(Nodo** L,jugador j);
Nodo* buscarJugador(Nodo *L,int DNI);
void eliminarNodo(Nodo **L, int DNI);
void mostrarHistorialPartidas();
void mostrarRanking(Nodo *L);
void guardarRanking(Nodo* L);
void cargarRanking(Nodo** L);
bool barcosVivos(jugador* j);
void modoJuego(Nodo** L,jugador* jugador1,jugador* jugador2, Barco tipoBarco[cantidadBarcos]);
void menuPrincipal(Nodo** L);
int pedirEntero(const char* mensaje, int minimo, int maximo);

//main principal
int main(){
    srand(time(NULL));
    Nodo* L=NULL;
    cargarRanking(&L); //carga el ranking al iniciar el programa
    menuPrincipal(&L);
    guardarRanking(L); //guarda el ranking al salir del menú
    return 0;
}

//aca cargo las celdas con agua y hago el limite de la matriz de los 2 mapas el de carga de barcos y el de ataque
void inicializarMapa(celda Mapa[fila][columna], celda mapaAtaque[fila][columna]) {
    // Imprimir encabezado de columnas
    printf("   ");
    for (int j = 0; j < columna; j++) {
        printf(" %d", j);
    }
    printf("\n");

    for (int i = 0; i < fila; i++) {
        // Imprimir número de fila con formato
        printf("%2d ", i);

        for (int j = 0; j < columna; j++) {
            Mapa[i][j].hayBarco = false;
            Mapa[i][j].imprimir = '~';
            Mapa[i][j].yaDispararon = false;
            Mapa[i][j].idBarco=-1;
            mapaAtaque[i][j].hayBarco = false;
            mapaAtaque[i][j].imprimir = '~';
            mapaAtaque[i][j].yaDispararon = false;

            printf(" %c", Mapa[i][j].imprimir);
        }
        printf("\n");
    }
}


//creo un barco y pongo hay barco en true
void crearBarco(celda* C, int idBarco) {

    C->imprimir = 'B';
    C->hayBarco = true;
    C->yaDispararon = false;
    C->idBarco = idBarco;   // Esto permite ver qué barco es

}


//cargo los 5 barcos disponibles en la matriz
void cargarBarco(jugador* jugador1,Barco tipoBarco[cantidadBarcos]){
celda C;
int orientacion;    // 1 vertical  2 horizontal
int posF,posC;
bool valido;
  for (int i = 0; i < cantidadBarcos; i++) {
        int largo = tipoBarco[i].tamanio;
        do {
            printf("\nUbicando barco: %s (tamanio %d)\n", tipoBarco[i].nombre, largo);
            posF = pedirEntero("Ingrese fila inicial: ", 0, fila-1);
            posC = pedirEntero("Ingrese columna inicial: ", 0, columna-1);
            orientacion = pedirEntero("Ingrese orientacion (1 = Vertical, 2 = Horizontal): ", 1, 2);
            valido = true;

            // pregunto si el barco entra en el mapa o es valida la posicion

           if ((orientacion == 1 && (posF < 0 || posF + largo > fila || posC < 0 || posC >= columna)) || (orientacion == 2 && (posC < 0 || posC + largo > columna || posF < 0 || posF >= fila))) {
                valido = false;
              } else {

            // me fijo si no hay barco en las posiciones donde lo voy a ubicar

                 for(int j=0; j<largo; j++){
                    if(orientacion==1){  // vertical
                        if(jugador1->Mapa[posF+j][posC].hayBarco==true){
                            valido=false;
                        }
                    } else {            // horizontal
                        if(jugador1->Mapa[posF][posC+j].hayBarco==true){
                            valido=false;
                        }
                    }
                 }
              }

            if (!valido) {
                printf("¡Posicion invalida o se superpone con otro barco! Intente de nuevo.\n\n");
            }

         }while(!valido);

        // Colocar el barco en el mapa
        for (int j = 0; j < largo; j++) {
            if (orientacion == 1) { // Vertical
                crearBarco(&jugador1->Mapa[posF + j][posC],i);  //le mando el mapa donde lo crea y el id del barco para saber cual es
            } else { // Horizontal
                crearBarco(&jugador1->Mapa[posF][posC + j],i);
            }
        }
       jugador1->vidasBarcos[i] = largo;  // cargo en el vector

system("cls");
imprimirBarcos(jugador1->Mapa);  // imprimo para mostrar

    }

}


void cargarbarcoMaquina(jugador* maquina, Barco tipoBarco[cantidadBarcos]) {
    int posF, posC;
    int orientacion; // 1: vertical, 2: horizontal
    bool valido;
    int largo;

    for (int i = 0; i < cantidadBarcos; i++) {
        largo = tipoBarco[i].tamanio;

        do {
            posF = rand() % fila;
            posC = rand() % columna;
            orientacion = (rand() % 2) + 1; // 1 o 2
            valido = true;
            if ((orientacion == 1 && (posF < 0 || posF + largo > fila || posC < 0 || posC >= columna)) || (orientacion == 2 && (posC < 0 || posC + largo > columna || posF < 0 || posF >= fila))) {
                valido = false;
              } else {

            // me fijo si no hay barco en las posiciones donde lo voy a ubicar

                 for(int j=0; j<largo; j++){
                    if(orientacion==1){  // vertical
                        if(maquina->Mapa[posF+j][posC].hayBarco==true){
                            valido=false;
                        }
                    } else {  // horizontal
                        if(maquina->Mapa[posF][posC+j].hayBarco==true){
                            valido=false;
                        }
                    }
                 }
              }
          } while (!valido);

              // Colocar el barco en el mapa
        for (int j = 0; j < largo; j++) {
            if (orientacion == 1) { // Vertical
                crearBarco(&maquina->Mapa[posF + j][posC],i);  //le mando el mapa donde lo crea y el id del barco para saber cual es
            } else { // Horizontal
                crearBarco(&maquina->Mapa[posF][posC + j],i);
            }
        }
        maquina->vidasBarcos[i] = largo;
        system("cls");
    }
  printf("Todos los barcos de la maquina fueron cargados correctamente\n");
}


void cargarDatosjugador(jugador* j, bool esMaquina) {
    bool valido = false;
    if(!esMaquina) {
        printf("Por favor, complete los datos de identificacion.\n\n");

        j->dni = pedirEntero("DNI: ", 1000000, 99999999);

        printf("Nombre: ");
        fgets(j->Nombre, sizeof(j->Nombre), stdin);
        j->Nombre[strcspn(j->Nombre, "\n")] = '\0';

        printf("Apellido: ");
        fgets(j->Apellido, sizeof(j->Apellido), stdin);
        j->Apellido[strcspn(j->Apellido, "\n")] = '\0';
    } else {
        // Datos para la máquina
        strcpy(j->Nombre, "Máquina");
        strcpy(j->Apellido, "CPU");
        j->dni = 0;  // DNI ficticio
    }

    // Campos comunes para ambos
    j->cantBarcos = cantidadBarcos;
    j->victorias = 0;
}

void insertarOrdenado(Nodo** L, jugador j) {
    Nodo* act = *L;
    Nodo* ant = NULL;
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevo == NULL) {
        printf("Error: no se pudo reservar memoria\n");
        exit(1);
    }
    nuevo->dato = j;
    nuevo->siguiente = NULL;  // Inicializar puntero para evitar problemas

    while (act != NULL && act->dato.victorias > j.victorias) {
        ant = act;
        act = act->siguiente;
    }
    if (ant == NULL) {
        nuevo->siguiente = act;
        *L = nuevo;
    } else {
        ant->siguiente = nuevo;
        nuevo->siguiente = act;
    }
}



// imprimo los barcos que voy cargando
void imprimirBarcos(celda Mapa[fila][columna]) {
    // Encabezado de columnas
printf("   ");
 for (int j = 0; j < columna; j++) {
    printf(" %d", j);
 }
 printf("\n");

 for (int i = 0; i < fila; i++) {
    // Número de fila al inicio de cada línea
    printf("%2d ", i);

    for (int j = 0; j < columna; j++) {
            printf(" %c", Mapa[i][j].imprimir);
    }
     printf("\n");
 }

}

void disparoMaquina(jugador* atacante, jugador* defensor,Barco tipoBarco[cantidadBarcos]) {
    int posF, posC;
    do {
        posF = rand() % fila;
        posC = rand() % columna;
    } while (defensor->Mapa[posF][posC].yaDispararon == true);
    printf("La maquina dispara en la posicion Fila %d - Columna %d\n", posF, posC);
    if (defensor->Mapa[posF][posC].hayBarco == true) {
        int id = defensor->Mapa[posF][posC].idBarco;
        defensor->vidasBarcos[id]--;

        defensor->Mapa[posF][posC].yaDispararon = true;
        defensor->Mapa[posF][posC].imprimir = 'X';          // Muestra que esa parte fue impactada

        atacante->mapaAtaque[posF][posC].hayBarco = true;
        atacante->mapaAtaque[posF][posC].yaDispararon = true;
        atacante->mapaAtaque[posF][posC].imprimir = 'B';

        printf("Impacto!\n\n");
        if (defensor->vidasBarcos[id] == 0) {
            printf("La maquina hundio tu %s!\n", tipoBarco[id].nombre);
            defensor->cantBarcos--;
        }



    } else {
        defensor->Mapa[posF][posC].yaDispararon = true;
        atacante->mapaAtaque[posF][posC].yaDispararon = true;
        atacante->mapaAtaque[posF][posC].imprimir = 'X';
        printf("Agua...\n");
    }
    printf("\n\n\n\nCargando...");
    sleep(3);
    system("cls");
    printf("Mapa de la maquina actualizado: \n\n------------------------------------------------------\n\n");
    imprimirBarcos(atacante->mapaAtaque);
    printf("\n");
}


// procedimiento de disparo con 2 matriz la mia y la del jugador rival
void disparos(jugador* atacante, jugador* defensor,Barco tipoBarco[cantidadBarcos]){
int posF,posC;

    printf("\n\nHora de Atacar!! \n\n");
    printf("Ingrese la posicion a disparar\n\n");
    posF = pedirEntero("Fila: ", 0, fila-1);
    posC = pedirEntero("Columna: ", 0, columna-1);


   while( posF < 0 || posF >= fila || posC < 0 || posC >= columna || defensor->Mapa[posF][posC].yaDispararon == true ) {
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        if (posF >= 0 && posF < fila && posC >= 0 && posC < columna && defensor->Mapa[posF][posC].yaDispararon == true) {
            printf("\n Esa posicion ya fue atacada. Elegi una diferente.\n");
        } else {
                printf("\n La posicion esta fuera del mapa. Ingrese nuevamente.\n");
        }
        posF = pedirEntero("Fila: ", 0, fila-1);
        posC = pedirEntero("Columna: ", 0, columna-1);
    }

   if (defensor->Mapa[posF][posC].hayBarco) {
        int id = defensor->Mapa[posF][posC].idBarco;
        defensor->vidasBarcos[id]--;

        defensor->Mapa[posF][posC].yaDispararon = true;
        defensor->Mapa[posF][posC].imprimir = 'X';

        atacante->mapaAtaque[posF][posC].hayBarco = true;
        atacante->mapaAtaque[posF][posC].yaDispararon = true;
        atacante->mapaAtaque[posF][posC].imprimir = 'B';

    if (defensor->vidasBarcos[id] == 0) {
        system("cls");
        printf("Hundiste el barco: %s!\n", tipoBarco[id].nombre);
        printf("Presione ENTER para continuar...\n");
        while (getchar() != '\n');  // espera hasta que se presione ENTER
    }

    defensor->cantBarcos--;

    } else{
        defensor->Mapa[posF][posC].yaDispararon=true; // sino marco en el mapa del que esta siendo atacado que dispararon en esa posicion
        atacante->mapaAtaque[posF][posC].yaDispararon=true; // y marco en el mapa de ataque del que disparo que ya dispare ahi
        atacante->mapaAtaque[posF][posC].imprimir='X'; // marco en el mapa de ataque del que disparo una X asi se ve que ya dispare
    }

    system("cls"); // limpio consola
    printf("Mapa de Ataque de jugador: %s %s: \n\n\n\n------------------------------------------------------\n\n",atacante->Apellido,atacante->Nombre);
    imprimirBarcos(atacante->mapaAtaque); // imprimo los barcos de el mapa que esta disparando en la parte mapa ataque asi el que ataca va viendo si ya disparo en ese lugar o no y si hay barco o agua



}

Nodo* buscarJugador(Nodo *L,int DNI){ // recorro la lista y me fijo si el dni del jugador ya esta cargado y devuelvo el nodo o NULL
Nodo* act=L;
    while((act!=NULL)&&(act->dato.dni!= DNI)){
        act=act->siguiente;
    }
    if(act!=NULL){                  //si el dni existe devuelvo el nodo
       return act;
    } else{
           return NULL;               // sino existe retorno nULL
    }
}

void eliminarNodo(Nodo** L,int DNI){  //elimino el jugador para luego insertarlo ordenado por numero de victorias actualizado
Nodo* ant=NULL;
Nodo* act=*L;
    while(act!=NULL && act->dato.dni!=DNI){  //mientras sea diferente a null o no encuentre el dni recorro
        ant=act;
        act=act->siguiente;
    }
    if(act!=NULL){      // si encontre el dni
        if(ant==NULL){ // si el dni que encontre esta al principio de la lista
            *L=act->siguiente;  //actualizo cabeza de lista
        } else{
               ant->siguiente=act->siguiente;  // sino el anterior lo apunto al siguiente del que voy a eliminar
        }
        free(act); //libero memoria (elimino)
    }
}


// recorro las vidas de todos los barcos para ver si termine
bool barcosVivos(jugador* j) {
    for (int i = 0; i < cantidadBarcos; i++) {
        if (j->vidasBarcos[i] > 0) return true;
    }
    return false;
}

// Guarda el historial de cada partida individual con fecha y hora
void guardarHistorialPartida(jugador ganador, char* perdedor, char* modo) {
    FILE* archivo = fopen("historial_partidas.txt", "a");
    if (archivo == NULL) {
        printf("Error: No se pudo guardar el historial\n");
        return;
    }

    time_t t = time(NULL);
    struct tm* fecha = localtime(&t);

    fprintf(archivo, "==========================================\n");
    fprintf(archivo, "Fecha: %02d/%02d/%04d - %02d:%02d\n",
            fecha->tm_mday, fecha->tm_mon + 1, fecha->tm_year + 1900,
            fecha->tm_hour, fecha->tm_min);
    fprintf(archivo, "Modo: %s\n", modo);
    fprintf(archivo, "Ganador: %s %s (DNI: %d)\n",
            ganador.Apellido, ganador.Nombre, ganador.dni);
    fprintf(archivo, "Perdedor: %s\n", perdedor);
    fprintf(archivo, "==========================================\n\n");

    fclose(archivo);
    printf("Partida guardada en historial\n");
}



void unJugador(Nodo** L,jugador* jugador1, jugador* maquina,Barco tipoBarco[cantidadBarcos]){ //modo 1 jugador vs maquina
    bool termino=false;
    Nodo* nodoGanador;
    while(termino==false){
        printf("turno de: %s \n\n",jugador1->Nombre);
        imprimirBarcos(jugador1->mapaAtaque);  //imprimo el mapa para ver donde disparar

        disparos(jugador1,maquina,tipoBarco);
        printf("\nTurno finalizado. Presione ENTER y pase al siguiente jugador...");
        while (getchar() != '\n');  // espera hasta que se presione ENTER
        system("cls");  // Limpia la pantalla
        if(!barcosVivos(maquina)){           // si maquina se quedo si barcos
            termino=true;                      // termine = true termina el juego gano jugador1
        }else{                                 //sino
            printf("turno de la Maquina \n\n");
            disparoMaquina(maquina,jugador1,tipoBarco);
            if(!barcosVivos(jugador1)){
                termino=true;
            }

        }

}
    if (termino && barcosVivos(jugador1)) {
        char perdedor[] = "Maquina CPU";
        char modo[] = "1 Jugador";
        sleep(1);
        system("cls");
        printf("Juego finalizado ganaste!\n");

        nodoGanador = buscarJugador(*L, jugador1->dni);
        if (nodoGanador == NULL) {
            jugador1->victorias++;
            insertarOrdenado(L, *jugador1);
        } else {
                jugador copia = nodoGanador->dato;
                copia.victorias++;
                eliminarNodo(L, copia.dni);
                insertarOrdenado(L, copia);
        }

    guardarHistorialPartida(*jugador1, perdedor, modo);

    } else if (termino && !barcosVivos(jugador1)) {
        sleep(2);
        system("cls");
        printf("Juego finalizado perdiste!\n");

        char perdedor[50];
        sprintf(perdedor, "%s %s", jugador1->Apellido, jugador1->Nombre);
        char modo[] = "1 Jugador";

        jugador maquinaGanadora;
        strcpy(maquinaGanadora.Nombre, "Maquina");
        strcpy(maquinaGanadora.Apellido, "CPU");
        maquinaGanadora.dni = 0;

        guardarHistorialPartida(maquinaGanadora, perdedor, modo);
    }

        printf("Presione ENTER para volver al menu principal...\n");
        while (getchar() != '\n');  // espera hasta que se presione ENTER
    }


void dosjugadores(Nodo** L, jugador* jugador1, jugador* jugador2,Barco tipoBarco[cantidadBarcos]){ //modo de juego 2 jugadores
    bool termino=false;
    Nodo* nodoGanador;
    while(termino==false){     //mientras no termine
        printf("turno de: %s \n\n",jugador1->Nombre);
        imprimirBarcos(jugador1->mapaAtaque);  //imprimo el mapa para ver donde disparar
        disparos(jugador1,jugador2,tipoBarco);           //procedimiento de disparos ataca jugador 1
        printf("\nTurno finalizado. Presione ENTER y pase al siguiente jugador...\n");
        while (getchar() != '\n');  // espera hasta que se presione ENTER
        system("cls");  // Limpia la pantalla

        if(!barcosVivos(jugador2)){           // si jugador 2 se quedo si barcos
            termino=true;                      // termine = true termina el juego gano jugador1
        }else{                                 //sino
            printf("turno de: %s \n\n",jugador2->Nombre);
            imprimirBarcos(jugador2->mapaAtaque);  //imprimo el mapa para ver donde disparar
            disparos(jugador2,jugador1,tipoBarco);           //procedimiento de disparos ataca jugador 2
            printf("\nTurno finalizado. Presione ENTER y pase al siguiente jugador...\n");
            while (getchar() != '\n');  // espera hasta que se presione ENTER
            system("cls");  // Limpia la pantalla
            if(jugador1->cantBarcos==0){           // si jugador 1 se quedo si barcos
                termino=true;                      // termine = true termina el juego gano jugador2
            }
        }
    }
    if(!barcosVivos(jugador1)){
        printf("Juego finalizado \n\n El ganador es el jugador:  %s %s\n\n",jugador2->Apellido,jugador2->Nombre);  //si jugador 1 se queda sin barcos imprimo ganador jugador 2

        char perdedor[50];
        sprintf(perdedor, "%s %s", jugador1->Apellido, jugador1->Nombre);
        char modo[] = "2 Jugadores";



       nodoGanador=buscarJugador(*L,jugador2->dni);                //busco el nodo del jugador si ya esta en la lista de jugadores(ranking)
       if(nodoGanador==NULL){                                     //si nodoganador == null no encontro jugador osea que no esta cargado en la lista de jugadores pq es nuevo
             jugador2->victorias++;                              // sumo 1 victoria al nuevo ganador
            insertarOrdenado(L,*jugador2);                      //lo cargo en la lista
       } else{                                                  // sino
            jugador copia=nodoGanador->dato;                    // copio el nodoganador en un registro poreç valor asi no tengo problemas al eliminar
            copia.victorias++;                                  //sumo victorias
            eliminarNodo(L,copia.dni);                          //elimino el jugador de l a lista utilizando el registor copia en la parte dni asi si lo encuentro lo elimino
            insertarOrdenado(L,copia);                          // lo inserot nuevamente ordenado con el registro copia ya que el otro puntero jugador 2 ya se elimino
       }


       // Guardar historial
        guardarHistorialPartida(*jugador1, perdedor, modo);

    }else if(!barcosVivos(jugador2)){
          printf("Juego finalizado \n\n El ganador es el jugador:  %s %s\n\n",jugador1->Apellido,jugador1->Nombre);  //si jugador 2 se queda sin barcos imprimo ganador jugador 1

          char perdedor[50];
          sprintf(perdedor, "%s %s", jugador2->Apellido, jugador2->Nombre);
          char modo[] = "2 Jugadores";


          nodoGanador=buscarJugador(*L,jugador1->dni);      //busco el nodo del jugador si ya esta en la lista de jugadores(ranking)
          if(nodoGanador==NULL){                            //si nodoganador == null no encontro jugador osea que no esta cargado en la lista de jugadores pq es nuevo
               jugador1->victorias++;                        // sumo 1 victoria al nuevo ganador
               insertarOrdenado(L,*jugador1);               //lo cargo en la lista
          } else{                                           // sino
                 jugador copia=nodoGanador->dato;           // copio el nodoganador en un registro poreç valor asi no tengo problemas al eliminar
                 copia.victorias++;                         //sumo victorias
                 eliminarNodo(L,copia.dni);                 //elimino el jugador de l a lista utilizando el registor copia en la parte dni asi si lo encuentro lo elimino
                 insertarOrdenado(L,copia);                 // lo inserot nuevamente ordenado con el registro copia ya que el otro puntero jugador 2 ya se elimino
          }

          // Guardar historial
            guardarHistorialPartida(*jugador1, perdedor, modo);

   printf("Presione ENTER para volver al menu principal...\n");
   while (getchar() != '\n');  // espera hasta que se presione ENTER
}

}

// Muestra todo el historial de partidas guardado
void mostrarHistorialPartidas() {
    FILE* archivo = fopen("historial_partidas.txt", "r");
    if (archivo == NULL) {
        printf("No hay historial de partidas disponible.\n\n");
        printf("Presione ENTER para volver...\n");
        while (getchar() != '\n');  // espera hasta que se presione ENTER

    }

    printf("========== HISTORIAL DE PARTIDAS ==========\n\n");

    char linea[200];
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        printf("%s", linea);
    }

   fclose(archivo);

    printf("Presione ENTER para volver al menu principal...");
    while (getchar() != '\n');  // espera hasta que se presione ENTER
}

void mostrarRanking(Nodo* L) {
    Nodo* act = L;
    int posicion = 1;

    printf("\n\n====================== RANKING DE JUGADORES ======================\n");
    printf("%-6s %-20s %-20s %-12s %-10s\n", "Puesto", "Apellido", "Nombre", "DNI", "Victorias");
    printf("--------------------------------------------------------------------------\n");

    while (act != NULL) {
        printf("%-6d %-20s %-20s %-12d %-10d\n",
               posicion,
               act->dato.Apellido,
               act->dato.Nombre,
               act->dato.dni,
               act->dato.victorias);

        posicion++;
        act = act->siguiente;
    }

    printf("==========================================================================\n\n");
    printf("Presione ENTER para volver al menu principal...");
    while (getchar() != '\n');  // espera hasta que se presione ENTER

}

void guardarRanking(Nodo* L) {
    FILE* archivo = fopen("ranking.txt", "w"); //abre (o crea si no existe) un archivo llamado 'ranking.txt' en modo escritura
    if (archivo == NULL) {
        printf("Error al guardar el ranking.\n");
        return;
    }

    Nodo* act = L;
     while (act != NULL) {
        // Guardar los datos separados por '|'
        fprintf(archivo, "%d|%s|%s|%d\n",
                act->dato.dni,
                act->dato.Nombre,
                act->dato.Apellido,
                act->dato.victorias);
        act = act->siguiente;
    }

    fclose(archivo);
}

void cargarRanking(Nodo** L) {
    FILE* archivo = fopen("ranking.txt", "r");  // Abre archivo para lectura
    if (archivo == NULL) {
        return; // Puede ser la primera vez que se ejecuta
    }

    jugador j;
    char linea[200];

    while (fgets(linea, sizeof(linea), archivo) != NULL) {   //lee hasta encontrar un \n o que se llene  el vector linea
        // Leer con separador '|', permitiendo espacios
        if (sscanf(linea, "%d|%[^|]|%[^|]|%d", &j.dni, j.Nombre, j.Apellido, &j.victorias) == 4) {   // lee dni y lo separo con | asi tengo un limite. despues leo caracteres hasta la proxima | (%[^|])
            j.cantBarcos = cantidadBarcos;
            insertarOrdenado(L, j);
        }
    }

    fclose(archivo);
}

int pedirEntero(const char* mensaje, int minimo, int maximo) {
    char input[20];
    int numero;

    while (1) {
        printf("%s", mensaje);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (sscanf(input, "%d", &numero) == 1 && numero >= minimo && numero <= maximo) {
            return numero;
        } else {
            printf("\nEntrada invalida. Ingrese un numero entre %d y %d.\n\n", minimo, maximo);
        }
    }
}

 //seleccionar modo de juego 1 jugador o 2 jugadores
 void modoJuego(Nodo** L,jugador* jugador1,jugador* jugador2, Barco tipoBarco[cantidadBarcos]) {

    int opcionModo;
    bool volverMenu = false;


    while (!volverMenu) {
        system("cls");
        printf("\n--- Seleccionar modo de juego ---\n");
        printf("1. Modo 1 jugador\n");
        printf("2. Modo 2 jugadores\n");
        printf("3. Volver al menu principal\n\n");
        printf("Seleccione una opcion: ");

        char input[10];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (sscanf(input, "%d", &opcionModo) != 1) {
        printf("- - - Entrada invalida. Debe ingresar un numero del 1 al 3 - - -");
        sleep(2);
        continue;
        }

        switch (opcionModo) {
            case 1:  // Modo 1 jugador
                system("cls");
                printf("Iniciando modo 1 jugador...\n");
                cargarDatosjugador(jugador1, false);  // Jugador humano
                inicializarMapa(jugador1->Mapa, jugador1->mapaAtaque);
                cargarBarco(jugador1,tipoBarco);
                system("cls");

                cargarDatosjugador(jugador2, true);  // Máquina
                inicializarMapa(jugador2->Mapa, jugador2->mapaAtaque);
                cargarbarcoMaquina(jugador2,tipoBarco);
                unJugador(L, jugador1, jugador2,tipoBarco);
                break;
            case 2:  // Modo 2 jugadores
                system("cls");
                printf("Iniciando modo 2 jugadores...\n");
                cargarDatosjugador(jugador1, false);  // Jugador 1
                inicializarMapa(jugador1->Mapa, jugador1->mapaAtaque);
                cargarBarco(jugador1,tipoBarco);
                system("cls");

                cargarDatosjugador(jugador2, false);  // Jugador 2 (humano)
                inicializarMapa(jugador2->Mapa, jugador2->mapaAtaque);
                cargarBarco(jugador2,tipoBarco);
                dosjugadores(L, jugador1, jugador2,tipoBarco);
                break;
            case 3:
               volverMenu = true;                    // Sale del modoJuego y vuelve al menuPrincipal
                break;
            default:
                printf("Opcion invalida\n");
                sleep(1);
        }
    }
}




void menuPrincipal(Nodo** L){  //menu principal
jugador jugador1; jugador jugador2; // de tipo jugador tengo jugador 1 y jugador 2
int opcion;
int DNI;
bool salir=false;

Barco tipoBarco[cantidadBarcos] = {    //declaro vector de tipoBarco con los diferentes nombres de barcos y tamaños
    {"Lancha",1},
    {"Submarino",2},
    {"Destructor",3},
    {"Acorazado",4},
    {"Portaaviones",5}
};



  while(salir==false){
    system("cls");
    printf("+======================================+\n");
    printf("|        B A T A L L A   N A V A L     |\n");
    printf("|         M E N U   P R I N C I P A L  |\n");
    printf("+======================================+\n");
    printf("| 1. > Seleccionar modo de juego       |\n");
    printf("| 2. > Ver Ranking de jugadores        |\n");
    printf("| 3. > Ver historial de partidas       |\n");
    printf("| 4. > Salir                           |\n");
    printf("+======================================+\n");

    printf("\nSeleccione una opcion: ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (sscanf(input, "%d", &opcion) != 1) {
        printf("- - - Entrada invalida. Debe ingresar un numero del 1 al 4 - - -");
        sleep(2);
    continue;
    }

    switch(opcion){
        case 1:
            system("cls");
            modoJuego(L,&jugador1,&jugador2,tipoBarco);
            system("cls");
        break;

        case 2:
            system("cls");
            mostrarRanking(*L);
            system("cls");
        break;

        case 3:
            system("cls");
            mostrarHistorialPartidas();
            system("cls");
        break;

        case 4:
            salir = true;
        break;

        default:
            printf("opcion invalida");
            sleep(1);
            system("cls");
    }

  }
}

