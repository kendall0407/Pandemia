#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char *pais;
    int *aspectos;          // valores de los aspectos
    int numAspectos;        // cantidad de aspectos

    struct Node **vecinos;  // arreglo de punteros a países vecinos
    int numVecinos;

    struct Node *sigt;
    struct Node *ant;
};

struct Paises {
    struct Node *start;
};

// Crear lista
struct Paises *crearPaises() {
    struct Paises *newList = calloc(1, sizeof(struct Paises));
    if (newList == NULL) {
        printf("No se pudieron crear los paises\n");
    }
    return newList;
}

// Crear un país
struct Node* createNewNode(char* pais, int numAspectos) {
    struct Node *newNode = calloc(1, sizeof(struct Node));
    if (newNode == NULL) {
        printf("Error creando un nodo\n");
        return NULL;
    }
    newNode->pais = pais;
    newNode->aspectos = calloc(numAspectos, sizeof(int));
    newNode->numAspectos = numAspectos;

    newNode->vecinos = NULL;
    newNode->numVecinos = 0;

    newNode->ant = NULL;
    newNode->sigt = NULL;
    return newNode;
}

// Agregar vecino a un país
void agregarVecino(struct Node *pais, struct Node *nuevoVecino) {
    pais->numVecinos++;
    pais->vecinos = realloc(pais->vecinos, pais->numVecinos * sizeof(struct Node *));
    pais->vecinos[pais->numVecinos - 1] = nuevoVecino;
}

// Conectar dos países (bidireccionalmente)
void conectarPaises(struct Node *a, struct Node *b) {
    agregarVecino(a, b);
    agregarVecino(b, a);
}

// Insertar país al final de la lista
int insertar_final(struct Paises *lista, char *elemento, int numAspectos) {
    if (lista == NULL) {
        printf("No es lista valida\n");
        return -1;
    }

    struct Node* newNode = createNewNode(elemento, numAspectos);
    if (newNode == NULL) {
        printf("Error creando un nuevo nodo\n");
        return -1;
    } else if (lista->start == NULL) {
        lista->start = newNode;
    } else {
        struct Node* currentNode = lista->start;
        while (currentNode->sigt != NULL) {
            currentNode = currentNode->sigt;
        }
        currentNode->sigt = newNode;
        newNode->ant = currentNode;
    }
    return 0;
}

// Buscar país por nombre
struct Node* buscarPais(struct Paises *lista, char *nombre) {
    struct Node *current = lista->start;
    while (current != NULL) {
        if (strcmp(current->pais, nombre) == 0)
            return current;
        current = current->sigt;
    }
    return NULL;
}

// Imprimir lista de países
void imprimir_lista(struct Paises *lista) {
    if (lista == NULL || lista->start == NULL) {
        printf("⚠️  No hay países en la lista.\n");
        return;
    }

    struct Node *actual = lista->start;
    printf("\n🌎 ====== ESTADO ACTUAL DEL MAPA DE AMÉRICA LATINA ======\n\n");

    while (actual != NULL) {
        printf("🗺️  País: %s\n", actual->pais);

        // Mostrar país anterior y siguiente
        if (actual->ant)
            printf("   ↩️  Anterior: %s\n", actual->ant->pais);
        else
            printf("   ↩️  Anterior: (Ninguno)\n");

        if (actual->sigt)
            printf("   ↪️  Siguiente: %s\n", actual->sigt->pais);
        else
            printf("   ↪️  Siguiente: (Ninguno)\n");

        // Mostrar aspectos
        printf("   🌡️  Aspectos (%d): ", actual->numAspectos);
        for (int i = 0; i < actual->numAspectos; i++) {
            printf("[%d]", actual->aspectos[i]);
            if (i < actual->numAspectos - 1) printf(" ");
        }
        printf("\n");

        // Mostrar vecinos
        printf("   🌐  Vecinos (%d): ", actual->numVecinos);
        if (actual->numVecinos == 0) {
            printf("(Sin vecinos registrados)");
        } else {
            for (int i = 0; i < actual->numVecinos; i++) {
                printf("%s", actual->vecinos[i]->pais);
                if (i < actual->numVecinos - 1) printf(", ");
            }
        }
        printf("\n");

        printf("   ------------------------------------------------------\n");
        actual = actual->sigt;
    }

    printf("\n✅ Fin de la lista de países.\n\n");
}

// Imprimir vecinos de un país
void imprimir_vecinos(struct Node *pais) {
    printf("Vecinos de %s:\n", pais->pais);
    for (int i = 0; i < pais->numVecinos; i++) {
        printf(" - %s\n", pais->vecinos[i]->pais);
    }
    if (pais->numVecinos == 0)
        printf(" (Sin vecinos registrados)\n");
    printf("\n");
}

// MAIN
int main() {
    struct Paises *lista = crearPaises();

    char *paises[] = {
        "Argentina", "Bolivia", "Brasil", "Chile", "Colombia",
        "Costa Rica", "Cuba", "Republica Dominicana", "Ecuador",
        "El Salvador", "Guatemala", "Honduras", "Mexico",
        "Nicaragua", "Panama", "Paraguay", "Peru",
        "Puerto Rico", "Uruguay", "Venezuela",
        "Haiti", "Belice", "Guyana", "Surinam"
    };

    // Insertar países
    for (int i = 0; i < 24; i++) {
        insertar_final(lista, paises[i], 5);
    }

    // Conectar algunos países como ejemplo
    struct Node *costaRica = buscarPais(lista, "Costa Rica");
    struct Node *panama = buscarPais(lista, "Panama");
    struct Node *nicaragua = buscarPais(lista, "Nicaragua");
    struct Node *colombia = buscarPais(lista, "Colombia");

    if (costaRica && panama) conectarPaises(costaRica, panama);
    if (costaRica && nicaragua) conectarPaises(costaRica, nicaragua);
    if (panama && colombia) conectarPaises(panama, colombia);

    // Mostrar lista
    imprimir_lista(lista);

    // Mostrar vecinos específicos
    imprimir_vecinos(costaRica);
    imprimir_vecinos(panama);

    return 0;
}

