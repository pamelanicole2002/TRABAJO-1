#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ZONAS 5

typedef struct {
    char nombre[30];
    float pm25;
    float co2;
    float temperatura;
    float humedad;
    float viento;
    float prediccion;
    char alerta[30];
    char medida[100];
} Zona;

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}   

int validarNombre(char nombre[]) {
    int i;

    if (strlen(nombre) == 0) {
        return 0;
    } 

    for (i = 0; nombre[i] != '\0'; i++) {
        if (!isalpha(nombre[i]) && nombre[i] != ' ' ) {
            return 0;
        } 
    } 

    return 1;
}   

float leerNumero(char mensaje[], float minimo, float maximo) {
    float valor;
    char entrada[50];
    char extra;

    while (1) {
        printf("%s", mensaje);
        fgets(entrada, 50, stdin);

        if (sscanf(entrada, "%f %c", &valor, &extra) !=1) {
            printf("Error: ingrese solo numeros.\n");
        } else if (valor < minimo || valor > maximo) {
            printf("Error: ingrese un valor real entre %.2f y %.2f.\n", minimo, maximo);
        } else {
            return valor;
        }
    }
}

void ingresarDatos(Zona zonas[]) { 
    int i;

    for (i = 0; i < ZONAS; i++) { 
        printf("\n--- Zona %d ---\n", i + 1);

        do {
            printf("Nombre de la zona: ");
            fgets(zonas[i].nombre, 30, stdin);
            zonas[i].nombre[strcspn(zonas[i].nombre, "\n")] = '\0';

            if (!validarNombre(zonas[i].nombre)) { 
                printf("Error: el nombre solo debe contener letras y espacios.\n");
            }

        } while (!validarNombre(zonas[i].nombre));

        zonas[i].pm25 =leerNumero("PM2.5: ", 0, 500);
        zonas[i].co2 = leerNumero("CO2: ", 300, 5000);
        zonas[i].temperatura = leerNumero("Temperatura: ", -10, 50);
        zonas[i].humedad = leerNumero("Humedad: ", 0, 100);
        zonas[i].viento = leerNumero("Velocidad del viento: ", 0, 150);
    }
}

void calcularPrediccion(Zona zonas[]) { 
    int i;

    for (i = 0; i < ZONAS; i++) { 
        zonas[i].prediccion =
            (zonas[i].pm25 * 0.45) +
            (zonas[i].co2 * 0.25) +
            (zonas[i].humedad * 0.10) +
            (zonas[i].temperatura * 0.10) -
            (zonas[i].viento * 0.10);
     }
 }

 void generarAlertas(Zona zonas[]) {
    int i;

    for (i = 0; i < ZONAS; i++) {
        if (zonas[i].prediccion < 50) {
            strcpy(zonas[i].alerta, "BAJA");
            strcpy(zonas[i].medida, "Calidad del aire aceptable. ");
        } else if (zonas[i].prediccion < 100) {
            strcpy(zonas[i].alerta, "MEDIA");
            strcpy(zonas[i].medida, "Evitar actividad fisica intensa al aire libre.");
        } else {
            strcpy(zonas[i].alerta, "ALTA");
            strcpy(zonas[i].medida, "Reducir trafico y evitar exposicion prolongada.");
        }
    }  
}

void mostrarResultados(Zona zonas[]) {
    int i;

    printf("\n========== REPORTE AIRECLARO ==========\n");

    for (i = 0; i < ZONAS; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        printf("PM2.5: %.2f\n", zonas[i].pm25);
        printf("CO2: %.2f\n", zonas[i].co2);
        printf("Temperatura: %.2f\n", zonas[i].temperatura);
        printf("Humedad: %.2f\n", zonas[i].humedad);
        printf("Viento: %.2f\n", zonas[i].viento);
        printf("Prediccion 24h: %.2f\n", zonas[i].prediccion);
        printf("Alerta: %s\n", zonas[i].alerta);
        printf("Medida: %s\n", zonas[i].medida);
    }
}

void guardarArchivo(Zona zonas[]) {
    FILE *archivo;
    int i;

    archivo = fopen("reporte_aireclaro.txt", "w");

    if (archivo == NULL) {
        printf("Error al crear el archivo.\n");
        return;
    }
    
    fprintf(archivo, "========== REPORTE AIRECLARO ==========\n");

    for (i = 0; i < ZONAS; i++) {
        fprintf(archivo, "\nZona: %s\n", zonas[i].nombre);
        fprintf(archivo, "PM2.5:%.2f\n", zonas[i].pm25);
        fprintf(archivo, "CO2: %.2f\n", zonas[i].co2);
        fprintf(archivo, "Temperatura: %.2f\n", zonas[i].temperatura);
        fprintf(archivo, "Humedad: %.2f\n", zonas[i].humedad);
        fprintf(archivo, "Viento: %.2f\n", zonas[i].viento);
        fprintf(archivo, "Prediccion 24h: %.2f\n", zonas[i].prediccion);
        fprintf(archivo, "Alerta: %s\n", zonas[i].alerta);
        fprintf(archivo, "Medida: %s\n", zonas[i].medida);
    }

    fclose(archivo);
}

int main() {
    Zona zonas[ZONAS];

    printf("===== SISTEMA AIRECLARO =====\n");

    ingresarDatos(zonas);
    calcularPrediccion(zonas);
    generarAlertas(zonas);
    mostrarResultados(zonas);
    guardarArchivo(zonas);

    printf("\nReporte guardado correctamente en reporte_aireclaro.txt\n");

    return 0;
}