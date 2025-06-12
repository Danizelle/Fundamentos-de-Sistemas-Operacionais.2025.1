#include <pthread.h>

/* Protótipo fornecido do professor */
int work(int id);

/* Variáveis globais exigidas */
int count = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;


struct thread_arg {
    int vezes;         
};

/* Função executada pela threads */
void *thread_func(void *arg)
{
    struct thread_arg *t = (struct thread_arg *)arg;

    for (int i = 0; i < t->vezes; ++i) {
        int id;

        /* Area critica */
        pthread_mutex_lock(&mut);
        id = count;
        ++count;
        pthread_mutex_unlock(&mut);

        /* Fora da area critica */
        work(id);
    }

    return NULL;
}
