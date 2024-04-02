#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define MAX_MENSAJES 10
#define MAX_LONGITUD_MENSAJE 100

typedef struct {
    int num_mensajes;
    char mensajes[MAX_MENSAJES][MAX_LONGITUD_MENSAJE];
} MessageQueue;

int main() {
    const char *shm_name = "/mi_memoria_compartida";

    // Crear el segmento de memoria compartida
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Error al abrir el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Establecer el tamaño del segmento de memoria compartida
    if (ftruncate(shm_fd, sizeof(MessageQueue)) == -1) {
        perror("Error al establecer el tamaño del segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Mapear el segmento de memoria compartida en el espacio de direcciones del proceso
    MessageQueue *message_queue = mmap(0, sizeof(MessageQueue), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (message_queue == MAP_FAILED) {
        perror("Error al mapear el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Escribir mensajes en la cola
    strncpy(message_queue->mensajes[0], "Mensaje 1", MAX_LONGITUD_MENSAJE);
    strncpy(message_queue->mensajes[1], "Mensaje 2", MAX_LONGITUD_MENSAJE);
    message_queue->num_mensajes = 2;

    // Leer mensajes de la cola
    for (int i = 0; i < message_queue->num_mensajes; ++i) {
        printf("Mensaje %d: %s\n", i + 1, message_queue->mensajes[i]);
    }

    // Desvincular el segmento de memoria compartida
    if (munmap(message_queue, sizeof(MessageQueue)) == -1) {
        perror("Error al desvincular el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Cerrar el descriptor de archivo del segmento de memoria compartida
    if (close(shm_fd) == -1) {
        perror("Error al cerrar el descriptor de archivo del segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Eliminar el segmento de memoria compartida
    if (shm_unlink(shm_name) == -1) {
        perror("Error al eliminar el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    return 0;
}
