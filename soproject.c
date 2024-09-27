#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000 // Tamanho do vetor
#define MAX_VALUE 20000 // Valor máximo dos números aleatórios

int vetor[N];

// Estrutura para passar parâmetros para as threads
typedef struct {
    int inicio;
    int fim;
} ThreadData;

// Função para encontrar o maior elemento em uma parte do vetor
void* encontrar_maior(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int maior_local = vetor[data->inicio];

    for (int i = data->inicio + 1; i < data->fim; i++) {
        if (vetor[i] > maior_local) {
            maior_local = vetor[i];
        }
    }

    // Return the local maximum value found
    int* resultado = malloc(sizeof(int));
    *resultado = maior_local;
    pthread_exit((void*)resultado);
}

int main() {
    srand(time(NULL));

    // Inicialização do vetor com valores randômicos entre 1 e 20000
    for (int i = 0; i < N; i++) {
        vetor[i] = rand() % MAX_VALUE + 1; // Valores entre 1 e 20000
    }

    // Imprimir todos os elementos do vetor
    printf("Todos os elementos do vetor:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");

    pthread_t thread1, thread2;
    ThreadData data1 = {0, N / 2};  // Primeira metade do vetor
    ThreadData data2 = {N / 2, N};  // Segunda metade do vetor

    // Criar as threads
    pthread_create(&thread1, NULL, encontrar_maior, &data1);
    pthread_create(&thread2, NULL, encontrar_maior, &data2);

    // Variáveis para armazenar o resultado de cada thread
    int* resultado1;
    int* resultado2;

    // Aguardar a finalização das threads e obter os resultados
    pthread_join(thread1, (void**)&resultado1);
    pthread_join(thread2, (void**)&resultado2);

    // Encontrar o maior elemento entre os dois encontrados
    int maior_global = *resultado1 > *resultado2 ? *resultado1 : *resultado2;

    printf("O maior elemento do vetor é: %d\n", maior_global);

    // Liberar a memória alocada
    free(resultado1);
    free(resultado2);

    return 0;
}
