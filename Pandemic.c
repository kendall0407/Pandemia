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
struct Paises* crearMapaLatinoamerica() {
    struct Paises *lista = crearPaises();

    // Crear países
    char *paises[] = {
        "Argentina", "Bolivia", "Brasil", "Chile", "Colombia",
        "Costa Rica", "Cuba", "Republica Dominicana", "Ecuador",
        "El Salvador", "Guatemala", "Honduras", "Mexico",
        "Nicaragua", "Panama", "Paraguay", "Peru",
        "Puerto Rico", "Uruguay", "Venezuela",
        "Haiti", "Belice", "Guyana", "Surinam"
    };

    for (int i = 0; i < 24; i++)
        insertar_final(lista, paises[i], 5);

    // Buscar punteros
    struct Node *arg = buscarPais(lista, "Argentina");
    struct Node *bol = buscarPais(lista, "Bolivia");
    struct Node *bra = buscarPais(lista, "Brasil");
    struct Node *chi = buscarPais(lista, "Chile");
    struct Node *col = buscarPais(lista, "Colombia");
    struct Node *crc = buscarPais(lista, "Costa Rica");
    struct Node *ecu = buscarPais(lista, "Ecuador");
    struct Node *sal = buscarPais(lista, "El Salvador");
    struct Node *gua = buscarPais(lista, "Guatemala");
    struct Node *hon = buscarPais(lista, "Honduras");
    struct Node *mex = buscarPais(lista, "Mexico");
    struct Node *nic = buscarPais(lista, "Nicaragua");
    struct Node *pan = buscarPais(lista, "Panama");
    struct Node *par = buscarPais(lista, "Paraguay");
    struct Node *per = buscarPais(lista, "Peru");
    struct Node *uru = buscarPais(lista, "Uruguay");
    struct Node *ven = buscarPais(lista, "Venezuela");
    struct Node *guy = buscarPais(lista, "Guyana");
    struct Node *sur = buscarPais(lista, "Surinam");
    struct Node *bel = buscarPais(lista, "Belice");

    // 🔗 Conexiones geográficas
    conectarPaises(arg, chi);
    conectarPaises(arg, bol);
    conectarPaises(arg, par);
    conectarPaises(arg, uru);

    conectarPaises(bol, arg);
    conectarPaises(bol, par);
    conectarPaises(bol, per);
    conectarPaises(bol, bra);
    conectarPaises(bol, chi);

    conectarPaises(bra, arg);
    conectarPaises(bra, uru);
    conectarPaises(bra, par);
    conectarPaises(bra, bol);
    conectarPaises(bra, per);
    conectarPaises(bra, ven);
    conectarPaises(bra, guy);
    conectarPaises(bra, sur);

    conectarPaises(chi, arg);
    conectarPaises(chi, bol);
    conectarPaises(chi, per);

    conectarPaises(per, chi);
    conectarPaises(per, bol);
    conectarPaises(per, ecu);
    conectarPaises(per, bra);
    conectarPaises(per, col);

    conectarPaises(ecu, per);
    conectarPaises(ecu, col);

    conectarPaises(col, ecu);
    conectarPaises(col, ven);
    conectarPaises(col, pan);
    conectarPaises(col, per);
    conectarPaises(col, bra);

    conectarPaises(pan, col);
    conectarPaises(pan, crc);

    conectarPaises(crc, pan);
    conectarPaises(crc, nic);

    conectarPaises(nic, crc);
    conectarPaises(nic, hon);

    conectarPaises(hon, nic);
    conectarPaises(hon, sal);
    conectarPaises(hon, gua);

    conectarPaises(sal, hon);
    conectarPaises(sal, gua);

    conectarPaises(gua, sal);
    conectarPaises(gua, mex);
    conectarPaises(gua, bel);

    conectarPaises(bel, gua);
    conectarPaises(bel, mex);

    conectarPaises(mex, gua);
    conectarPaises(mex, bel);

    conectarPaises(ven, col);
    conectarPaises(ven, bra);
    conectarPaises(ven, guy);

    conectarPaises(guy, ven);
    conectarPaises(guy, sur);
    conectarPaises(guy, bra);

    conectarPaises(sur, guy);
    conectarPaises(sur, bra);

    conectarPaises(uru, arg);
    conectarPaises(uru, bra);

    return lista;
}


//Tabla de dispersion
#define TABLE_SIZE 100

// -------------------- Estructuras --------------------
typedef struct Proyecto {
    char *nombre;
    char *descripcion;
    char *bibliografia;
    char **paises;  // arreglo dinámico de cadenas
    int numPaises;
    struct Proyecto *siguiente;  // para colisiones
} Proyecto;

typedef struct {
    Proyecto *buckets[TABLE_SIZE];
} HashTable;

// -------------------- Función hash --------------------
unsigned int hash(const char *str) {
    unsigned int h = 0;
    while (*str)
        h = (h * 31) + *str++;
    return h % TABLE_SIZE;
}

// -------------------- Crear tabla --------------------
HashTable *crearTabla() {
    HashTable *tabla = calloc(1, sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++)
        tabla->buckets[i] = NULL;
    return tabla;
}

// -------------------- Insertar elemento --------------------
void insertar(HashTable *tabla, const char *nombre, const char *descripcion,
                      const char *bibliografia, char **paises, int numPaises) {
    unsigned int indice = hash(nombre);

    Proyecto *nuevo = malloc(sizeof(Proyecto));
    nuevo->nombre = strdup(nombre);
    nuevo->descripcion = strdup(descripcion);
    nuevo->bibliografia = strdup(bibliografia);

    nuevo->numPaises = numPaises;
    nuevo->paises = malloc(numPaises * sizeof(char *));
    for (int i = 0; i < numPaises; i++)
        nuevo->paises[i] = strdup(paises[i]);

    nuevo->siguiente = tabla->buckets[indice];
    tabla->buckets[indice] = nuevo;
}


// -------------------- Buscar elemento --------------------
Proyecto *buscar(HashTable *tabla, const char *nombre) {
    unsigned int indice = hash(nombre);
    Proyecto *actual = tabla->buckets[indice];

    while (actual) {
        if (strcmp(actual->nombre, nombre) == 0)
            return actual;
        actual = actual->siguiente;
    }
    return NULL;
}

// -------------------- Liberar memoria --------------------
void liberarMapa(struct Paises *lista) {
    if (!lista) return;

    struct Node *actual = lista->start;
    while (actual != NULL) {
        struct Node *temp = actual->sigt;
        free(actual->aspectos);
        free(actual->vecinos);
        free(actual);
        actual = temp;
    }
    free(lista);
}

void liberarTabla(HashTable *tabla) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Proyecto *actual = tabla->buckets[i];
        while (actual) {
            Proyecto *tmp = actual;

            free(actual->nombre);
            free(actual->descripcion);
            free(actual->bibliografia);

            for (int j = 0; j < actual->numPaises; j++)
                free(actual->paises[j]);
            free(actual->paises);

            actual = actual->siguiente;
            free(tmp);
        }
    }
    free(tabla);
}




// MAIN
int main() {
    struct Paises *latam = crearMapaLatinoamerica();
    imprimir_lista(latam);
    HashTable *tabla = crearTabla();

    char *paises1[] = {"Costa Rica", "Mexico", "Chile"};
    insertar(tabla, 
        "Policias", 
        "Incrementar y mejorar la calidad de la fuerza publica", 
        "Referencia",
        paises1, 3
    );
    Proyecto *p = buscar(tabla, "Agrotech");
    if (p) {
        printf("📘 Proyecto: %s\n", p->nombre);
        printf("Descripción: %s\n", p->descripcion);
        printf("Bibliografía: %s\n", p->bibliografia);
        printf("Países aplicados:\n");
        for (int i = 0; i < p->numPaises; i++)
            printf(" - %s\n", p->paises[i]);
    }
    liberarTabla(tabla);    
    liberarMapa(latam);
    return 0;
}
