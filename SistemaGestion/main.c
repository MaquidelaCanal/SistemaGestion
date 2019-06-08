#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "gotoxy.h"

#define archivoUsuarios "arUsuarios.dat"
#define archivoProductos "arProductos.dat"
#define archivoVentas "arVentas.dat"

#define ESC 27

#define CODADMIN 123
#define TIPOADMIN 1
#define TIPONORMAL 0

typedef struct
{
    int Id;
    int Tipo;
    int Activo;
    char Nombre[30];
    char Contra[30];
}Usuario;

///Crear usuarios
int cargaUnUsuario(Usuario usua[], int val);

///Mostrar usuarios
void mostrarUnUsuario(Usuario usuaAux[], int pos);
void mostrarArregloUsuarios(Usuario usua[], int val);

///Agrega al final del archivo el arreglo de Usuarios
int agregaUsuariosEnArchivo(Usuario usua[],int val, int dim);

///Muestra todos los usuarios del archivo
void mostrarArchivoUsuarios(char arUsuarios[]);

int getUltimoIdUsuario(char archivo[]);

int main()
{
    Usuario efra[50];
    int val = 0;
    int dim = 50;

    val = agregaUsuariosEnArchivo(efra,val,dim);

    ///mostrarArregloUsuarios(efra, val);
    mostrarArchivoUsuarios(archivoUsuarios);


    return 0;
}

int cargaUnUsuario(Usuario usua[], int val)
{
    int codAdmin;
    usua[val].Activo = 1;
    usua[val].Id = getUltimoIdUsuario(archivoUsuarios)+1;

    printf("\n\t\t<<<<<<<<<< CREACION DE USUARIO >>>>>>>>>>");

    printf("\nNombre de Usuario..............:");
    fflush(stdin);
    gets(usua[val].Nombre);

    printf("\nContraseña.....................:");
    fflush(stdin);
    gets(usua[val].Contra);

    printf("\nCodigo de Administrador........:");
    scanf("%d", &codAdmin);
    if(codAdmin == CODADMIN)
        usua[val].Tipo = TIPOADMIN;
    else///TIPO DE USUARIO 1 ADMIN 0 USUARIO REGULAR
        usua[val].Tipo = TIPONORMAL;

    val++;

    return val;
}

void mostrarUnUsuario(Usuario usuaAux[], int pos)
{

    printf("\n ID: %d", usuaAux[pos].Id);
    printf("\n Activo: %d", usuaAux[pos].Activo);
    printf("\n Tipo: %d", usuaAux[pos].Tipo);
    printf("\n Nombre: %s", usuaAux[pos].Nombre);
    printf("\n contraseña: %s", usuaAux[pos].Contra);
    printf("\n____________________");
}

void mostrarArregloUsuarios(Usuario usua[], int val)
{
    for(int i = 0; i < val; i++)
    {
        mostrarUnUsuario(usua,i);
    }
}

int agregaUsuariosEnArchivo(Usuario usua[],int val, int dim)
{

    int opcion = 0;
    Usuario usuario;
    FILE *arUsua;


    while((val < dim) && (opcion != ESC) && (arUsua != NULL))
    {
        arUsua = fopen(archivoUsuarios, "ab");
        if(arUsua!=NULL)
        {
            system("cls");
            val = cargaUnUsuario(usua, val);
            usuario = usua[val-1];
            fwrite(&usuario, sizeof(Usuario), 1, arUsua);
            printf("\n\t\tESC PARA SALIR");
            opcion = getch();
        }
        fclose(arUsua);
    }


    return val;
}

void mostrarArchivoUsuarios(char arUsuarios[])
{
    FILE *archi;
    archi = fopen(arUsuarios, "rb");
    Usuario usuaAux;

    if(archi != NULL)
    {
        printf("\nContenido del archivo:\n\n");
        while(fread(&usuaAux, sizeof(Usuario),1,archi)>0)
        {
            printf("\n ID: %d", usuaAux.Id);
            printf("\n Activo: %d", usuaAux.Activo);
            printf("\n Tipo: %d", usuaAux.Tipo);
            printf("\n Nombre: %s", usuaAux.Nombre);
            printf("\n contraseña: %s", usuaAux.Contra);
            printf("\n____________________");
        }
        fclose(archi);
    }
}

int getUltimoIdUsuario(char archivo[])
{
    int mayor = 0;
    FILE *archi;
    archi = fopen(archivo, "rb");
    Usuario usuaAux;

    if(archi != NULL)
    {
        while(!feof(archi))
        {
            fread(&usuaAux,sizeof(usuaAux),1,archi);
            if(!feof(archi))
            {
                if(usuaAux.Id > mayor)
                    mayor = usuaAux.Id;
            }
        }
        fclose(archi);
    }
    return mayor;
}

