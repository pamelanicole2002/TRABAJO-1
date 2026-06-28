#include <stdio.h>
#include <string.h>

#define NUM_ZONAS 5
#define DIAS_HISTORICO 30
#define NUM_CONTAMINANTES 4

// Límites OMS (µg/m³)
#define LIMITE_CO2  400.0
#define LIMITE_SO2  40.0
#define LIMITE_NO2  25.0
#define LIMITE_PM25 15.0

typedef struct {
    char nombre[50];
    double actual[NUM_CONTAMINANTES];       // CO2, SO2, NO2, PM2.5
    double historico[DIAS_HISTORICO][NUM_CONTAMINANTES];
    double prediccion[NUM_CONTAMINANTES];
    double temperatura;
    double viento;
    double humedad;
} Zona;

const char* nombres_contaminantes[NUM_CONTAMINANTES] = {"CO2", "SO2", "NO2", "PM2.5"};
const double limites[NUM_CONTAMINANTES] = {LIMITE_CO2, LIMITE_SO2, LIMITE_NO2, LIMITE_PM25};

// ── Funciones ──────────────────────────────────────────────

void inicializar_zonas(Zona zonas[]) {
    char nombres[NUM_ZONAS][50] = {
        "Zona Norte", "Zona Sur", "Zona Este", "Zona Oeste", "Zona Centro"
    };
    // Datos simulados de ejemplo
    double datos_actuales[NUM_ZONAS][NUM_CONTAMINANTES] = {
        {380, 35, 20, 12},
        {420, 50, 30, 18},
        {360, 28, 18, 10},
        {450, 60, 40, 25},
        {390, 42, 26, 16}
    };
    double clima[NUM_ZONAS][3] = {  // temp, viento, humedad
        {22, 10, 60}, {25, 5, 75}, {20, 15, 55}, {28, 3, 80}, {23, 8, 65}
    };

    for (int i = 0; i < NUM_ZONAS; i++) {
        strcpy(zonas[i].nombre, nombres[i]);
        for (int j = 0; j < NUM_CONTAMINANTES; j++) {
            zonas[i].actual[j] = datos_actuales[i][j];
            // Histórico simulado con variación
            for (int d = 0; d < DIAS_HISTORICO; d++) {
                zonas[i].historico[d][j] = datos_actuales[i][j] * (0.85 + (d % 5) * 0.05);
            }
            zonas[i].prediccion[j] = 0;
        }
        zonas[i].temperatura = clima[i][0];
        zonas[i].viento      = clima[i][1];
        zonas[i].humedad     = clima[i][2];
    }
}

void calcular_prediccion(Zona *z) {
    // Promedio ponderado: días más recientes tienen mayor peso
    for (int j = 0; j < NUM_CONTAMINANTES; j++) {
        double suma_pesos = 0, suma_valores = 0;
        for (int d = 0; d < DIAS_HISTORICO; d++) {
            double peso = (double)(d + 1);  // día más reciente = mayor peso
            suma_valores += z->historico[d][j] * peso;
            suma_pesos   += peso;
        }
        double pred = suma_valores / suma_pesos;
        // Ajuste por viento (mayor viento reduce contaminación)
        pred *= (1.0 - z->viento * 0.005);
        // Ajuste por humedad (mayor humedad puede aumentar PM2.5)
        if (j == 3) pred *= (1.0 + z->humedad * 0.001);
        z->prediccion[j] = pred;
    }
}

void mostrar_estado_zona(Zona *z) {
    printf("\n══════════════════════════════════════\n");
    printf("  %s\n", z->nombre);
    printf("  Temp: %.1f°C | Viento: %.1f km/h | Humedad: %.1f%%\n",
           z->temperatura, z->viento, z->humedad);
    printf("──────────────────────────────────────\n");
    printf("  %-8s %-12s %-12s %-10s\n", "Gas", "Actual", "Predicción", "Límite OMS");
    for (int j = 0; j < NUM_CONTAMINANTES; j++) {
        char alerta[5] = "";
        if (z->actual[j] > limites[j] || z->prediccion[j] > limites[j])
            strcpy(alerta, " (!)");
        printf("  %-8s %-12.2f %-12.2f %-10.2f%s\n",
               nombres_contaminantes[j],
               z->actual[j], z->prediccion[j], limites[j], alerta);
    }
}

void emitir_alertas(Zona *z) {
    int hay_alerta = 0;
    for (int j = 0; j < NUM_CONTAMINANTES; j++) {
        if (z->prediccion[j] > limites[j]) {
            if (!hay_alerta) {
                printf("\n  *** ALERTA en %s ***\n", z->nombre);
                hay_alerta = 1;
            }
            printf("  - %s previsto en %.2f µg/m³ (límite: %.2f)\n",
                   nombres_contaminantes[j], z->prediccion[j], limites[j]);
        }
    }
}

void generar_recomendaciones(Zona *z) {
    int necesita = 0;
    for (int j = 0; j < NUM_CONTAMINANTES; j++) {
        if (z->actual[j] > limites[j] || z->prediccion[j] > limites[j]) {
            necesita = 1; break;
        }
    }
    if (!necesita) return;

    printf("\n  Recomendaciones para %s:\n", z->nombre);
    if (z->actual[2] > LIMITE_NO2 || z->prediccion[2] > LIMITE_NO2)
        printf("  - Reducir circulacion vehicular en horario pico.\n");
    if (z->actual[1] > LIMITE_SO2 || z->prediccion[1] > LIMITE_SO2)
        printf("  - Cierre temporal de industrias con alta emision de SO2.\n");
    if (z->actual[3] > LIMITE_PM25 || z->prediccion[3] > LIMITE_PM25)
        printf("  - Suspender actividades al aire libre. Uso de mascarillas.\n");
    if (z->actual[0] > LIMITE_CO2)
        printf("  - Fomentar uso de transporte publico y areas verdes.\n");
}

void calcular_promedio_historico(Zona *z) {
    printf("\n  Promedios historicos (30 dias) - %s:\n", z->nombre);
    for (int j = 0; j < NUM_CONTAMINANTES; j++) {
        double suma = 0;
        for (int d = 0; d < DIAS_HISTORICO; d++) suma += z->historico[d][j];
        double prom = suma / DIAS_HISTORICO;
        printf("  %s: %.2f µg/m³ %s\n", nombres_contaminantes[j], prom,
               prom > limites[j] ? "[SUPERA LIMITE OMS]" : "[OK]");
    }
}

void exportar_reporte(Zona zonas[]) {
    FILE *f = fopen("reporte_contaminacion.txt", "w");
    if (!f) { printf("Error al crear el archivo de reporte.\n"); return; }

    fprintf(f, "REPORTE DE CONTAMINACION DEL AIRE\n");
    fprintf(f, "==================================\n\n");

    for (int i = 0; i < NUM_ZONAS; i++) {
        Zona *z = &zonas[i];
        fprintf(f, "Zona: %s\n", z->nombre);
        fprintf(f, "Temperatura: %.1f C | Viento: %.1f km/h | Humedad: %.1f%%\n",
                z->temperatura, z->viento, z->humedad);
        fprintf(f, "%-8s %-12s %-12s %-10s\n", "Gas", "Actual", "Prediccion", "Limite");
        for (int j = 0; j < NUM_CONTAMINANTES; j++) {
            fprintf(f, "%-8s %-12.2f %-12.2f %-10.2f\n",
                    nombres_contaminantes[j],
                    z->actual[j], z->prediccion[j], limites[j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    printf("\nReporte exportado a 'reporte_contaminacion.txt'\n");
}

// ── Menú principal ─────────────────────────────────────────

int main() {
    Zona zonas[NUM_ZONAS];
    inicializar_zonas(zonas);

    // Calcular predicciones al inicio
    for (int i = 0; i < NUM_ZONAS; i++)
        calcular_prediccion(&zonas[i]);

    int opcion;
    do {
        printf("\n\n╔══════════════════════════════════════╗\n");
        printf("║     SIGPCA - Menu Principal          ║\n");
        printf("╠══════════════════════════════════════╣\n");
        printf("║ 1. Ver estado actual de todas las    ║\n");
        printf("║    zonas                             ║\n");
        printf("║ 2. Ver alertas preventivas           ║\n");
        printf("║ 3. Ver promedios historicos          ║\n");
        printf("║ 4. Ver recomendaciones               ║\n");
        printf("║ 5. Exportar reporte                  ║\n");
        printf("║ 0. Salir                             ║\n");
        printf("╚══════════════════════════════════════╝\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                for (int i = 0; i < NUM_ZONAS; i++)
                    mostrar_estado_zona(&zonas[i]);
                break;
            case 2:
                printf("\n-- ALERTAS PREVENTIVAS (proximas 24h) --");
                for (int i = 0; i < NUM_ZONAS; i++)
                    emitir_alertas(&zonas[i]);
                break;
            case 3:
                for (int i = 0; i < NUM_ZONAS; i++)
                    calcular_promedio_historico(&zonas[i]);
                break;
            case 4:
                printf("\n-- RECOMENDACIONES DE MITIGACION --");
                for (int i = 0; i < NUM_ZONAS; i++)
                    generar_recomendaciones(&zonas[i]);
                break;
            case 5:
                exportar_reporte(zonas);
                break;
            case 0:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 0);

    return 0;
}
