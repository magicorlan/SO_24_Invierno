#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define SHM_SIZE 1024

int main() {
    const char *shm_name = "/my_shared_memory";

    // Abrir el segmento de memoria compartida
    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Error al abrir el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Mapear el segmento de memoria compartida en el espacio de direcciones del proceso
    char *shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("Error al mapear el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Leer el mensaje del segmento de memoria compartida
    printf("Mensaje recibido: %s\n", shm_ptr);

    // Marcar el mensaje como leído
    *shm_ptr = '*';

    // Desvincular el segmento de memoria compartida
    if (munmap(shm_ptr, SHM_SIZE) == -1) {
        perror("Error al desvincular el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Cerrar el descriptor de archivo del segmento de memoria compartida
    if (close(shm_fd) == -1) {
        perror("Error al cerrar el descriptor de archivo del segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    return 0;
}
