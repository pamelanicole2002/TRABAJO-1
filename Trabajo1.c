#include <stdio.h>
#include <string.h>


int main() {
    int id, stock, opcion, cantidad;
    float precio, ganancias = 0, totalVenta;
    char nombre[50];

    // Registro del producto
    printf("Registro del producto\n");
    printf("Ingrese ID del producto: ");
    scanf("%d", &id);

    printf("Ingrese nombre del producto: ");
    scanf("%s", nombre);

    printf("Ingrese cantidad en stock: ");
    scanf("%d", &stock);

    printf("Ingrese precio unitario: ");
    scanf("%f", &precio);

    do {
        printf("\n Menu \n");
        printf("1. Vender producto\n");
        printf("2. Reabastecer stock\n");
        printf("3. Consultar producto\n");
        printf("4. Ver ganancias\n");
        printf("5. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                printf("Ingrese cantidad a vender: ");
                scanf("%d", &cantidad);

                if (cantidad <= 0) {
                    printf("Cantidad invalida.\n");
                } else if (cantidad > stock) {
                    printf("No hay suficiente stock.\n");
                } else {
                    totalVenta = cantidad * precio;
                    stock -= cantidad;
                    ganancias += totalVenta;

                    printf("Venta realizada con exito.\n");
                    printf("Total: %.2f\n", totalVenta);
                }
                break;

            case 2:
                        printf("Ingrese cantidad a agregar: ");
                scanf("%d", &cantidad);

                if (cantidad > 0) {
                    stock += cantidad;
                    printf("Stock actualizado correctamente.\n");
                } else {
                    printf("Cantidad invalida.\n");
                }
                break;

            case 3:
                printf("\nInformacion del producto\n");
                printf("ID: %d\n", id);
                printf("Nombre: %s\n", nombre);
                printf("Stock: %d\n", stock);
                printf("Precio: %.2f\n", precio);
                break;

            case 4:
                printf("Ganancias acumuladas: %.2f\n", ganancias);
                break;

            case 5:
                printf("Saliendo del programa...\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }

    } while(opcion != 5);

    return 0;
}