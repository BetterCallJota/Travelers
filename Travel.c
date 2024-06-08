#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para representar un viaje
typedef struct Viaje {
    char matricula[20];
    char nombre[50];
    char fecha[20];
    float precio;
    char destino[50];
    int capacidad;
    int identificador;
} Viaje;

typedef struct AVLNode {
    Viaje viaje;
    struct AVLNode *izquierda;
    struct AVLNode *derecha;
    int altura;
} AVLNode;

AVLNode *crear_nodo(Viaje viaje);
int altura(AVLNode *nodo);
int max(int a, int b);
AVLNode *rotacion_derecha(AVLNode *y);
AVLNode *rotacion_izquierda(AVLNode *x);
int obtener_balance(AVLNode *nodo);
AVLNode *insertar_viaje(AVLNode *raiz, Viaje viaje);
AVLNode *buscar_viaje(AVLNode *raiz, int identificador);
void imprimir_viaje(Viaje viaje);
void listar_viajes(AVLNode *raiz);
void eliminar_viaje(AVLNode **raiz, int identificador);
void registrar_pasajero(AVLNode *raiz, int identificador, char *nombre);
void listar_pasajeros(AVLNode *raiz, int identificador);
void guardar_registros_en_archivo(AVLNode *raiz, const char *nombre_archivo);
void guardar_viajes_en_archivo(AVLNode *nodo, FILE *archivo);
void registrar_viaje(AVLNode **raiz);
void buscar_y_mostrar_viaje(AVLNode *raiz);
void eliminar_viaje_por_identificador(AVLNode **raiz);
void registrar_pasajero_en_viaje(AVLNode *raiz);
void listar_pasajeros_de_viaje(AVLNode *raiz);

int main() {
    AVLNode *arbol = NULL;
    int opcion;

    do {
        printf("\nMENU:\n");
        printf("1. Registrar Viaje\n");
        printf("2. Buscar Viaje\n");
        printf("3. Listar Viajes\n");
        printf("4. Eliminar Viaje\n");
        printf("5. Registrar Pasajero\n");
        printf("6. Listar Pasajeros\n");
        printf("7. Guardar Registros en Archivo\n");
        printf("8. Retroceder\n");
        printf("9. Salir\n");
        printf("Ingrese su opcion: ");
        scanf("%d%*c", &opcion);

        switch(opcion) {
            case 1:
                registrar_viaje(&arbol);
                break;
            case 2:
                buscar_y_mostrar_viaje(arbol);
                break;
            case 3:
                printf("\nListado de viajes:\n");
                listar_viajes(arbol);
                break;
            case 4:
                eliminar_viaje_por_identificador(&arbol);
                break;
            case 5:
                registrar_pasajero_en_viaje(arbol);
                break;
            case 6:
                listar_pasajeros_de_viaje(arbol);
                break;
            case 7:
                guardar_registros_en_archivo(arbol, "registros_viajes.txt");
                printf("Registros guardados correctamente en el archivo 'registros_viajes.txt'.\n");
                break;
            case 8:
                
                break;
            case 9:
                printf("\nSaliendo del programa...\n");
                break;
            default:
                printf("\nOpcion no valida. Por favor, ingrese una opcion valida.\n");
        }
    } while (opcion != 9);

    return 0;
}

AVLNode *crear_nodo(Viaje viaje) {
    AVLNode *nodo = (AVLNode *)malloc(sizeof(AVLNode));
    if (nodo == NULL) {
        printf("Error: No se pudo asignar memoria para el nodo AVL.\n");
        exit(EXIT_FAILURE);
    }
    nodo->viaje = viaje;
    nodo->izquierda = NULL;
    nodo->derecha = NULL;
    nodo->altura = 1;
    return nodo;
}

int altura(AVLNode *nodo) {
    return (nodo == NULL) ? 0 : nodo->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode *rotacion_derecha(AVLNode *y) {
    AVLNode *x = y->izquierda;
    AVLNode *T2 = x->derecha;

    x->derecha = y;
    y->izquierda = T2;

    y->altura = max(altura(y->izquierda), altura(y->derecha)) + 1;
    x->altura = max(altura(x->izquierda), altura(x->derecha)) + 1;

    return x;
}

AVLNode *rotacion_izquierda(AVLNode *x) {
    AVLNode *y = x->derecha;
    AVLNode *T2 = y->izquierda;

    y->izquierda = x;
    x->derecha = T2;

    x->altura = max(altura(x->izquierda), altura(x->derecha)) + 1;
    y->altura = max(altura(y->izquierda), altura(y->derecha)) + 1;

    return y;
}

int obtener_balance(AVLNode *nodo) {
    if (nodo == NULL) return 0;
    return altura(nodo->izquierda) - altura(nodo->derecha);
}

AVLNode *insertar_viaje(AVLNode *raiz, Viaje viaje) {
    if (raiz == NULL) return crear_nodo(viaje);

    if (viaje.identificador < raiz->viaje.identificador)
        raiz->izquierda = insertar_viaje(raiz->izquierda, viaje);
    else if (viaje.identificador > raiz->viaje.identificador)
        raiz->derecha = insertar_viaje(raiz->derecha, viaje);
    else 
        return raiz;

    raiz->altura = 1 + max(altura(raiz->izquierda), altura(raiz->derecha));

    int balance = obtener_balance(raiz);

    // Caso izquierda-izquierda
    if (balance > 1 && viaje.identificador < raiz->izquierda->viaje.identificador)
        return rotacion_derecha(raiz);

    // Caso derecha-derecha
    if (balance < -1 && viaje.identificador > raiz->derecha->viaje.identificador)
        return rotacion_izquierda(raiz);

    // Caso izquierda-derecha
    if (balance > 1 && viaje.identificador > raiz->izquierda->viaje.identificador) {
        raiz->izquierda = rotacion_izquierda(raiz->izquierda);
        return rotacion_derecha(raiz);
    }

    // Caso derecha-izquierda
    if (balance < -1 && viaje.identificador < raiz->derecha->viaje.identificador) {
        raiz->derecha = rotacion_derecha(raiz->derecha);
        return rotacion_izquierda(raiz);
    }

    return raiz;
}

AVLNode *buscar_viaje(AVLNode *raiz, int identificador) {
    if (raiz == NULL || raiz->viaje.identificador == identificador)
        return raiz;

    if (raiz->viaje.identificador < identificador)
        return buscar_viaje(raiz->derecha, identificador);

    return buscar_viaje(raiz->izquierda, identificador);
}

void imprimir_viaje(Viaje viaje) {
    printf("Identificador: %d\n", viaje.identificador);
    printf("Matricula: %s\n", viaje.matricula);
    printf("Nombre: %s\n", viaje.nombre);
    printf("Fecha: %s\n", viaje.fecha);
    printf("Precio: %.2f\n", viaje.precio);
    printf("Destino: %s\n", viaje.destino);
    printf("Capacidad: %d\n", viaje.capacidad);
}

void listar_viajes(AVLNode *raiz) {
    if (raiz != NULL) {
        listar_viajes(raiz->izquierda);
        imprimir_viaje(raiz->viaje);
        listar_viajes(raiz->derecha);
    }
}

void eliminar_viaje(AVLNode **raiz, int identificador) {
    if (*raiz == NULL) return;

    if (identificador < (*raiz)->viaje.identificador)
        eliminar_viaje(&((*raiz)->izquierda), identificador);
    else if (identificador > (*raiz)->viaje.identificador)
        eliminar_viaje(&((*raiz)->derecha), identificador);
    else {
        if ((*raiz)->izquierda == NULL || (*raiz)->derecha == NULL) {
            AVLNode *temp = (*raiz)->izquierda ? (*raiz)->izquierda : (*raiz)->derecha;

            if (temp == NULL) {
                temp = *raiz;
                *raiz = NULL;
            } else {
                **raiz = *temp;
            }
            free(temp);
        } else {
            AVLNode *temp = (*raiz)->derecha;
            while (temp->izquierda != NULL)
                temp = temp->izquierda;

            (*raiz)->viaje = temp->viaje;
            eliminar_viaje(&((*raiz)->derecha), temp->viaje.identificador);
        }
    }

    if (*raiz == NULL) return;

    (*raiz)->altura = 1 + max(altura((*raiz)->izquierda), altura((*raiz)->derecha));

    int balance = obtener_balance(*raiz);

    // Caso izquierda-izquierda
    if (balance > 1 && obtener_balance((*raiz)->izquierda) >= 0)
        *raiz = rotacion_derecha(*raiz);

    // Caso derecha-derecha
    if (balance < -1 && obtener_balance((*raiz)->derecha) <= 0)
        *raiz = rotacion_izquierda(*raiz);

    // Caso izquierda-derecha
    if (balance > 1 && obtener_balance((*raiz)->izquierda) < 0) {
        (*raiz)->izquierda = rotacion_izquierda((*raiz)->izquierda);
        *raiz = rotacion_derecha(*raiz);
    }

    // Caso derecha-izquierda
    if (balance < -1 && obtener_balance((*raiz)->derecha) > 0) {
        (*raiz)->derecha = rotacion_derecha((*raiz)->derecha);
        *raiz = rotacion_izquierda(*raiz);
    }
}

void registrar_pasajero(AVLNode *raiz, int identificador, char *nombre) {
    AVLNode *viaje = buscar_viaje(raiz, identificador);
    if (viaje != NULL) {
        printf("Pasajero %s registrado en el viaje con identificador %d.\n", nombre, identificador);
        
    } else {
        printf("El viaje con identificador %d no existe.\n", identificador);
    }
}

void listar_pasajeros(AVLNode *raiz, int identificador) {
    AVLNode *viaje = buscar_viaje(raiz, identificador);
    if (viaje != NULL) {
        printf("Pasajeros del viaje con identificador %d:\n", identificador);
        
    } else {
        printf("El viaje con identificador %d no existe.\n", identificador);
    }
}

void guardar_registros_en_archivo(AVLNode *raiz, const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo para escribir.\n");
        return;
    }

    guardar_viajes_en_archivo(raiz, archivo);

    fclose(archivo);
}

void guardar_viajes_en_archivo(AVLNode *nodo, FILE *archivo) {
    if (nodo != NULL) {
        guardar_viajes_en_archivo(nodo->izquierda, archivo);
        fprintf(archivo, "Identificador: %d\n", nodo->viaje.identificador);
        fprintf(archivo, "Matricula: %s\n", nodo->viaje.matricula);
        fprintf(archivo, "Nombre: %s\n", nodo->viaje.nombre);
        fprintf(archivo, "Fecha: %s\n", nodo->viaje.fecha);
        fprintf(archivo, "Precio: %.2f\n", nodo->viaje.precio);
        fprintf(archivo, "Destino: %s\n", nodo->viaje.destino);
        fprintf(archivo, "Capacidad: %d\n", nodo->viaje.capacidad);
        fprintf(archivo, "\n");
        guardar_viajes_en_archivo(nodo->derecha, archivo);
    }
}

void registrar_viaje(AVLNode **raiz) {
    Viaje nuevo_viaje;
    printf("Ingrese la matricula del vehiculo: ");
    fgets(nuevo_viaje.matricula, sizeof(nuevo_viaje.matricula), stdin);
    printf("Ingrese el nombre del viaje: ");
    fgets(nuevo_viaje.nombre, sizeof(nuevo_viaje.nombre), stdin);
    printf("Ingrese la fecha del viaje: ");
    fgets(nuevo_viaje.fecha, sizeof(nuevo_viaje.fecha), stdin);
    printf("Ingrese el precio del viaje: ");
    scanf("%f", &nuevo_viaje.precio);
    
    while (getchar() != '\n');

    printf("Ingrese el destino del viaje: ");
    fgets(nuevo_viaje.destino, sizeof(nuevo_viaje.destino), stdin);
    printf("Ingrese la capacidad del vehiculo: ");
    scanf("%d", &nuevo_viaje.capacidad);
    
    while (getchar() != '\n');

    printf("Ingrese el identificador del viaje: ");
    scanf("%d", &nuevo_viaje.identificador);

    *raiz = insertar_viaje(*raiz, nuevo_viaje);
}

void buscar_y_mostrar_viaje(AVLNode *raiz) {
    int identificador;
    printf("Ingrese el identificador del viaje a buscar: ");
    scanf("%d", &identificador);

    AVLNode *viaje_encontrado = buscar_viaje(raiz, identificador);
    if (viaje_encontrado != NULL) {
        printf("Detalles del viaje encontrado:\n");
        imprimir_viaje(viaje_encontrado->viaje);
    } else {
        printf("No se encontro ningun viaje con ese identificador.\n");
    }
}

void eliminar_viaje_por_identificador(AVLNode **raiz) {
    int identificador;
    printf("Ingrese el identificador del viaje a eliminar: ");
    scanf("%d", &identificador);

    eliminar_viaje(raiz, identificador);
    printf("Viaje eliminado correctamente.\n");
}

void registrar_pasajero_en_viaje(AVLNode *raiz) {
    int identificador;
    printf("Ingrese el identificador del viaje: ");
    scanf("%d", &identificador);
    
    while (getchar() != '\n');

    char nombre_pasajero[50];
    printf("Ingrese el nombre del pasajero: ");
    fgets(nombre_pasajero, sizeof(nombre_pasajero), stdin);

    registrar_pasajero(raiz, identificador, nombre_pasajero);
    printf("Pasajero registrado en el viaje correctamente.\n");
}

void listar_pasajeros_de_viaje(AVLNode *raiz) {
    int identificador;
    printf("Ingrese el identificador del viaje: ");
    scanf("%d", &identificador);

    listar_pasajeros(raiz, identificador);
}
