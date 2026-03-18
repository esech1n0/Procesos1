#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {

    int b, bb;
    pid_t pid_hijo, pid_nieto;
    pid_t bisnietos[3];
    pid_t pid_tataranieto, tataratataranietos[2];

    srand(time(NULL));

    printf(">>>>>>>>>> INICIO DE LA JERARQUÍA <<<<<<<<<<\n");
    printf("Soy el proceso PADRE (Nivel 0) con PID %d\n", getpid());
    fflush(stdout);

    pid_hijo = fork();

    if (pid_hijo == 0) {

        usleep((rand() % 500) * 1000);
        printf("Soy el proceso HIJO (Nivel 1) con PID %d, mi padre es %d\n", getpid(), getppid());
        fflush(stdout);

        pid_nieto = fork();

        if (pid_nieto == 0) {

            usleep((rand() % 500) * 1000);
            printf("Soy el proceso NIETO (Nivel 2) con PID %d, mi padre es %d\n", getpid(), getppid());
            fflush(stdout);

            for (b = 0; b < 3; b++) {

                bisnietos[b] = fork();

                if (bisnietos[b] == 0) {

                    usleep((rand() % 500) * 1000);
                    printf("Soy el proceso BISNIETO (Nivel 3) con PID %d, mi padre es %d\n", getpid(), getppid());
                    fflush(stdout);

                    if (b == 0) {

                        pid_tataranieto = fork();

                        if (pid_tataranieto == 0) {

                            usleep((rand() % 500) * 1000);
                            printf("Soy el proceso TATARANIETO (Nivel 4) con PID %d, mi padre es %d\n", getpid(), getppid());
                            fflush(stdout);

                            for (bb = 0; bb < 2; bb++) {

                                tataratataranietos[bb] = fork();

                                if (tataratataranietos[bb] == 0) {

                                    usleep((rand() % 500) * 1000);
                                    printf("Soy el proceso TATARATATARANIETO (Nivel 5) con PID %d, mi padre es %d\n", getpid(), getppid());
                                    fflush(stdout);
                                    exit(0);
                                }
                            }
                            for (bb = 0; bb < 2; bb++) {
                                waitpid(tataratataranietos[bb], NULL, 0);
                                printf("Fin del proceso TATARATATARANIETO (Nivel 5) con PID %d\n", tataratataranietos[bb]);
                                fflush(stdout);
                            }

                            exit(0);
                        }

                        waitpid(pid_tataranieto, NULL, 0);
                        printf("Fin del proceso TATARANIETO (Nivel 4) con PID %d\n", pid_tataranieto);
                        fflush(stdout);
                    }

                    exit(0);
                }
            }

            waitpid(bisnietos[0], NULL, 0);
            printf("Fin del proceso BISNIETO (Nivel 3) con PID %d\n", bisnietos[0]);
            fflush(stdout);

            for (b = 1; b < 3; b++) {
                waitpid(bisnietos[b], NULL, 0);
                printf("Fin del proceso BISNIETO (Nivel 3) con PID %d\n", bisnietos[b]);
                fflush(stdout);
            }

            exit(0);
        }

        waitpid(pid_nieto, NULL, 0);
        printf("Fin del proceso NIETO (Nivel 2) con PID %d\n", pid_nieto);
        fflush(stdout);

        exit(0);
    }

    waitpid(pid_hijo, NULL, 0);
    printf("Fin del proceso HIJO (Nivel 1) con PID %d\n", pid_hijo);
    printf("Fin del proceso PADRE (Nivel 0) con PID %d\n", getpid());
    printf(">>>>>>>>>> JERARQUÍA FINALIZADA <<<<<<<<<<\n");

    return 0;
}
