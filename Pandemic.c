#include <stdio.h>
#include <stdlib.h>

struct Node {
	char *pais;
    int *aspectos;
    int *numAspectos;


    struct Pais **vecinos; // arreglo de punteros a otros paises
    int numVecinos;  


	struct Node *sigt;
	struct Node *ant;
};

struct Paises {
	struct Node *start;
};

struct Paises *crearPais() {
    struct Paises *newList = calloc(1, sizeof(struct Paises));
    if (newList == NULL) {
        printf("No se pudieron crear los paises\n");
    }
    return newList;
}

struct Node* createNewNode(char* pais, int numAspectos) {
    struct Node *newNode = calloc(1, sizeof(struct Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode -> aspectos = calloc(numAspectos, sizeof(int));
    newNode -> pais = pais;
    newNode -> numAspectos = numAspectos;

    newNode->vecinos = NULL;     // no tiene vecinos todavia
    newNode->numVecinos = 0;

    newNode->ant = NULL;
    newNode->sigt = NULL;
    return newNode;
}
void agregarVecino(struct Node *pais, struct Node *nuevoVecino) {
    pais->numVecinos++;
    pais->vecinos = realloc(pais->vecinos, pais->numVecinos * sizeof(struct Pais *));
    pais->vecinos[pais->numVecinos - 1] = nuevoVecino;
}
void conectarPaises(struct Node *a, struct Node *b) {
    agregarVecino(a, b);
    agregarVecino(b, a);
}


int insertar_final(struct Paises *lista, char *elemento, int numAspectos){
	if (lista == NULL) {
        printf("No es lista valida\n");
        return -1;
    }
    struct Node* newNode = createNewNode(elemento, numAspectos);
	if (newNode == NULL) {
		printf("Error creando un nuevo nodo\n");
		return -1;
	} else if (lista->start == NULL) {
		lista -> start = newNode;
	} else {
		struct Node* currentNode = lista->start;
		while (currentNode->sigt != NULL) {
			currentNode = currentNode -> sigt;
		}
		currentNode->sigt = newNode;
		newNode-> ant = currentNode;
	}
	return 0; 
}
void imprimir_lista(struct Paises *lista){
    if (lista == NULL) {
        printf("No es lista valida\n");
    }
    struct Node* currentNode = lista->start;

    while (currentNode != NULL) {
        printf("Pais: %s\n", currentNode->pais);
        currentNode = currentNode->sigt;
    }

    printf("Se mostraron todos los valores\n\n");
}

int main() {
	struct Paises *lista = crearPais();
	char *paises[] = {
	"Argentina", "Bolivia", "Brasil", "Chile", "Colombia",
	"Costa Rica", "Cuba", "Republica Dominicana", "Ecuador",
	"El Salvador", "Guatemala", "Honduras", "Mexico",
	"Nicaragua", "Panama", "Paraguay", "Peru",
	"Puerto Rico", "Uruguay", "Venezuela",
	"Haiti", "Belice", "Guyana", "Surinam"
    };

    for (int i = 0; i < 24; i++) {
        insertar_final(lista, paises[i],5);
    }
    imprimir_lista(lista);
    return 0;
}
