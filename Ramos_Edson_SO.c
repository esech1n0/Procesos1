#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

int main(int argc, char *argv[]) {
int num;
int num_hijos=3;
pid_t pid_hijo, pid_nieto;

srand(time(NULL)); // Inicio de randoms

printf("Soy el proceso Padre con PID %d.\n", getpid());
for (num = 0; num < num_hijos ; num++) { //crea los 3 hijos
    pid_hijo = fork(); // Crea un nuevo proceso hijo
    if (pid_hijo < 0) { // Error en fork
        perror("Error al crear el proceso");
        exit(EXIT_FAILURE);
    }
    if(pid_hijo == 0){ //codigo del hijo, el fork retorna un 0 si existe un proceso hijo
        printf("Soy el proceso HIJO con PID %d, mi padre es %d\n",getpid(),getppid());
        usleep((rand() % 500) * 1000); //espera tiempo, random time

        for(num = 0; num < 2; num++) { //crea los 2 nietos de cada hijo
            pid_nieto = fork(); //creamos fork para nieto

            if (pid_nieto < 0) { // Error en fork nieto
                perror("Error al crear el proceso");
                exit(EXIT_FAILURE);
            }

            if(pid_nieto == 0){ //codigo de los nietos
                printf("Soy el proceso Nieto con PID %d, mi padre es %d\n",getpid(),getppid());
                usleep((rand() % 500) * 1000); //espera tiempo, random time
                exit(0); //el nieto termina aqui
                }
            }
            //el hijo espera a sus 2 hijos
            for (num = 0; num < 2; num++) {
                pid_nieto = wait(NULL);
                printf("Fin del proceso nieto con PID %d \n", pid_nieto);
            }
            exit(0); //el hijo termina aqui
            }

        }
        //el padre esperar a todos los hijos
        for (num = 0; num < num_hijos; num++) {
            pid_hijo = wait(NULL);
            printf("Fin del proceso hijo con PID %d\n",pid_hijo);
        }
        // aquí el padre se elimina al final
        printf("El proceso padre con PID %d eliminado \n", getpid());
        kill(getpid(), SIGKILL);

        return 0;
}
