/**
 * @file 
 * @author Kevin Stiven Jimenez Perafan (kjimenez@univirtual.edu.co)
 * @brief Implementacion del problema productor consumidor
 * @version 0.1
 * @date 2022-11-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <pthread.h> //al compilar se requiere -ptheread
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Alias de funciones
#define down sem_wait
#define up sem_post

/**
 * @brief Deficnicion del tipo semaphore
 */
typedef sem_t semaphore;

// mutex, garantiza la exclusion mutua
semaphore mutex;
// Cuantos elementos le falta por producir
semaphore empty;
// Cuantos elementos tiene producidos
semaphore full;
// ID de hilo producer
pthread_t t_producer;
// ID de hilo consumer
pthread_t t_consumer;
// Arreglo de id de productos
int *idprods;
// Cantidad de elemtos maximos que se desean producir
int n;
// Posicion donde se debe insertar
int pos;

/**
 * @brief Hilo del productor
 *
 * @param arg
 * @return void*
 * @note El hilo se bloquea si ya produjo la cantida maxima de productos, "empty == 0"
 * @note El hilo se bloquea si el consumidor esta sacando un producto
 * @note Desbloquea al consumidor si se encuentra bloqueado
 */
void *producer(void *arg);
/**
 * @brief Crea un nuevo producto
 *
 * @return int Identificador de ese producto
 */
int produce_item();
/**
 * @brief Se encarga de tomar el producto y almacenarlo
 * en un arreglo
 *
 * @param item Nuevo producto
 */
void insert_item(int item);
/**
 * @brief Hilo del consumidor
 *
 * @param arg
 * @return void*
 * @note El hilo se bloquea si no hay productos que consumir, "full == 0"
 * @note El hilo se bloquea si el productor esta insertando un item
 * @note Desbloquea al productor si este se encuentra bloqueado
 */
void *consumer(void *arg);
/**
 * @brief Remueve el ultimo producto ingresado y lo retorna
 * 
 * @return int elemento extraido 
 */
int remove_item();
/**
 * @brief Muestra el item consumido
 * 
 * @param item item consumido
 */
void consume_item(int item);

int main(int argc, char *argv[])
{
    n = 0;
    pos = 0;
    // Validar la cantidad de argumentos de la linea de comandos
    if (argc != 2)
    {
        fprintf(stderr, "Debe especificar la cantidad del buffer\n");
        exit(EXIT_FAILURE);
    }
    // Obtener n de la lista de comandos
    n = atoi(argv[1]);
    // Validar que la cantidad de productos max sea mayor a cero
    if (n <= 0)
    {
        fprintf(stderr, "Debe especificar la cantidad del buffer mayor que 0\n");
        exit(EXIT_FAILURE);
    }
    // POST: n > 0
    // Reservar memoria para los identidicadores de productos
    idprods = (int *)malloc(n * sizeof(int));

    // Inicializar los semaforos

    // Inicializar el mutex en 1
    sem_init(&mutex, 0, 1);
    // Inicializar el empty en N
    sem_init(&empty, 0, n);
    // Inicializar el full en 0
    sem_init(&full, 0, 0);

    // Crear los hilos
    pthread_create(&t_producer, NULL, producer, NULL);    
    pthread_create(&t_consumer, NULL, consumer, NULL);
    // Dar 5 s para que los hilos se ejecuten
    sleep(5);
    // Libero la memoria que asigne dinamicamente
    free(idprods);
    return EXIT_SUCCESS;
}

void *producer(void *arg)
{
    printf("Produccion iniciada\n");
    int item;
    while (1)
    {
        item = produce_item();
        down(&empty);
        down(&mutex);
        insert_item(item);
        up(&mutex);
        up(&full);
    }
    printf("Produccion terminada\n");
}

int produce_item()
{
    printf("Produciendo item...\n");
    usleep(rand() % 500000);
    return rand() % n;
}

void insert_item(int item)
{
    idprods[pos] = item;
    printf("Insertando item %d en la posicion %d\n", item, pos);
    pos = pos + 1;
}

void *consumer(void *arg)
{
    int item;
    while (1)
    {
        down(&full);
        down(&mutex);
        item = remove_item();
        up(&mutex);
        up(&empty);
        consume_item(item);
    }
}

int remove_item()
{
    int item = idprods[pos-1];
    printf("Removiendo item %d en la posicion %d\n", idprods[pos-1], pos-1);
    pos = pos - 1;
    return item;
}

void consume_item(int item)
{
    printf("Consumiendo item %d\n",item);
    usleep(rand() % 500000);
}
