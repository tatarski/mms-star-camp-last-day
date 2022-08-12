#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<wait.h>
typedef struct timeval timeval;
// gcc timer.c -o timer
// ./timer arg1 arg2 arg3 arg4 arg5 arg6 NULL
// ./timer cat file1 NULL
// ./timer ./slow_program NULL
int main(int argc, char *argp[]) {
    timeval tv1,tv2;
    gettimeofday(&tv1, NULL);
    printf("%lu %lu\n", tv1.tv_sec, tv1.tv_usec);

    int pid = fork();
    if(pid < 0) {
        perror("Fork failed\n");
        exit(-1);
    } else if(pid == 0) {
        // Child process

        // execv(argp[1], argp+1);
        execvp(argp[1], argp + 1);
        // Exec failed
        perror("Exec failed\n");
        exit(-1);
    } else {
        // Parent process
        int status;
        wait(&status);
        if(WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Child process has ended successfully\n");
        } else {
            printf("Child process has not ended successfully\n");
        }
        gettimeofday(&tv2, NULL);
        printf("%lu %lu\n", tv2.tv_sec, tv2.tv_usec);
        printf("Elapsed time: %lf\n", ((tv2.tv_sec - tv1.tv_sec)*1000000 + 
                                    (tv2.tv_usec - tv1.tv_usec))/1000000.);
        exit(0);
    }

    return 0;
}