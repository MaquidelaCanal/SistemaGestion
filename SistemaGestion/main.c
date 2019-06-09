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

typedef struct Usuario
{
    int Id;
    int Tipo;
    int Activo;
    char Nombre[30];
    char Contra[30];
}Usuario;

typedef struct Producto
{
    int Rubro;
    int Stock;
    int Activo;
    char Descripcion[50];
}Producto;

typedef struct Factura
{
    int VededorId;
    char Fecha;
    int NumeroFactura;
    Producto Productos[100];
    float PrecioTotal;
}Factura;

///Crear usuarios
int cargaUnUsuario(Usuario usua[], int val);

///Mostrar usuarios
void mostrarUnUsuario(Usuario usuaAux[], int pos);
void mostrarArregloUsuarios(Usuario usua[], int val);

///Agrega al final del archivo el arreglo de Usuarios
int agregaUsuariosEnArchivo(Usuario usua[],int val, int dim);

///Muestra todos los usuarios del archivo
void mostrarArchivoUsuarios(char arUsuarios[]);

///Devuelve el mayor ID del archivo
int getUltimoIdUsuario(char archivo[]);

///Checkea si el usuario existe
int checkExisteUsuarioId(int idBuscado, char arUsuarios[]);
int checkExisteUsuarioNombre(char nombreBuscado[], char arUsuario[]);

///Checkea si el usuario es administrador
int checkTipoUsuarioById(int usuaId, char arUsuarios[]);

///Devuelve el usuario con el id asociado, devuelve usuario.Id = -1 si no se encontro.
Usuario getUsuarioById(int idBuscado, char arUsuarios[]);

int main()
{
    Usuario efra[50];
    Usuario usuaAux;

    int val = 0;
    int dim = 50;
    int testBusqueda;
    int testAdmin;
    char nombreAux[30];
    char nombreTest[30];
    val = agregaUsuariosEnArchivo(efra,val,dim);

    mostrarArchivoUsuarios(archivoUsuarios);

    return 0;
}

int cargaUnUsuario(Usuario usua[], int val)
{
    int codAdmin;
    int valido = 0;
    char nombreAux[30];
    usua[val].Activo = 1;
    usua[val].Id = getUltimoIdUsuario(archivoUsuarios)+1;

    system("cls");

    while(valido == 0)
    {
        printf("\n\t\t<<<<<<<<<< CREACION DE USUARIO >>>>>>>>>>");
        printf("\nNombre de Usuario..............:");
        fflush(stdin);
        scanf("%s", &nombreAux);
        if(checkExisteUsuarioNombre(nombreAux,archivoUsuarios) == 0)
        {
            valido = 1;
            strcpy(usua[val].Nombre, nombreAux);
        }else
        {
            system("cls");
            printf("\a");
            gotoxy(15,5);
            printf("Ese usuario ya se encuentra en uso!");
            getch();
            gotoxy(0,0);
            system("cls");
        }

    }


    printf("\nPassword.....................:");
    fflush(stdin);
    gets(usua[val].Contra);

    printf("\nSi posee codigo de Administrador, Ingreselo..:");
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

    if(usuaAux[pos].Activo == 1)
    {
        printf("\n ID: %d", usuaAux[pos].Id);
        printf("\n Activo: %d", usuaAux[pos].Activo);
        printf("\n Tipo: %d", usuaAux[pos].Tipo);
        printf("\n Nombre: %s", usuaAux[pos].Nombre);
        printf("\n contrase�a: %s", usuaAux[pos].Contra);
        printf("\n____________________");
    }
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
            if(usuaAux.Activo == 1)
            {
                printf("\n ID: %d", usuaAux.Id);
                printf("\n Activo: %d", usuaAux.Activo);
                printf("\n Tipo: %d", usuaAux.Tipo);
                printf("\n Nombre: %s", usuaAux.Nombre);
                printf("\n contrase�a: %s", usuaAux.Contra);
                printf("\n____________________");
            }
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
        while(!feof(archi)) ///Se cambio de fread > 0 a !feof porque no funcionaba
        {
            fread(&usuaAux,sizeof(Usuario),1,archi);
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

int checkExisteUsuarioId(int idBuscado, char arUsuarios[])
{
    Usuario usua;
    int flag = -1; /// -1 error no abrio archivo, 0 no encontro ID, 1 encontro ID
    FILE *pArch;
    pArch = fopen(arUsuarios, "rb");

    if(pArch != NULL)
    {
        while(fread(&usua, sizeof(Usuario), 1, pArch)>0)
        {
            if((usua.Id == idBuscado) && (usua.Activo == 1))
            {
                flag = 1;
            }
        }
        if(flag != 1)
        {
            flag = 0;
        }
        fclose(pArch);
    }

   return flag;
}

int checkExisteUsuarioNombre(char nombreBuscado[], char arUsuarios[])
{
    Usuario usua;
    int flag = -1; /// -1 error no abrio archivo, 0 no encontro ID, 1 encontro ID
    FILE *pArch;
    pArch = fopen(arUsuarios, "rb");

    if(pArch != NULL)
    {
        while(fread(&usua, sizeof(Usuario), 1, pArch)>0)
        {
            if((strcmp(strlwr(usua.Nombre), strlwr(nombreBuscado)) == 0) && (usua.Activo == 1))
            {
                flag = 1;
            }
        }
        if(flag != 1)
        {
            flag = 0;
        }
        fclose(pArch);
    }
   return flag;
}

int checkTipoUsuarioById(int usuaId, char arUsuarios[])
{

    Usuario usua;
    int flag = -1; /// -1 error no abrio archivo, 0 no es admin, 1 es admin
    FILE *pArch;
    pArch = fopen(arUsuarios, "rb");
    if(usuaId <= getUltimoIdUsuario(arUsuarios))
    {
        if(pArch != NULL)
        {
            while(fread(&usua, sizeof(Usuario), 1, pArch)>0)
            {
                if(usua.Id == usuaId)
                {
                    if(usua.Tipo == 1)
                        flag = 1;
                }
            }
            if(flag != 1)
            {
                flag = 0;
            }
            fclose(pArch);
        }
    }else
    {
        system("cls");
        printf("\a");
        gotoxy(15,5);
        printf("No existe ningun usuario con ese id.");
        getch();
        gotoxy(0,0);
        system("cls");

    }
   return flag;
}

Usuario getUsuarioById(int idBuscado, char arUsuarios[])
{
    Usuario usua;
    int flag = 0;
    char stringAux[30] = "Usuario no encontrado";
    FILE *pArch;
    pArch = fopen(arUsuarios, "rb");

    if(idBuscado <= getUltimoIdUsuario(arUsuarios))
    {
        if(pArch != NULL)
        {
            while((flag == 0) && (fread(&usua, sizeof(Usuario), 1, pArch)>0))
            {
                if(usua.Id == idBuscado)
                {
                    flag = 1;
                }
            }
            if((flag == 0) || (usua.Activo == 0))
            {
                usua.Id = -1;
                strcpy(usua.Nombre, stringAux);
            }

            fclose(pArch);
        }
    }else
    {
        system("cls");
        printf("\a");
        gotoxy(15,5);
        printf("No existe ningun usuario con ese id.");
        getch();
        gotoxy(0,0);
        system("cls");

    }

   return usua;
}

int eliminarUsuarioById(int usuaId, char arUsuarios[])
{
    Usuario usua;
    int flag = -1; /// -1 error no abrio archivo, 0 no es admin, 1 es admin
    FILE *pArch;
    pArch = fopen(arUsuarios, "rb");

    if(pArch != NULL)
    {
        while(fread(&usua, sizeof(Usuario), 1, pArch)>0)
        {
            if(usua.Id == usuaId)
            {
                if(usua.Tipo == 1)
                    flag = 1;
            }
        }
        if(flag != 1)
        {
            flag = 0;
        }
        fclose(pArch);
    }
   return flag;
}

