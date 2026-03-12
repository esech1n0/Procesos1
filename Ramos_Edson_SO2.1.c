#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

int main() {

    int b, bb;
    pid_t pid_hijo, pid_nieto, pid_bisnieto, pid_bisbisnieto, pid_bisbisbisnieto;

    srand(time(NULL));

    printf(">>>>>>>>>> INICIO DE LA JERARQUÍA <<<<<<<<<<\n");
    printf("Soy el proceso PADRE (Nivel 1) con PID %d\n", getpid());
    fflush(stdout);

    // PADRE crea 1 HIJO
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

        // HIJO crea 1 NIETO
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

            // NIETO crea 3 BISNIETOS
            for (b = 0; b < 3; b++) {

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

                    // SOLO EL PRIMER BISNIETO CREA MÁS PROCESOS
                    if (b == 0) {

                        pid_bisbisnieto = fork();

                        if (pid_bisbisnieto == 0) {

                            // --- BISBISNIETO ---
                            usleep((rand() % 500) * 1000);
                            printf("Soy el proceso BISBISNIETO (Nivel 5) con PID %d, mi padre es %d\n", getpid(), getppid());
                            fflush(stdout);

                            // BISBISNIETO crea 2 BISBISBISNIETOS
                            for (bb = 0; bb < 2; bb++) {

                                pid_bisbisbisnieto = fork();

                                if (pid_bisbisbisnieto == 0) {

                                    // --- BISBISBISNIETO ---
                                    usleep((rand() % 500) * 1000);
                                    printf("Soy el proceso BISBISBISNIETO (Nivel 6) con PID %d, mi padre es %d\n", getpid(), getppid());
                                    fflush(stdout);

                                    usleep(200000);
                                    kill(getpid(), SIGTERM);
                                    exit(0);
                                }
                            }

                            // espera a 2 bisbisbisnietos
                            for (bb = 0; bb < 2; bb++) {
                                pid_bisbisbisnieto = wait(NULL);
                                printf("Fin del proceso BISBISBISNIETO (Nivel 6) con PID %d\n", pid_bisbisbisnieto);
                                fflush(stdout);
                            }

                            kill(getpid(), SIGTERM);
                            exit(0);
                        }

                        wait(NULL);
                    }

                    usleep(200000);
                    kill(getpid(), SIGTERM);
                    exit(0);
                }
            }

            // NIETO espera a 3 bisnietos
            for (b = 0; b < 3; b++) {
                pid_bisnieto = wait(NULL);
                printf("Fin del proceso BISNIETO (Nivel 4) con PID %d\n", pid_bisnieto);
                fflush(stdout);
            }

            kill(getpid(), SIGTERM);
            exit(0);
        }

        wait(NULL);
        printf("Fin del proceso NIETO (Nivel 3) con PID %d\n", pid_nieto);
        fflush(stdout);

        kill(getpid(), SIGTERM);
        exit(0);
    }

    // PADRE espera al hijo
    wait(NULL);

    printf("Fin del proceso HIJO (Nivel 2) con PID %d\n", pid_hijo);
    printf("El proceso PADRE (Nivel 1) PID %d eliminado\n", getpid());
    printf(">>>>>>>>>> JERARQUÍA FINALIZADA <<<<<<<<<<\n");

    kill(getpid(), SIGKILL);

    return 0;
}