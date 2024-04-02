#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define SHM_SIZE 1024

int main() {
    const char *shm_name = "/Mi memoria compartida";

    /*
      shm_name es el nombre del segmento de memoria compartida 
      0_CREAT es la bandera que indica que el segmento de memoria compartida se
              debe creat si es que no existe, y si ya existe se ignora esta bandera
      0_RDWR indica que el segmento de memoria se está abriendo para lectura y escritura
      0666 son los permisos de acceso al segmento de memoria compartida  
    */
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Error al abrir el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Establecer el tamaño del segmento de memoria compartida
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("Error al establecer el tamaño del segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Mapear el segmento de memoria compartida en el espacio de direcciones del proceso
    char *shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("Error al mapear el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Proceso de escritura en el segmento de memoria compartida
    printf("Escribe un mensaje: ");
    fgets(shm_ptr, SHM_SIZE, stdin);

    // Esperar a que el otro proceso lea el mensaje
    while (*shm_ptr != '*') {
        sleep(1);
    }

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

    // Eliminar el segmento de memoria compartida
    if (shm_unlink(shm_name) == -1) {
        perror("Error al eliminar el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    return 0;
}


