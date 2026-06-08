#include <stdio.h>
#include <string.h>

#define CAPACIDAD 10

struct RegistroLibro {
    int codigo;
    char nombreLibro[100];
    char escritor[50];
    int fechaPublicacion;
    char disponibilidad[15];
};

struct RegistroLibro catalogo[CAPACIDAD];
int totalRegistros = 0;

// Verificar código repetido
int existeCodigo(int codigo) {
    for (int i = 0; i < totalRegistros; i++) {
        if (catalogo[i].codigo == codigo) {
            return 1;
        }
    }
    return 0;
}

// Agregar libro
void agregarLibro() {

    if (totalRegistros == CAPACIDAD) {
        printf("\nNo hay espacio para mas registros.\n");
        return;
    }

    int nuevoCodigo;

    do {
        printf("\nCodigo del libro: ");
        scanf("%d", &nuevoCodigo);

        if (existeCodigo(nuevoCodigo)) {
            printf("El codigo ya se encuentra registrado.\n");
        }

    } while (existeCodigo(nuevoCodigo));

    catalogo[totalRegistros].codigo = nuevoCodigo;

    getchar();

    printf("Nombre del libro: ");
    fgets(catalogo[totalRegistros].nombreLibro, 100, stdin);
    catalogo[totalRegistros].nombreLibro[strcspn(catalogo[totalRegistros].nombreLibro, "\n")] = '\0';

    printf("Autor del libro: ");
    fgets(catalogo[totalRegistros].escritor, 50, stdin);
    catalogo[totalRegistros].escritor[strcspn(catalogo[totalRegistros].escritor, "\n")] = '\0';

    printf("Anio de publicacion: ");
    scanf("%d", &catalogo[totalRegistros].fechaPublicacion);

    strcpy(catalogo[totalRegistros].disponibilidad, "Disponible");

    totalRegistros++;

    printf("\nLibro almacenado exitosamente.\n");
}

// Mostrar catálogo
void visualizarCatalogo() {

    if (totalRegistros == 0) {
        printf("\nNo existen libros registrados.\n");
        return;
    }

    printf("\n=================== CATÁLOGO GENERAL ===================\n");

    for (int i = 0; i < totalRegistros; i++) {

        printf("\nRegistro #%d\n", i + 1);
        printf("Codigo : %d\n", catalogo[i].codigo);
        printf("Titulo : %s\n", catalogo[i].nombreLibro);
        printf("Autor  : %s\n", catalogo[i].escritor);
        printf("Anio    : %d\n", catalogo[i].fechaPublicacion);
        printf("Estado : %s\n", catalogo[i].disponibilidad);

        printf("--------------------------------------\n");
    }
}

// Buscar libro
void consultarLibro() {

    int codigoBuscado;
    int encontrado = 0;

    printf("\nIngrese el codigo del libro: ");
    scanf("%d", &codigoBuscado);

    for (int i = 0; i < totalRegistros; i++) {

        if (catalogo[i].codigo == codigoBuscado) {

            printf("\n===== INFORMACION DEL LIBRO =====\n");
            printf("Titulo : %s\n", catalogo[i].nombreLibro);
            printf("Autor  : %s\n", catalogo[i].escritor);
            printf("Anio    : %d\n", catalogo[i].fechaPublicacion);
            printf("Estado : %s\n", catalogo[i].disponibilidad);

            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("No se encontro el libro solicitado.\n");
    }
}

// Cambiar estado
void cambiarEstado() {

    int codigo;

    printf("\nCodigo del libro: ");
    scanf("%d", &codigo);

    for (int i = 0; i < totalRegistros; i++) {

        if (catalogo[i].codigo == codigo) {

            if (strcmp(catalogo[i].disponibilidad, "Disponible") == 0) {
                strcpy(catalogo[i].disponibilidad, "Prestado");
            } else {
                strcpy(catalogo[i].disponibilidad, "Disponible");
            }

            printf("Estado actualizado correctamente.\n");
            return;
        }
    }

    printf("Codigo no encontrado.\n");
}

// Eliminar registro
void retirarLibro() {

    int codigo;
    int existe = 0;

    printf("\nIngrese el codigo a eliminar: ");
    scanf("%d", &codigo);

    for (int i = 0; i < totalRegistros; i++) {

        if (catalogo[i].codigo == codigo) {

            for (int j = i; j < totalRegistros - 1; j++) {
                catalogo[j] = catalogo[j + 1];
            }

            totalRegistros--;
            existe = 1;

            printf("Libro eliminado del catalogo.\n");
            break;
        }
    }

    if (!existe) {
        printf("No existe un libro con ese codigo.\n");
    }
}

int main() {

    int opcionMenu;

    do {

        printf("\n=====================================\n");
        printf("   SISTEMA DE GESTIÓN BIBLIOTECARIA\n");
        printf("=====================================\n");
        printf("1. Incorporar libro\n");
        printf("2. Ver catalogo\n");
        printf("3. Consultar libro\n");
        printf("4. Modificar disponibilidad\n");
        printf("5. Eliminar registro\n");
        printf("6. Finalizar programa\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcionMenu);

        switch(opcionMenu) {

            case 1:
                agregarLibro();
                break;

            case 2:
                visualizarCatalogo();
                break;

            case 3:
                consultarLibro();
                break;

            case 4:
                cambiarEstado();
                break;

            case 5:
                retirarLibro();
                break;

            case 6:
                printf("\nGracias por utilizar el sistema.\n");
                break;

            default:
                printf("\nOpcion no válida.\n");
        }

    } while(opcionMenu != 6);

    return 0;
}