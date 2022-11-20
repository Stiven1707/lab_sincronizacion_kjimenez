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
 * @brief Remueve el ultimo producto ingresado
 *
 */
void remove_item();
/**
 * @brief Muestra el item consumido
 *
 */
void consume_item();

int main(int argc, char *argv[])
{
    // Cantidad de elemtos maximos que se desean producir
    int n;
    n = 0;
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
    printf("n: %d\n",n);
    sem_init(&empty, 0, n);
    printf("empty: %d\n",sem_getvalue(&empty,&n));
    // Inicializar el full en 0
    sem_init(&full, 0, 0);

    // Crear los hilos
    pthread_create(&t_producer, NULL, producer, NULL);
    // Dar 5 s para que los hilos se ejecuten
    sleep(5);
    // Libero la memoria que asigne dinamicamente
    free(idprods);
    return EXIT_SUCCESS;
}

void *producer(void *arg)
{
    // TODO
    printf("Produccion %d iniciada\n", full);
    int item;
    while (1)
    {
        //item = produce_item();
        down(&empty);
        down(&mutex);
        //printf("producto %d\n",item);
        //insert_item(item);
        up(&mutex);
        up(&full);
    } 
    printf("Produccion %d terminada\n", full);
}

int produce_item()
{
    int i = sem_getvalue(&full, &i);
    return i;
}

void insert_item(int item)
{
    // TODO
}

void *consumer(void *arg)
{
    // TODO
}

void remove_item()
{
    // TODO
}

void consume_item()
{
    // TODO
}
