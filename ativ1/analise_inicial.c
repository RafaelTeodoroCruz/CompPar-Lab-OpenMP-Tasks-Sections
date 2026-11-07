
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>
#include <math.h>

// Rafael Teodoro Cruz - RA: 10723258
// Atividade 1 - Analise Estatistica - Versao Inicial

#define N (1024 * 1024 * 100)

int main() {
    int *data = (int*) malloc(N * sizeof(int));

    if (!data) {
        perror("malloc falhou");
        return 1;
    }

    long long i;

    printf("Inicializando dados...\n");

    #pragma omp parallel for
    for (i = 0; i < N; i++) {
        data[i] = (i % 1000) - 500;
    }

    printf("Dados inicializados.\n");

    int min_val = INT_MAX;
    int max_val = INT_MIN;
    long long sum = 0;
    long long sum_sq = 0;

    double start_time, end_time;
    start_time = omp_get_wtime();

    printf("Calculando metricas (versao inicial)...\n");

    #pragma omp parallel for reduction(min:min_val)
    for (i = 0; i < N; i++) {
        if (data[i] < min_val) {
            min_val = data[i];
        }
    }

    printf("Min calculado.\n");

    #pragma omp parallel for reduction(max:max_val)
    for (i = 0; i < N; i++) {
        if (data[i] > max_val) {
            max_val = data[i];
        }
    }

    printf("Max calculado.\n");

    #pragma omp parallel for reduction(+:sum) reduction(+:sum_sq)
    for (i = 0; i < N; i++) {
        sum += data[i];
        sum_sq += (long long)data[i] * data[i];
    }

    printf("Soma e Soma Quadrados calculados.\n");

    end_time = omp_get_wtime();

    double average = (double)sum / N;
    double variance = ((double)sum_sq / N) - (average * average);
    double std_dev = sqrt(variance);

    printf("\n--- Resultados ---\n");
    printf("Min: %d\n", min_val);
    printf("Max: %d\n", max_val);
    printf("Soma: %lld\n", sum);
    printf("Media: %.2f\n", average);
    printf("Desvio Padrao: %.2f\n", std_dev);
    printf("Tempo de execucao (calculo): %.4f segundos\n",
           end_time - start_time);

    free(data);

    return 0;
}
