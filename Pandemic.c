#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
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
void insertarProyecto(HashTable *tabla, const char *nombre, const char *descripcion,
                      const char *bibliografia, char **paises, int numPaises) {
    unsigned int indice = hash(nombre);

    Proyecto *nuevo = calloc(1, sizeof(Proyecto));
    nuevo->nombre = strdup(nombre);
    nuevo->descripcion = strdup(descripcion);
    nuevo->bibliografia = strdup(bibliografia);

    nuevo->numPaises = numPaises;
    nuevo->paises = calloc(numPaises, sizeof(char *));
    for (int i = 0; i < numPaises; i++)
        nuevo->paises[i] = strdup(paises[i]);

    nuevo->siguiente = tabla->buckets[indice];
    tabla->buckets[indice] = nuevo;
}

//-------------------- Agregar Pais --------------------
void agregarPais(char ***lista, int *cantidad, const char *nuevoPais) {
    if (lista == NULL || cantidad == NULL || nuevoPais == NULL) {
        printf("Error, punteros inválidos en agregarPais\n");
        return;
    }

    char **tmp = realloc(*lista, (*cantidad + 1) * sizeof(char *));
    if (tmp == NULL) {
        printf("Error al asignar memoria para los países\n");
        return;
    }

    *lista = tmp;
    (*lista)[*cantidad] = strdup(nuevoPais);
    (*cantidad)++;
}

// -------------------- Buscar elemento --------------------
Proyecto *buscarProyecto(HashTable *tabla, const char *nombre) {
    unsigned int indice = hash(nombre);
    Proyecto *actual = tabla->buckets[indice];

    while (actual) {
        if (strcmp(actual->nombre, nombre) == 0)
            return actual;
        actual = actual->siguiente;
    }
    return NULL;
}




struct Node {
    char *pais;				// nombre de pais
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
        printf(" %d %s\n", i, pais->vecinos[i]->pais);
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

// -------------------- Elegir Paises y Aspectos Aleatorios--------------------
int contarPaises(struct Paises *lista) {
    int contador = 0;
    struct Node *actual = lista->start;
    while (actual != NULL) {
        contador++;
        actual = actual->sigt;
    }
    return contador;
}
struct Node* obtenerPaisPorIndice(struct Paises *lista, int indice) {
    struct Node *actual = lista->start;
    int i = 0;
    while (actual != NULL) {
        if (i == indice) return actual;
        actual = actual->sigt;
        i++;
    }
    return NULL;
}

void diagnosticarProblemas(struct Paises *lista) {
    int total = contarPaises(lista);
    if (total < 9) {
        printf("No hay suficientes países para diagnóstico.\n");
        return;
    }

    srand(time(NULL)); // semilla aleatoria
    int usados[9];
    int usadosCount = 0;

    // Elegir 9 países únicos
    while (usadosCount < 9) {
        int randomIndex = rand() % total;

        // verificar que no esté repetido
        int repetido = 0;
        for (int i = 0; i < usadosCount; i++) {
            if (usados[i] == randomIndex) {
                repetido = 1;
                break;
            }
        }
        if (!repetido) {
            usados[usadosCount++] = randomIndex;
        }
    }

    // Asignar valores a los países seleccionados
    for (int i = 0; i < 9; i++) {
        struct Node *pais = obtenerPaisPorIndice(lista, usados[i]);
        if (!pais) continue;

        if (i < 3) { // Primer grupo
            pais->aspectos[0] = 3;
            pais->aspectos[1] = 2;
        } else if (i < 6) { // Segundo grupo
            pais->aspectos[0] = 2;
            pais->aspectos[1] = 1;
        } else { // Último grupo
            for (int j = 0; j < pais->numAspectos; j++)
                pais->aspectos[j] = 1;
        }

        printf("🌍 Diagnóstico inicial en %s → ", pais->pais);
        for (int j = 0; j < pais->numAspectos; j++) {
            printf("[%d]", pais->aspectos[j]);
            if (j < pais->numAspectos - 1) printf(" ");
        }
        printf("\n");
    }
}

struct Node *paisAleatorio(struct Paises *lista){
	int total = contarPaises(lista);
	srand(time(NULL));
	int i = rand() % total;
	return obtenerPaisPorIndice(lista, i);
}


int ejecucion(struct Node *pais) {
	int seleccion = 0;
	int seleccionPais = 0;
	printf("Que deseas hacer?\n1. Viajar\n2. Implementar proyecto\n(Presione 1 o 2): ");
	if (scanf("%d", &seleccion) != 1){
		printf("Error, parametros incorrectos\n");
		return -1;
	} else {
		if (seleccion == 1) {
			return 1;
			
		} else if (seleccion == 2){
			return 2;
			
		} else {
			printf("Error, numero no valido (1 o 2 solo)\n");
			return -1;
		}
	}
}

struct Node *viajar(struct Node *pais) {
	int seleccionPais = 0;
	imprimir_vecinos(pais);
	printf("Escoga el pais al que quiere viajar\n");
	scanf("%d", &seleccionPais);
	return pais->vecinos[seleccionPais];
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
    printf("\n🔬 Realizando diagnóstico inicial de problemas...\n\n");
    //diagnosticarProblemas(latam);
	
	HashTable *tabla = crearTabla();
	int tamaño = contarPaises(latam);
	
	//Paises donde se van implementando los proyectos
	char **paisesP1 = NULL;
	char **paisesP2 = NULL;
	char **paisesP3 = NULL;
	char **paisesP4 = NULL;
	char **paisesP5 = NULL;
	
	int numP1 = 0, numP2 = 0, numP3 = 0, numP4 = 0, numP5 = 0;
	
	//Problematica Princial: Crimen Organizado
	//Aspectos: Asesinatos, Robos, Narcotrafico, Extorsion y Secuestro, Corrupcion
	//1 Proyecto
	insertarProyecto(tabla, "Centros comunitarios juveniles", 
		"Crear centros comunitarios enfocados a proporcionar un entorno seguro y talleres de diversas areas",
        "Ott, C. (2020). Centros para la juventud: espacios para el desarrollo de habilidades físicas, sociales, emocionales y cognitivas. arch daily. https://www.archdaily.cl/cl/945587/centros-para-la-juventud-espacios-para-el-desarrollo-de-habilidades-fisicas-sociales-emocionales-y-cognitivas",
         paisesP1, numP1);
         
    //2 Proyecto
	insertarProyecto(tabla, "Rediseño urbano con seguridad aumentada", 
		"Realizar cambios y agregar nueva tecnologia capaz de mejorar la visibilidad y reducir las oportunidades de un delito",
        "Bibliografia",
         paisesP2, numP2);
         
    //3 Proyecto
	insertarProyecto(tabla, "Programa de reinsercion laboral", 
		"Realizar cambios y agregar nueva tecnologia capaz de mejorar la visibilidad y reducir las oportunidades de un delito",
        "Bibliografia",
         paisesP3, numP3);
         
    //4 Proyecto
	insertarProyecto(tabla, "Tecnologia y seguridad policial", 
		"Implementar tecnologias capaces de identificar y rastrear, ademas de aumentar el servicio policial",
        "Bibliografia",
         paisesP4, numP4);
         
    //5 Proyecto
	insertarProyecto(tabla, "Plataformas de transparencia", 
		"Plataformas para los ciudadanos donde es posible realizar solicitudes de información a los distintos organismos y sus integrantes",
        "Bibliografia",
         paisesP5, numP5);
         
    Proyecto *proyecto1 = buscarProyecto(tabla, "Centros comunitarios juveniles");     
    Proyecto *proyecto2 = buscarProyecto(tabla, "Rediseño urbano con seguridad aumentada");   
    Proyecto *proyecto3 = buscarProyecto(tabla, "Programa de reinsercion laboral");   
    Proyecto *proyecto4 = buscarProyecto(tabla, "Tecnologia y seguridad policial");   
    Proyecto *proyecto5 = buscarProyecto(tabla, "Plataformas de transparencia");   
         
		
        
    imprimir_lista(latam);
    
    
    struct Node *paisJ1 = paisAleatorio(latam);
    struct Node *paisJ2 = paisAleatorio(latam);
	printf("Bienvenido, jugador 1, en este momento estas en: %s\n", paisJ1->pais);
	int seleccionPais = ejecucion(paisJ1);
	
	if (seleccionPais==1) {
		paisJ1 = viajar(paisJ1);
	} else if (seleccionPais==2) {
		printf("\nCual proyecto desea aplicar?\n");
		if (scanf("%d", &seleccionPais) != 1) {
			printf("Error, entrada inválida\n");
			return -1;
		}
	//agregarle el pais al array del proyecto

		switch (seleccionPais) {
			case 1:
				agregarPais(&paisesP1, &(proyecto1->numPaises), paisJ1->pais);
				break;
			case 2:
				agregarPais(&paisesP2, &(proyecto2->numPaises), paisJ1->pais);
				break;
			case 3:
				agregarPais(&paisesP3, &(proyecto3->numPaises), paisJ1->pais);
				break;
			case 4:
				agregarPais(&paisesP4, &(proyecto4->numPaises), paisJ1->pais);
				break;
			case 5:
				agregarPais(&paisesP5, &(proyecto5->numPaises), paisJ1->pais);
				break;
			default:
				printf("Error, parámetro inválido.\n");
				return -1;
		}
	}
	

         
    liberarMapa(latam);
    liberarTabla(tabla);
    return 0;
}

