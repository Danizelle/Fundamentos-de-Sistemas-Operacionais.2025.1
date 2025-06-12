#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

static double diff(struct timeval a, struct timeval b)
{
    return (b.tv_sec - a.tv_sec) + (double)(b.tv_usec - a.tv_usec) / 1e6;
}

int main(void)
{
    char prog[256];
    char arg[256];
    pid_t pid = 1;
    int   status;
    struct timeval g_ini, g_fim, l_ini, l_fim;

    gettimeofday(&g_ini, NULL);

    /* lê até EOF; exige dois tokens por linha, como no gabarito */
    while (scanf("%255s %255s", prog, arg) != EOF) {

        fflush(stdout);                     /* evita duplicação após fork */
        pid = fork();
        gettimeofday(&l_ini, NULL);         /* cronômetro local começa aqui */

        if (pid == 0) {                     /* --- processo filho --- */
            execl(prog, prog, arg, (char *)NULL);

            /* só chega aqui se execl falhar */
            if (errno != 0)
                printf("> Erro: %s\n", strerror(errno));
            fflush(stdout);

            int err = errno;
            fclose(stdin);                  /* fecha stdin, como no gabarito */
            exit(err);                      /* encerra filho com errno */
        }
        else if (pid < 0) {                 /* erro no fork */
            perror("fork");
            return 1;
        }
        else {                              /* --- processo pai --- */
            waitpid(pid, &status, WUNTRACED); /* mesma flag do professor */
            gettimeofday(&l_fim, NULL);

            double elapsed = diff(l_ini, l_fim);
            int    ret     = WEXITSTATUS(status);

            printf("> Demorou %.1f segundos, retornou %d\n", elapsed, ret);
            fflush(stdout);
        }
    }

    gettimeofday(&g_fim, NULL);
    double total = diff(g_ini, g_fim);

    if (pid != 0)   /* evita mensagem duplicada caso último fork falhe */
        printf(">> O tempo total foi de %.1f segundos\n", total);

    return 0;
}
