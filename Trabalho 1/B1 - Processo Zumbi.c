/* --------------------------------------------------------------
   Pequena mini-aplicação para demonstrar sinais e processos
   zumbi: três sinais (qualquer combinação de SIGUSR1 / SIGUSR2)
   guiam o fluxo — cria zumbi, recolhe zumbi, encerra.          */

/* Garante disponibilidade de sigaction com campos modernos     */
#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

/* Handler minimalista: não faz nada além de acordar pause()    */
static void catcher(int signo)
{
    (void)signo;            /* evitamos warning de parâmetro não usado */
}

int main(void)
{
    /* Instala o mesmo tratador para SIGUSR1 e SIGUSR2.
       SA_RESTART faz com que chamadas bloqueantes (pause, wait…)
       sejam retomadas automaticamente após o sinal.             */
    struct sigaction sa = {
        .sa_handler = catcher,
        .sa_flags   = SA_RESTART
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    /* 1º sinal: apenas acorda o pause e segue o fluxo           */
    pause();

    /* Cria o filho que termina imediatamente; pai não faz wait,
       portanto o filho vira zumbi até receber o 2º sinal.       */
    if (fork() == 0)
        _exit(0);           /* termina filho sem flushing de stdio   */

    /* 2º sinal: acorda e permite que o pai faça wait()           */
    pause();

    /* Recolhe o processo zumbi. Se for interrompido por outro
       sinal, repete até completar com sucesso.                  */
    while (wait(NULL) == -1 && errno == EINTR)
        ;

    /* 3º sinal: última pausa; ao acordar, encerra normalmente    */
    pause();
    return 0;
}

