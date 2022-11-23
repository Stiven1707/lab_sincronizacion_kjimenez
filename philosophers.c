/**
 * @file 
 * @author Kevin Stiven Jimenez Perafan (kjimenez@univirtual.edu.co)
 * @brief Implementacion del problema filosofos
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
/**
 * @brief diferentes estados que atraviezan los filosofos
 */
typedef enum
{
    THINKING,
    HUNGRY,
    EATING
} philosopher_state;
// mutex
semaphore mutex;
// Arreglo de semaforos
semaphore *s;
// Arreglo de id de filosofos
int *ids;
// Arreglo de ID de hilos
pthread_t *t;
// Arreglo de estados de los filosofos
philosopher_state *state;
// Cantidad de filosofos
int n;
/**
 * @brief Hilo de filosofo
 *
 * @param arg Id del filosofo (int*)
 * @return NULL
 */
void *philosopher(void *arg);
/**
 * @brief Indica que el filosofo esta pensando
 *
 * @param i Posicion del filosofo en la mesa
 */
void think(int i);
/**
 * @brief Trata de tomar los tenedores
 *
 * @param i Posicion del filosofo en la mesa
 * @note El hilo se bloquea si alguno de sus vecinos esta usando los tenedores
 * @note Si ninguno de lso vecinos esta usando los tenedores, no se bloquea
 */
void take_forks(int i);
/**
 * @brief Indica que el filosofo esta comiendo
 *
 * @param i Posicion del filosofo en la mesa
 */
void eat(int i);
/**
 * @brief Indica que el filosofo dejo de comer
 *
 * @param i Posicion del filosofo en la mesa
 * @note El filosofp desbloquea a sus vecinos si se encuentra bloqueados
 */
void put_forks(int i);
/**
 * @brief Realiza la transicion entre hambriento
 * y comiendo si se puede
 *
 * @param i Posicion del filosofo en la mesa
 */
void test(int i);

int main(int argc, char *argv[])
{

    // Contador para los for
    int i;
    // Validar la cantidad de argumentos de linea de comandos
    n = 0;
    if (argc != 2)
    {
        fprintf(stderr, "Debe especificar la cantidad de filosofos\n");
        exit(EXIT_FAILURE);
    }
    // Obtener n de la lista de comandos
    n = atoi(argv[1]);
    if (n <= 2)
    {
        fprintf(stderr, "Debe especificar la cantidad de filosofos mayor que 2\n");
        exit(EXIT_FAILURE);
    }
    // POST: n > 2
    // Reservar memoria para el arreglo de semaforos
    s = (semaphore *)malloc(n * sizeof(semaphore));
    // Reservar memoria para los identidicadores
    t = (pthread_t *)malloc(n * sizeof(pthread_t));
    ids = (int *)malloc(n * sizeof(int));
    // Reservar memoria para los estados de los filosofos
    state = (philosopher_state *)malloc(n * sizeof(philosopher_state));
    // Inicializar los semaforos
    // Inicializar el mutex en 1
    sem_init(&mutex, 0, 1);
    // Inicializar los semaforos de los filosofos
    for (i = 0; i < n; i++)
    {
        sem_init(&s[i], 0, 0);
    }

    // Crear los hilos
    for (i = 0; i < n; i++)
    {
        ids[i] = i; // inicializamor el ID de los filosofos i
        pthread_create(&t[i], NULL, philosopher, (void *)&ids[i]);
    }
    // Dar 2 s para que los hilos se ejecuten
    sleep(2);
    // Libero la memoria que asigne dinamicamente
    free(s);
    free(t);
    free(ids);
    free(state);

    return EXIT_SUCCESS;
}
void *philosopher(void *arg)
{
    int i;
    // i = [obtener el valor *][obtener un apuntador de entero (int *)arg];
    i = *(int *)arg;
    printf("philosopher %d started\n", i);
    while (1)
    {
        think(i);
        take_forks(i);
        eat(i);
        put_forks(i);
    }

    printf("philosopher %d finished\n", i);
}

void take_forks(int i)
{
    printf("philosopher %d is hungry! taking the forks...\n", i);
    down(&mutex);
    state[i] = HUNGRY;
    test(i);
    up(&mutex);
    down(&s[i]);
}
void think(int i)
{
    printf("philosopher %d is thinking\n", i);
}

void eat(int i)
{
    printf("philosopher %d is eating\n", i);
}
void put_forks(int i)
{
    // TODO LEFT and RIGHT
    int LEFT;
    int RIGHT;
    LEFT = (i + n - 1) % n;
    RIGHT = (i + 1) % n;
    printf("philosopher %d is full! realsing the forks...\n", i);
    down(&mutex);
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    up(&mutex);
}

void test(int i)
{
    // TODO LEFT and RIGHT
    int LEFT;
    int RIGHT;

    LEFT = (i + n - 1) % n;
    RIGHT = (i + 1) % n;
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        for (int i = 0; i < n; i++)
        {
            printf("[i{%d} s{%d}] ",i,state[i]);
        }
        printf("\n");
        up(&s[i]);
    }
}