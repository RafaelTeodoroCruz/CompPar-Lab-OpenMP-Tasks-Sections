
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>
#include <math.h>

// Rafael Teodoro Cruz - RA: 10723258
// Atividade 1 - Analise Estatistica com Sections

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

    printf("Calculando metricas (versao sections)...\n");

    #pragma omp parallel sections reduction(min:min_val) reduction(max:max_val) reduction(+:sum,sum_sq)
    {
        #pragma omp section
        {
            for (long long j = 0; j < N; j++) {
                if (data[j] < min_val) {
                    min_val = data[j];
                }
            }
        }

        #pragma omp section
        {
            for (long long j = 0; j < N; j++) {
                if (data[j] > max_val) {
                    max_val = data[j];
                }
            }
        }

        #pragma omp section
        {
            for (long long j = 0; j < N; j++) {
                sum += data[j];
                sum_sq += (long long)data[j] * data[j];
            }
        }
    }

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
