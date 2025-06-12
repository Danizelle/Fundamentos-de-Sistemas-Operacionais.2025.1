#include <stdio.h>

/* Estrutura simples:
   - id   → identifica o processo
   - time → tempo restante em ms */
typedef struct {
    int id;
    int time;
} Process;

/* Simula o Round-Robin */
static void solve(Process v[], int N, int quantum)
{
    /* Caso trivial: só um processo */
    if (N == 1) {
        printf("%d (%d)\n", v[0].id, v[0].time);
        return;
    }

    int clock   = 0;        /* tempo global */
    int current = 0;        /* índice do processo em execução */
    int last    = N - 1;    /* último índice válido */

    while (last > 0) {
        int slice = v[current].time < quantum ? v[current].time : quantum;

        clock        += slice;
        v[current].time -= slice;

        if (v[current].time == 0) {      /* terminou */
            printf("%d (%d)\n", v[current].id, clock);

            /* remove o terminado puxando todo mundo à esquerda */
            for (int i = current; i < last; ++i)
                v[i] = v[i + 1];
            --last;
        } else {
            ++current;                   /* passa a vez */
        }

        if (current > last)              /* fila circular */
            current = 0;
    }

    /* imprime o último que sobrou */
    printf("%d (%d)\n", v[0].id, clock + v[0].time);
}

int main(void)
{
    int N;
    if (scanf("%d", &N) != 1 || N <= 0)
        return 0;

    int quantum;
    scanf("%d", &quantum);

    Process v[100];
    for (int i = 0; i < N; ++i) {
        scanf("%d %d", &v[i].id, &v[i].time);
        v[i].time *= 1000;              /* s → ms */
    }

    solve(v, N, quantum);
    return 0;
}

