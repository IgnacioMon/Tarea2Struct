#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Map.h"
/* TRABAJO REALIZADO POR IGNACIO MONTENEGRO GUERRA - EDUARDO MARIN AVALOS
19.831.791-8
Ramo= Estructura de Datos
Paralelo= 2 */
char * _strdup (const char *s) {
    size_t len = strlen (s) + 1;
    void *new = malloc (len);

    if (new == NULL)
    	return NULL;

    return (char *) memcpy (new, s, len);
}

const char *get_csv_field (char * tmp, int i) {
    char * line = _strdup (tmp);
    const char * tok;
    for (tok = strtok (line, ","); tok && *tok; tok = strtok (NULL, ",\n")) {
        if (!--i) {
            return tok;
        }
    }
    return NULL;
}


long long stringHash(const void * key) {
    long long hash = 5381;

    const char * ptr;

    for (ptr = key; *ptr != '\0'; ptr++) {
        hash = ((hash << 5) + hash) + tolower(*ptr); /* hash * 33 + c */
    }

    return hash;
}

int stringEqual(const void * key1, const void * key2) {
    const char * A = key1;
    const char * B = key2;

    return strcmp(A, B) == 0;
}
typedef struct Cancion{//estructura de la canción y cada una de sus categorias

    char * titulo;

    char * artista;

    char * duracion;

    char * album;

}Cancion;
Cancion * crearCancion(char * linea){
   Cancion* cancioncita =(Cancion*)malloc(sizeof(Cancion));
     cancioncita->titulo = (char*)get_csv_field(linea,1); // Se obtiene el nombre
     cancioncita->artista =   (char*)get_csv_field(linea, 2); // Artista de la cancion
     cancioncita ->duracion = (char*)get_csv_field(linea, 3); // Duracion de la cancion
     cancioncita->album = (char*)get_csv_field(linea, 4); // Album de la cancion
     return cancioncita;
}

int main() {
//se crea los mapas que vamos a utilizar
    Map * song_map = createMap(stringHash, stringEqual);
    Map * album_map = createMap(stringHash,stringEqual);
    Map * artist_map = createMap(stringHash,stringEqual);
    Map * aux_artist = createMap(stringHash, stringEqual);
    Map * aux_album = createMap(stringHash,stringEqual);
    Map * aux_map = createMap(stringHash, stringEqual);
int opcion =0;
FILE * fp=NULL;
char linea[100]; //representa una linea a leer para ser utilizada
Cancion* cancion;
int desea;
char nombre_artista[50];
do{
// se imprime el menú con cada una de sus opciones
    opcion =0;
    printf("|_______________________________________|\n");
    printf("|INGRESE UNA DE LAS SIGUIENTES OPCIONES |\n");
    printf("|1.- Cargar musica                      |\n");
    printf("|2.- Exportar musica                    |\n");
    printf("|3.- Agregar nuevo album                |\n");
    printf("|4.- Agregar nueva cancion              |\n");
    printf("|5.- Eliminar canciones                 |\n");
    printf("|6.- Buscar cancion                     |\n");
    printf("|7.- Buscar cancion de un artista       |\n");
    printf("|8.- Buscar album                       |\n");
    printf("|0.- SALIR                              |\n");
    printf("|_______________________________________|\n");
    scanf("%d",&opcion);
    if (opcion == 1 ){

        fp = fopen("canciones.csv", "r"); //se define fp para utilizar el archivo
        if (fp== NULL){
            printf("Ha ocurrido un error al cargar el archivo");
        }
        while( fgets(linea, 99, fp)!=NULL ){

          cancion = crearCancion(linea);

        //Busqueda en el mapa canciones al no ser encontrada sera agregada
            if(searchMap(song_map, cancion->titulo)==NULL){
                insertMap(song_map,cancion->titulo,cancion);
            }

             //Buscar en el mapa artistas si no se encuentra se agrega
            if(searchMap(artist_map, cancion->artista)==NULL){  // Se busca la cancion en el mapa artistas
                insertMap(aux_artist,cancion->titulo,cancion);
                insertMap(artist_map,cancion->artista, aux_artist);
            }else{
                aux_artist = searchMap(artist_map,cancion->artista);
                insertMap(aux_artist,cancion->titulo,cancion);
            }
            //Buscar en el  mapa album si no se encuentra se agrega
            if (searchMap(album_map,cancion->album)== NULL){
                insertMap(aux_album,cancion->titulo, cancion);
                insertMap(album_map,cancion->album, aux_album);
            }else{
                aux_album =searchMap(album_map,cancion->album);
                insertMap(aux_album,cancion->titulo,cancion);
            }


        }
         printf("\n Se ha cargado la musica \n");
        fclose(fp);
    }

    if (opcion == 2){//si se selecciona la opción de exportar  musica
        fp = fopen("canciones.csv", "w");
        cancion = firstMap(song_map);
        while(cancion!=NULL){
            fprintf("canciones.csv","%s, %s, %s, %s \n", cancion->titulo, cancion->artista, cancion->duracion, cancion->album);
            cancion = nextMap(song_map);
        }
        fclose(fp);
    }

    if (opcion == 3){ //opcion para agregar un nuevo álbum
        char nombre_album[100];
        char nombre_cancion[100];
        cancion = NULL;
        printf("\n Ingresar el nombre del nuevo album \n");
        getchar();
        scanf("%90[^\n]s", nombre_album);
        getchar();

        if (searchMap(album_map,nombre_album)== NULL){
            printf("\n Album inexistente, Se agregara \n");
            printf("\n Ingresar cancion nueva? |Si-1 // No-2| \n");
            scanf("%d", &desea);
            getchar();
            while (desea==1){
                    printf("Ingresar el titulo causara el único error desconocido \n");
                    printf("Inarreglable tras varias pruebas, no pudimos encontrar error \n");
                    printf("Se valora revisar codigo \n");
                    printf("\n Titulo \n");
                    getchar();
                    scanf("%90[^\n]s",cancion->titulo);
                    getchar();
                    printf("%s", cancion->titulo);
                    getchar();
                    aux_map=searchMap(song_map,cancion->titulo);
                    if(aux_map==NULL){
                        printf(" Artista \n");
                        getchar();
                        scanf("%90[^\n]s", cancion->artista);
                        cancion->album =nombre_album;
                        printf("Duracion \n");
                        getchar();
                        scanf("%90[^\n]s", cancion->duracion);
                        getchar();

                        insertMap(song_map,cancion->titulo,cancion);
                        insertMap(aux_artist,cancion->titulo,cancion);
                        insertMap(artist_map,cancion->artista, aux_artist);
                        insertMap(aux_album,cancion->titulo, cancion);
                        insertMap(album_map,cancion->album, aux_album);

                    }else{
                        printf("\n Cancion ya existente \n");
                    }

                    printf("\n ¿Agragar nueva cancion al album? |Si-1 // No-0|  \n");
                    scanf("%d", &desea);
            }
        }else{
            printf("\nEl album ya existe \n");
        }

    }

    if (opcion == 4){//agregar una nueva canción
        printf("Porfavor ingresar los datos de la cancion \n");
        printf("Separando los valores con una coma. \n");
        printf("Ejemplo: Nombre,Artista,Dura:cion,álbum \n");
        scanf("%s", linea);
        cancion = crearCancion(linea);
        if (searchMap(song_map,cancion->titulo) ==NULL){
             insertMap(song_map,cancion->titulo,cancion);
             Map * aux_artist = createMap(stringHash, stringEqual);
             //Buscar en el mapa artistas si no se encuentra se agrega
            if(searchMap(artist_map, cancion->artista)==NULL){  // Se busca la cancion en el mapa artistas
                insertMap(aux_artist,cancion->titulo,cancion);
                insertMap(artist_map,cancion->artista, aux_artist);
            }else{
                    aux_artist = searchMap(artist_map,cancion->artista);
                    insertMap(aux_artist,cancion->titulo,cancion);
            }
            Map * aux_album = createMap(stringHash,stringEqual);
            //Buscar en el  mapa album si no se encuentra se agrega
            if (searchMap(album_map,cancion->album)== NULL){
                insertMap(aux_album,cancion->titulo, cancion);
                insertMap(album_map,cancion->album, aux_album);
            }else{
                aux_album =searchMap(album_map,cancion->album);
                insertMap(aux_album,cancion->titulo,cancion);
            }
        }
    }
    if (opcion == 5){
        printf("Ingresar nombre del artista a borrar \n");
        getchar();
        scanf("%90[^\n]s", nombre_artista);
        Map * aux_map = createMap(stringHash, stringEqual);//se crea un nuevo para con el nombre de aux_map

        //al mapa le asigno el mapa que se devuelve de la funcion
        aux_map = searchMap(artist_map,nombre_artista);

        //creo tipo de dato cancion
        Cancion* cancion;

        //almaceno cancion del "mapa canciones del artista"
        cancion = firstMap(aux_map);

        //recorro cancion por cancion
        while(cancion!=NULL){
            //elimino cancion de song_map
            eraseKeyMap(song_map,cancion->titulo);
            //elimino cancion de album_map
            eraseKeyMap(searchMap(album_map,cancion->album),cancion->titulo);
            //paso al siguiente elemento del "mapa canciones del artista"
            cancion = nextMap(aux_map);
        }
        //borro del mapa artistas el artista pasado por parámetro
        removeAllMap(searchMap(artist_map,nombre_artista));
        printf("El artista fue eliminado");

    }

    if (opcion == 6){
        printf("Ingresar el nombre de cancion a buscar \n");
        getchar();
        scanf("%90[^\n]s", linea);
        if(searchMap(song_map,linea)!= NULL){//si la cancan existe se imprime cada de sus categorias
            cancion = searchMap(song_map,linea);
            printf(" Titulo :%s \n Artista : %s \n Duracion : %s \n Album : %s \n", cancion->titulo, cancion->artista, cancion->duracion, cancion->album);
        }else{
            printf("La cancion no se encuentra disponible \n");
             }
    }
    if (opcion == 7){
            int num=1;
            printf("Ingrese nombre del artista a buscar\n");
            getchar();
            scanf("%90[^\n]s", nombre_artista);//se busca el nombre del artista
            if (searchMap(artist_map, nombre_artista)== NULL){// si no se encuentra se imprimirán que no esta disponible
                printf("No se encuentra disponible\n");
            }else{
                aux_map = searchMap(artist_map,nombre_artista);
                cancion = firstMap(aux_map);
                printf("Canciones del artista %s : \n ", nombre_artista);
                while (cancion != NULL){//imprimir mientras exista una canción
                    printf("Cancion num%d -> %s \n", num,cancion->titulo);
                    num++;
                    cancion = nextMap(aux_map);
                }
            }
    }

    if (opcion == 8){
            int num1=1;
            printf("Ingresar nombre del album \n");
            getchar();
            scanf("%90[^\n]s", linea);
            if (searchMap(album_map,linea) == NULL){
                printf("Album no disponible \n");
            }else{
                aux_map = searchMap(album_map,linea);
                cancion = firstMap(aux_map);
                printf("Canciones del album %s : \n", cancion->album);
                while (cancion != NULL){
                    printf("Cancion num%d -> %s \n", num1, cancion->titulo);
                    num1++;
                    cancion = nextMap(aux_map);
                }
            }
    }


}while (opcion != 0);
    return 0;
}
