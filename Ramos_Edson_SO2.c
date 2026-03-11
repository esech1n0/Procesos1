#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

int main() {

    int h, n, b;
    pid_t pid_hijo, pid_nieto, pid_bisnieto;

    srand(time(NULL));

    printf(">>>>>>>>>> INICIO DE LA JERARQUÍA <<<<<<<<<<\n");
    printf("Soy el proceso PADRE (Nivel 1) con PID %d\n", getpid());
    fflush(stdout);

    // PADRE crea 2 HIJOS
    for (h = 0; h < 2; h++) {

        pid_hijo = fork();

        if (pid_hijo < 0) {
            perror("Error al crear hijo");
            exit(EXIT_FAILURE);
        }

        if (pid_hijo == 0) {

            // --- HIJO ---
            usleep((rand() % 500) * 1000);
            printf("Soy el proceso HIJO (Nivel 2) con PID %d, mi padre es %d\n", getpid(), getppid());
            fflush(stdout);

            // HIJO crea 3 NIETOS
            for (n = 0; n < 3; n++) {

                pid_nieto = fork();

                if (pid_nieto < 0) {
                    perror("Error al crear nieto");
                    exit(EXIT_FAILURE);
                }

                if (pid_nieto == 0) {

                    // --- NIETO ---
                    usleep((rand() % 500) * 1000);
                    printf("Soy el proceso NIETO (Nivel 3) con PID %d, mi padre es %d\n", getpid(), getppid());
                    fflush(stdout);

                    // NIETO crea 4 BISNIETOS
                    for (b = 0; b < 4; b++) {

                        pid_bisnieto = fork();

                        if (pid_bisnieto < 0) {
                            perror("Error al crear bisnieto");
                            exit(EXIT_FAILURE);
                        }

                        if (pid_bisnieto == 0) {

                            // --- BISNIETO ---
                            usleep((rand() % 500) * 1000);
                            printf("Soy el proceso BISNIETO (Nivel 4) con PID %d, mi padre es %d\n", getpid(), getppid());
                            fflush(stdout);

                            usleep(200000);
                            kill(getpid(), SIGTERM);
                            exit(0);
                        }
                    }

                    // NIETO espera a 4 bisnietos
                    for (b = 0; b < 4; b++) {
                        pid_bisnieto = wait(NULL);
                        printf("Fin del proceso BISNIETO (Nivel 4) con PID %d\n", pid_bisnieto);
                        fflush(stdout);
                        usleep((rand() % 500) * 1000);
                    }
                    kill(getpid(), SIGTERM);
                    exit(0);
                }
            }

            // HIJO espera a 3 nietos
            for (n = 0; n < 3; n++) {
                pid_nieto = wait(NULL);
                printf("Fin del proceso NIETO (3) con PID %d\n", pid_nieto);
                fflush(stdout);
                usleep((rand() % 500) * 1000);
            }
            kill(getpid(), SIGTERM);
            exit(0);
        }
    }

    // PADRE espera a 2 hijos
    for (h = 0; h < 2; h++) {
        pid_hijo = wait(NULL);
        printf("Fin del proceso HIJO (Nivel 2) con PID %d\n", pid_hijo);
        fflush(stdout);
        usleep((rand() % 500) * 1000);
    }

    printf("El proceso PADRE (Nivel 1) PID %d eliminado\n", getpid());
    printf(">>>>>>>>>> JERARQUÍA FINALIZADA <<<<<<<<<<\n");

    kill(getpid(), SIGKILL);

    return 0;
}
