#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

//Tabla de dispersion tamano
#define TABLE_SIZE 100

// -------------------- Estructura de hash --------------------
typedef struct Proyecto {
    char *nombre;
    char *descripcion;
    char *bibliografia;
    char **paises;  // arreglo dinámico de cadenas
    int numPaises;
    struct Proyecto *sigt;  // para colisiones
} Proyecto;

typedef struct {
    Proyecto *buckets[TABLE_SIZE];
} HashTable;

// -------------------- Función hash --------------------
unsigned int hash(const char *str) {	//valores no pueden ser negativos
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
    unsigned int indice = hash(nombre);	//determina en que bucket del arreglo se guardara el proyecto

    Proyecto *nuevo = calloc(1, sizeof(Proyecto));
    nuevo->nombre = strdup(nombre);		//crea copia dinamica de los campos
    nuevo->descripcion = strdup(descripcion);
    nuevo->bibliografia = strdup(bibliografia);

    nuevo->numPaises = numPaises;
    nuevo->paises = calloc(numPaises, sizeof(char *));	//inicializa arreglo de punteros a cadenas (strings)
    for (int i = 0; i < numPaises; i++)
        nuevo->paises[i] = strdup(paises[i]);			

    nuevo->sigt = tabla->buckets[indice];		//insertar el proyecto al inicio de la lista enlazada en el bucket
    tabla->buckets[indice] = nuevo;			//si ya había proyectos con el mismo hash, quedan detráa del nuevo nodo
}

// -------------------- Imprimir todos los proyectos --------------------
void imprimirProyectos(HashTable *tabla) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Proyecto *actual = tabla->buckets[i];
        while (actual) {
            printf("📌 Nombre: %s\n", actual->nombre);
            printf("📝 Descripcion: %s\n", actual->descripcion);
            printf("📚 Bibliografia: %s\n", actual->bibliografia);
            
            if (actual->numPaises > 0) {
				printf("🌎 Paises: ");
				for (int j = 0; j < actual->numPaises; j++) {
					printf("🏳️ %s\n", actual->paises[j]);
				}
			} else {
				printf("⚠️ Este proyecto no se ha aplicado en ningun pais aun");
			}
            printf("\n-----------------------\n");
            
            actual = actual->sigt; // correcto
        }
    }
}


// -------------------- Buscar elemento --------------------
Proyecto *buscarProyecto(HashTable *tabla, const char *nombre) {
    unsigned int indice = hash(nombre);
    Proyecto *actual = tabla->buckets[indice];

    while (actual) {
        if (strcmp(actual->nombre, nombre) == 0)
            return actual;
        actual = actual->sigt;
    }
    return NULL;
}
// -------------------- Agrega Pais al Proyecto --------------------
void agregarPaisAProyecto(Proyecto *proyecto, const char *nuevoPais) {
    if (proyecto == NULL || nuevoPais == NULL) {
        printf("❌ Error: punteros nulos en agregarPaisAProyecto\n");
        return;
    }

    // Aumentar tamaño del arreglo de países
    char **nuevaLista = realloc(proyecto->paises, (proyecto->numPaises + 1) * sizeof(char *));
    if (nuevaLista == NULL) {
        printf("❌ Error al reasignar memoria para países del proyecto %s\n", proyecto->nombre);
        return;
    }

    proyecto->paises = nuevaLista;
    proyecto->paises[proyecto->numPaises] = strdup(nuevoPais);
    proyecto->numPaises++;

    printf("✅ País agregado al proyecto '%s': %s\n", proyecto->nombre, nuevoPais);
}



// -------------------- Buscar elemento --------------------
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

// -------------------- Crea Lista de Paises--------------------
struct Paises *crearPaises() {
    struct Paises *newList = calloc(1, sizeof(struct Paises));
    if (newList == NULL) {
        printf("No se pudieron crear los paises\n");
    }
    return newList;
}

// -------------------- Crea Pais--------------------
struct Node* createNewNode(char* pais) {
    struct Node *newNode = calloc(1, sizeof(struct Node));
    if (newNode == NULL) {
        printf("Error creando un nodo\n");
        return NULL;
    }
    newNode->pais = strdup(pais);
    newNode->aspectos = calloc(5, sizeof(int));
    newNode->numAspectos = 5;

    newNode->vecinos = NULL;
    newNode->numVecinos = 0;

    return newNode;
}

// --------------------Agrega Vecino a Pais--------------------
void agregarVecino(struct Node *pais, struct Node *nuevoVecino) {

    // Revisar si ya existe
    for (int i = 0; i < pais->numVecinos; i++) {
        if (pais->vecinos[i] == nuevoVecino)
            return; // ya existe, no agregamos
    }

    struct Node **temp = realloc(pais->vecinos, (pais->numVecinos + 1) * sizeof(struct Node *));
    if (!temp) {
        printf("Error: no se pudo asignar memoria para agregar vecino a %s\n", pais->pais);
        return; // evitar segfault
    }

    pais->vecinos = temp;
    pais->vecinos[pais->numVecinos] = nuevoVecino;
    pais->numVecinos++;
}


// -------------------- Conecta Paises  --------------------
void conectarPaises(struct Node *a, struct Node *b) {
    agregarVecino(a, b);
    agregarVecino(b, a);
}

// -------------------- Inserta al Final --------------------
int insertar_final(struct Paises *lista, char *elemento) {
    if (lista == NULL) {
        printf("No es lista valida\n");
        return -1;
    }

    struct Node* newNode = createNewNode(elemento);
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

// -------------------- Busca Pais --------------------
struct Node* buscarPais(struct Paises *lista, char *nombre) {
    struct Node *current = lista->start;
    while (current != NULL) {
        if (strcmp(current->pais, nombre) == 0)
            return current;
        current = current->sigt;
    }
    return NULL;
}

// -------------------- Imprime Lista --------------------
/*  COLORES 
    printf("\033[31mEste texto es rojo\033[0m\n");
    printf("\033[32mEste texto es verde\033[0m\n");
    printf("\033[33mEste texto es amarillo\033[0m\n");
    printf("\033[34mEste texto es azul\033[0m\n");
*/     
void imprimir_lista(struct Paises *lista) {
    if (lista == NULL || lista->start == NULL) {
        printf("⚠️  No hay países en la lista.\n");
        return;
    }

    struct Node *actual = lista->start;
    printf("\n🌎 ====== ESTADO ACTUAL DEL MAPA DE AMÉRICA LATINA ======\n\n");

    while (actual != NULL) {
        printf("\033[31m🗺️  País: %s\033[0m\n", actual->pais);

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

// -------------------- Imprime Vecinos de un Pais --------------------
void imprimir_vecinos(struct Node *pais) {
    printf("Vecinos de %s:\n", pais->pais);
    for (int i = 0; i < pais->numVecinos; i++) {
        printf(" %d %s\n", i, pais->vecinos[i]->pais);
    }
    if (pais->numVecinos == 0)
        printf(" (Sin vecinos registrados)\n");
    printf("\n");
}
// -------------------- Crea el Mapa de LATAM --------------------
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
        insertar_final(lista, paises[i]);

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
    struct Node *cub = buscarPais(lista, "Cuba");
	struct Node *dom = buscarPais(lista, "Republica Dominicana");
	struct Node *pr = buscarPais(lista, "Puerto Rico");
	struct Node *hti = buscarPais(lista, "Haiti");

	// conexiones geograficas 
	conectarPaises(arg, chi);
	conectarPaises(arg, bol);
	conectarPaises(arg, par);
	conectarPaises(arg, uru);
	conectarPaises(arg, bra);

	conectarPaises(bol, per);
	conectarPaises(bol, bra);
	conectarPaises(bol, chi);
	conectarPaises(bol, sur);   // agregado para equilibrio

	conectarPaises(bra, ven);
	conectarPaises(bra, guy);
	conectarPaises(bra, sur);
	conectarPaises(bra, col);
	conectarPaises(bra, uru);

	conectarPaises(chi, per);
	conectarPaises(chi, guy);
	conectarPaises(chi, bel);
	conectarPaises(chi, ecu);   // agregado

	conectarPaises(per, ecu);
	conectarPaises(per, col);
	conectarPaises(per, chi);

	conectarPaises(ecu, ven);
	conectarPaises(ecu, col);
	conectarPaises(ecu, guy);   // agregado

	conectarPaises(col, pan);
	conectarPaises(col, bra);
	conectarPaises(col, ven);

	conectarPaises(pan, crc);
	conectarPaises(pan, nic);
	conectarPaises(pan, mex);

	conectarPaises(crc, nic);
	conectarPaises(crc, hon);
	conectarPaises(crc, mex);

	conectarPaises(nic, sal);
	conectarPaises(nic, hon);

	conectarPaises(hon, gua);
	conectarPaises(hon, sal);

	conectarPaises(sal, gua);

	conectarPaises(gua, mex);
	conectarPaises(gua, bel);

	conectarPaises(bel, mex);
	conectarPaises(bel, crc);
	conectarPaises(bel, gua);

	conectarPaises(mex, col);

	conectarPaises(ven, guy);
	conectarPaises(ven, ecu);
	conectarPaises(ven, bra);

	conectarPaises(guy, sur);
	conectarPaises(guy, chi);

	conectarPaises(sur, par);
	conectarPaises(sur, bol);

	conectarPaises(uru, par);
	conectarPaises(uru, bra);

	conectarPaises(par, bol);
	conectarPaises(par, sur);
	conectarPaises(par, crc);   // agregado para evitar duplicado (en lugar de arg)
	
	conectarPaises(cub, pr);   // Costa Rica
	conectarPaises(cub, mex);   // Mexico
	conectarPaises(cub, gua);   // Guatemala

	// República Dominicana
	conectarPaises(dom, col);   // Colombia
	conectarPaises(dom, crc);   // Venezuela
	conectarPaises(dom, per);   // Peru

	// Puerto Rico
	conectarPaises(pr, crc);    // Panama
	conectarPaises(pr, ecu);    // Ecuador
	conectarPaises(pr, arg);    // Argentina

	// Haití
	conectarPaises(hti, sur);   // Venezuela
	conectarPaises(hti, sal);   // Brasil
	conectarPaises(hti, chi);   // Chile

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
// -------------------- Busca el Pais por indice --------------------
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
// -------------------- Agregar Problemas Random --------------------
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
//  Aumentar aleatoriamente las problemáticas en algunos países
void aumentarProblemasAleatorios(struct Paises *lista) {
    srand(time(NULL));
    struct Node *actual = lista->start;

    while (actual != NULL) {
        // 30% de probabilidad de aumentar un aspecto
        if ((rand() % 100) < 30) {
            int indice = rand() % actual->numAspectos;
            if (actual->aspectos[indice] < 3) {
                actual->aspectos[indice]++;
                printf("⚠️  Aumenta problema en %s (aspecto %d → %d)\n",
                       actual->pais, indice + 1, actual->aspectos[indice]);
            }
        }
        actual = actual->sigt;
    }
}

// Expandir problemáticas a vecinos si algún país llega a nivel 3
void expandirProblemas(struct Paises *lista) {
    struct Node *actual = lista->start;

    while (actual != NULL) {
        for (int i = 0; i < actual->numAspectos; i++) {
            if (actual->aspectos[i] >= 3) {
                // Infectar a los vecinos
                for (int j = 0; j < actual->numVecinos; j++) {
                    struct Node *vecino = actual->vecinos[j];
                    int aspectoAfectado = rand() % vecino->numAspectos;

                    if (vecino->aspectos[aspectoAfectado] < 3) {
                        vecino->aspectos[aspectoAfectado]++;
                        printf("🦠 Expansión de %s → %s (aspecto %d sube a %d)\n",
                               actual->pais, vecino->pais,
                               aspectoAfectado + 1,
                               vecino->aspectos[aspectoAfectado]);
                    }
                }
            }
        }
        actual = actual->sigt;
    }
}

// Verificar condiciones de victoria o derrota
int verificarEstadoJuego(struct Paises *lista) {
    int paisesTotales = 0;
    int paisesColapsados = 0;
    int paisesEstables = 0;
    struct Node *actual = lista->start;

    while (actual != NULL) {
        paisesTotales++;
        int colapsado = 1;
        int estable = 1;

        for (int i = 0; i < actual->numAspectos; i++) {
            if (actual->aspectos[i] < 3)
                colapsado = 0; // no colapsado

            if (actual->aspectos[i] > 1)
                estable = 0; // no estable
        }

        if (colapsado) paisesColapsados++;
        if (estable) paisesEstables++;

        actual = actual->sigt;
    }

    if (paisesTotales == 0) return -1; // todos eliminados, derrota total

    if (paisesColapsados >= 3) {
        return -1; // derrota
    }

    if (paisesEstables == paisesTotales) {
        return 1; // victoria
    }

    return 0; // sigue el juego
}

// -------------------- Asigna un pais Random --------------------
struct Node *paisAleatorio(struct Paises *lista){
	int total = contarPaises(lista);
	int i = rand() % total;
	return obtenerPaisPorIndice(lista, i);
}


//------------------- Eliminar paises con todos los aspectos al maximo--------------
void eliminarPaisesColapsados(struct Paises *lista, struct Node **paisJ1, struct Node **paisJ2) {
    struct Node *actual = lista->start;

    while (actual != NULL) {
        int colapsado = 1;		
        for (int i = 0; i < actual->numAspectos; i++) {
            if (actual->aspectos[i] < 3) {		//verifica que un aspecto sea menor a 3, lo que indica que no esta colapsado
                colapsado = 0;
                break;		//si todo esta bien se va al siguiente nodo
            }
        }

        if (colapsado) {
            printf("💀 %s ha colapsado completamente.\n", actual->pais);

            // desconectar vecinos
            for (int i = 0; i < actual->numVecinos; i++) {
                struct Node *vecino = actual->vecinos[i];
                for (int j = 0; j < vecino->numVecinos; j++) {
                    if (vecino->vecinos[j] == actual) {
                        for (int k = j; k < vecino->numVecinos - 1; k++)
                            vecino->vecinos[k] = vecino->vecinos[k + 1];
                        
                        vecino->numVecinos--;
                        break;  
                    }
                }
            }

            // eliminar nodo de la lista
            struct Node *temp = actual;
            
            if (actual->ant) actual->ant->sigt = actual->sigt;
            if (actual->sigt) actual->sigt->ant = actual->ant;
            if (lista->start == actual) lista->start = actual->sigt;

            actual = actual->sigt;
            
            if (temp == *paisJ1) {
                *paisJ1 = paisAleatorio(lista);
                if (!*paisJ1){
                    printf("Tu pais fue colapsado y no puedes huir a otro, perderas todos tus turnos");
                }
            }

            if (temp == *paisJ2) {
                *paisJ2 = paisAleatorio(lista);
                if (!*paisJ2){
                    printf("Tu pais fue colapsado y no puedes huir a otro, perderas todos tus turnos");
                }
            }

            free(temp->aspectos);
            free(temp->vecinos);
            free(temp->pais);
            free(temp);
        } else {
            actual = actual->sigt;
        }
    }
}
// -------------------- Pregunta Que Quiere Hacer --------------------
int ejecucion(struct Node *pais) {
	int seleccion = 0;
	int seleccionPais = 0;
	printf("Que deseas hacer?\n1. Viajar\n2. Implementar proyecto\n3. Ver informacion de paises \n4. Salir\n(Presione 1,2,3 o 4): ");
	if (scanf("%d", &seleccion) != 1) {
		printf("Error, parametros incorrectos\n");
		return -1;
	}

	switch (seleccion) {
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 3;
		case 4:
			return 4;
		default:
			printf("Error, numero no valido (1, 2, 3 o 4)\n");
			return -1;
	}
}
// -------------------- Viaja de Un Pais a Otro vecino --------------------

struct Node *viajar(struct Node *pais) {
    if (pais->numVecinos == 0) {
        printf("⚠️ No hay vecinos donde viajar te asignaremos otro pais\n");
        return NULL;
    }
	int seleccionPais = -1;
	imprimir_vecinos(pais);
	printf("Escoga el pais al que quiere viajar\n");
	while (1) {
		if (scanf("%d", &seleccionPais) != 1){
			printf("❌ Error, parametros incorrectos\n");
			while (getchar() != '\n'); // limpiar buffer
			continue;
		} 
        
		if (seleccionPais < 0 || seleccionPais >= pais->numVecinos) {
            printf("❌ Error, selección fuera de rango. Intente de nuevo.\n");
            continue;
        }

        if (pais->vecinos[seleccionPais] != NULL) {
            return pais->vecinos[seleccionPais];
        } else {
            printf("❌ Error, no hay vecino en esa posición. Intente de nuevo.\n");
        }
	}
}
// ------------------ Liberar memoria de la lista -------------------
void liberarMapa(struct Paises *lista) {
    if (!lista) return;

    struct Node *actual = lista->start;
    while (actual != NULL) {
        struct Node *temp = actual->sigt;
        free(actual->aspectos);
        free(actual->vecinos);
        free(actual->pais);
        free(actual);
        actual = temp;
    }
    free(lista);
}
// -------------------- Libera Memoria De Tabla --------------------

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

            actual = actual->sigt;
            free(tmp);
        }
    }
    free(tabla);
}
// -------------------- Maneja el Turno del Jugador --------------------
int turnoJugador(int numeroJugador, struct Node **paisActual, 
                 Proyecto *p1, Proyecto *p2, Proyecto *p3, Proyecto *p4, Proyecto *p5,
                 struct Paises *mapa) {
    printf("\n====================\n");
    printf("🎮 Turno del Jugador %d\n", numeroJugador);            
	printf("Actualmente estás en: %s\n", (*paisActual)->pais);

    printf("====================\n");

    int accion = 1;
    while (accion <= 4) {
        printf("\n🌀 Acción %d de 4\n", accion);
        int eleccion = ejecucion(*paisActual);

        if (eleccion == 1) {
            // VIAJAR
            *paisActual = viajar(*paisActual);
            if (!*paisActual) {
				printf("Ocurrio un error, no puedes viajar. Te asignaremos otro pais.\n");
                *paisActual = paisAleatorio(mapa);
                if (!*paisActual) {
                    printf("No hay mas paises al cual viajar, perderas todos tus turnos\n");
                    accion = 5;
                }
                printf("🚶 Pais asignado: %s\n", (*paisActual)->pais);
			} else {
				printf("🚶 Viajaste a: %s\n", (*paisActual)->pais);
				accion++;  // gastar acción
			} 
        } else if (eleccion == 2) {
            // APLICAR PROYECTO
            int seleccionProyecto = 0;
            printf("\nCual proyecto desea aplicar?\n");
            printf("1. Centros comunitarios juveniles\n");
            printf("2. Rediseño urbano con seguridad aumentada\n");
            printf("3. Programa de reinsercion laboral\n");
            printf("4. Tecnologia y seguridad policial\n");
            printf("5. Plataformas de transparencia\n");
            printf("Seleccione (1-5): ");

            if (scanf("%d", &seleccionProyecto) != 1) {
                printf("❌ Entrada inválida.\n");
                while (getchar() != '\n'); // limpiar buffer
                continue;
            }

            // Bajar nivel de problemáticas
            if (seleccionProyecto >= 1 && seleccionProyecto <= (*paisActual)->numAspectos) {
                if ((*paisActual)->aspectos[seleccionProyecto - 1] > 0) {
                    switch(seleccionProyecto) {
                        case 1:
                            (*paisActual)->aspectos[seleccionProyecto - 1]--;
                    }
                    
                    printf("📉 Problemas reducidos en %s\n", (*paisActual)->pais);
                } else {
                    printf("⚪ Ese aspecto ya está en su nivel mínimo.\n");
                }

                // Asociar país al proyecto
                switch (seleccionProyecto) {
                    case 1: 
						agregarPaisAProyecto(p1, (*paisActual)->pais); 
						break;
                    case 2: 
						agregarPaisAProyecto(p2, (*paisActual)->pais); 
						break;
                    case 3: 
						agregarPaisAProyecto(p3, (*paisActual)->pais); 
						break;
                    case 4: 
						agregarPaisAProyecto(p4, (*paisActual)->pais); 
						break;
                    case 5: 
						agregarPaisAProyecto(p5, (*paisActual)->pais); 
						break;
                }
            } else {
                printf("❌ Número de proyecto inválido.\n");
                continue;
            }

            accion++;  // gastar acción
        }
        else if (eleccion == 3) {
            // VER INFORMACIÓN — NO GASTA TURNO
            printf("\n📊 Información actual del mapa:\n");
            imprimir_lista(mapa);
            printf("🔁 Esta acción no consume turno.\n");
            continue;  // no incrementar acción
        }
        else if (eleccion == 4) {
            // SALIR DEL JUEGO
            printf("\n🚪 El jugador %d decidió salir del juego.\n", numeroJugador);
            return -1; // señal para terminar juego
        }
        else {
            printf("⚠️ Acción inválida. Se pierde esta acción.\n");
            accion++;  // igual gasta turno
        }
    }

    return 0; // turno completado normalmente
}


// MAIN
int main() {
    srand(time(NULL)); // semilla única
    struct Paises *latam = crearMapaLatinoamerica();

    printf("\n🔬 Realizando diagnóstico inicial de problemas...\n\n");
    diagnosticarProblemas(latam);

    HashTable *tabla = crearTabla();

    insertarProyecto(tabla, "Centros comunitarios juveniles", 
        "Crear centros comunitarios enfocados a proporcionar un entorno seguro.",
        "Ott, C. (2020). ArchDaily.", NULL, 0);

    insertarProyecto(tabla, "Rediseño urbano con seguridad aumentada", 
        "Crear ciudades seguras, cohesionadas y resilientes mediante la planificación y el diseño urbanos: Diez consideraciones para alcaldes y gobiernos locales.",
        "Strong Cities Network. (2024).", NULL, 0);

    insertarProyecto(tabla, "Programa de reinsercion laboral", 
        "Reinserción laboral: ¿qué es y en qué consisten sus programas?",
        "Salinas, A. (s.f).", NULL, 0);

    insertarProyecto(tabla, "Tecnologia y seguridad policial", 
        "El desarrollo tecnológico en materia policial: una receta de éxito para la prevención del delito.",
        "Villalobos, H. (2020).", NULL, 0);

    insertarProyecto(tabla, "Plataformas de transparencia", 
        "Portal de transparencia: Una herramienta esencial para la gestión pública.",
        "As-Tech-Solutions. (s.f).",NULL, 0);

    Proyecto *p1 = buscarProyecto(tabla, "Centros comunitarios juveniles");
    Proyecto *p2 = buscarProyecto(tabla, "Rediseño urbano con seguridad aumentada");
    Proyecto *p3 = buscarProyecto(tabla, "Programa de reinsercion laboral");
    Proyecto *p4 = buscarProyecto(tabla, "Tecnologia y seguridad policial");
    Proyecto *p5 = buscarProyecto(tabla, "Plataformas de transparencia");

    // Jugadores
    struct Node *paisJ1 = paisAleatorio(latam);
    struct Node *paisJ2 = paisAleatorio(latam);
	
    printf("\n🎮 Jugador 1 empieza en: %s\n", paisJ1->pais);
    printf("🎮 Jugador 2 empieza en: %s\n", paisJ2->pais);
	
	int seleccionProyecto = 0;
	while(1) {
		printf("Antes de empezar, ¿desea ver los proyectos disponibles? (1 para si, 2 para no):\n");
		if (scanf("%d", &seleccionProyecto) != 1) {
			printf("❌ Entrada inválida.\n");
			while (getchar() != '\n'); // limpiar buffer
			continue;
		} else if (seleccionProyecto == 1) {
			imprimirProyectos(tabla);
			break; // entrada válida, salir del bucle
		} else if (seleccionProyecto == 2) {
			break;
		} else {
			printf("❌ Solo puede ingresar 1 o 2. Intente de nuevo.\n");
		}
	}
    //  Turnos alternados
    for (int ronda = 1; ronda <= 10; ronda++) {     // máximo 10 rondas
        printf("\n==============================\n");
        printf("🔁 RONDA %d\n", ronda);
        printf("==============================\n");

		
		if (!&paisJ1) {
			paisJ1 = paisAleatorio(latam);
		} else if (!&paisJ2){
			paisJ2 = paisAleatorio(latam);
		} 


		if (turnoJugador(1, &paisJ1, p1, p2, p3, p4, p5, latam) == -1) break;
		if (turnoJugador(2, &paisJ2, p1, p2, p3, p4, p5, latam) == -1) break;


        printf("\n🌡️  Fin de la ronda: aplicando efectos...\n");
        aumentarProblemasAleatorios(latam);
        expandirProblemas(latam);
        eliminarPaisesColapsados(latam, &paisJ1, &paisJ2);   //se le pasan los paises para ver si uno de esos es eliminado
        
		int estado = verificarEstadoJuego(latam);
		if (estado == -1) {
            printf("\n");
            printf("       ______\n");
            printf("     .-        -.\n");
            printf("    /            \\\n");
            printf("   |,  .-.  .-.  ,|\n");
            printf("   | )(_o/  \\o_)( |\n");
            printf("   |/     /\\     \\|\n");
            printf("   (_     ^^     _)\n");
            printf("    \\__|IIIIII|__/\n");
            printf("     | \\IIIIII/ |\n");
            printf("     \\          /\n");
            printf("      `--------`\n");
            printf("     💀 DERROTA 💀\n\n");
            break;
        }
        else if (estado == 1) {
            printf("\n");
            printf("   🏆🏆🏆🏆🏆🏆🏆🏆🏆\n");
            printf("   🏆               🏆\n");
            printf("   🏆  ¡VICTORIA!   🏆\n");
            printf("   🏆 Todos los     🏆\n");
            printf("   🏆 paises están  🏆\n");
            printf("   🏆 estables!     🏆\n");
            printf("   🏆               🏆\n");
            printf("   🏆🏆🏆🏆🏆🏆🏆🏆🏆\n");
            printf("\n");
            break;
        } 

    }
    

    //  Limpieza
    liberarMapa(latam);
    liberarTabla(tabla);
    printf("\n✅ Fin del juego. Gracias por jugar.\n");
    return 0;
}



