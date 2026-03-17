#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {

    int b, bb;
    pid_t pid_hijo, pid_nieto, pid_bisnieto, pid_tataranieto, pid_chozno;

    srand(time(NULL));

    printf(">>>>>>>>>> INICIO DE LA JERARQUÍA <<<<<<<<<<\n");
    printf("Soy el proceso PADRE (Nivel 0) con PID %d\n", getpid());
    fflush(stdout);

    // HIJO
    pid_hijo = fork();

    if (pid_hijo == 0) {

        usleep((rand() % 500) * 1000);
        printf("Soy el proceso HIJO (Nivel 1) con PID %d, mi padre es %d\n", getpid(), getppid());
        fflush(stdout);

        // NIETO
        pid_nieto = fork();

        if (pid_nieto == 0) {

            usleep((rand() % 500) * 1000);
            printf("Soy el proceso NIETO (Nivel 2) con PID %d, mi padre es %d\n", getpid(), getppid());
            fflush(stdout);

            // 3 BISNIETOS
            for (b = 0; b < 3; b++) {

                pid_bisnieto = fork();

                if (pid_bisnieto == 0) {

                    usleep((rand() % 500) * 1000);
                    printf("Soy el proceso BISNIETO (Nivel 3) con PID %d, mi padre es %d\n", getpid(), getppid());
                    fflush(stdout);

                    // SOLO el primero crea más
                    if (b == 0) {

                        // TATARANIETO
                        pid_tataranieto = fork();

                        if (pid_tataranieto == 0) {

                            usleep((rand() % 500) * 1000);
                            printf("Soy el proceso TATARANIETO (Nivel 4) con PID %d, mi padre es %d\n", getpid(), getppid());
                            fflush(stdout);

                            // 2 CHOZNOS
                            for (bb = 0; bb < 2; bb++) {

                                pid_chozno = fork();

                                if (pid_chozno == 0) {

                                    usleep((rand() % 500) * 1000);
                                    printf("Soy el proceso CHOZNO (Nivel 5) con PID %d, mi padre es %d\n", getpid(), getppid());
                                    fflush(stdout);

                                    exit(0); // mueren primero
                                }
                            }

                            // espera CHOZNOS
                            for (bb = 0; bb < 2; bb++) {
                                pid_chozno = wait(NULL);
                                printf("Fin del proceso CHOZNO (Nivel 5) con PID %d\n", pid_chozno);
                                fflush(stdout);
                            }

                            exit(0); // luego TATARANIETO
                        }

                        wait(NULL);
                        printf("Fin del proceso TATARANIETO (Nivel 4) con PID %d\n", pid_tataranieto);
                        fflush(stdout);
                    }

                    exit(0); // luego BISNIETO
                }
            }

            // espera BISNIETOS
            for (b = 0; b < 3; b++) {
                pid_bisnieto = wait(NULL);
                printf("Fin del proceso BISNIETO (Nivel 3) con PID %d\n", pid_bisnieto);
                fflush(stdout);
            }

            exit(0); // luego NIETO
        }

        wait(NULL);
        printf("Fin del proceso NIETO (Nivel 2) con PID %d\n", pid_nieto);
        fflush(stdout);

        exit(0); // luego HIJO
    }

    wait(NULL);
    printf("Fin del proceso HIJO (Nivel 1) con PID %d\n", pid_hijo);
    printf("Fin del proceso PADRE (Nivel 0) con PID %d\n", getpid());
    printf(">>>>>>>>>> JERARQUÍA FINALIZADA <<<<<<<<<<\n");

    return 0;
}
