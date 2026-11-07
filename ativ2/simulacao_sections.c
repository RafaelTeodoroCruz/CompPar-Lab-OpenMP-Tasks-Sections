
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Rafael Teodoro Cruz - RA: 10723258
// Atividade 2 - Simulacao de Particulas com Sections

#define NUM_PARTICLES (1024 * 1024 * 10)
#define DT 0.01

typedef struct {
    double x, y, z;
    double vx, vy, vz;
} Particle;

int main() {
    Particle *particles = malloc(NUM_PARTICLES * sizeof(Particle));
    double *old_vy = malloc(NUM_PARTICLES * sizeof(double));

    if (!particles || !old_vy) {
        perror("malloc falhou");
        free(particles);
        free(old_vy);
        return 1;
    }

    printf("Inicializando particulas...\n");

    #pragma omp parallel for
    for (long long i = 0; i < NUM_PARTICLES; i++) {
        particles[i].x = (i % 100) * 1.0;
        particles[i].y = (i % 200) * 1.0;
        particles[i].z = (i % 300) * 1.0;
        particles[i].vx = (i % 10) * 0.5;
        particles[i].vy = (i % 10) * 0.5;
        particles[i].vz = (i % 10) * 0.5;
        old_vy[i] = particles[i].vy;
    }

    printf("Particulas inicializadas.\n");

    double start_time = omp_get_wtime();
    const double GRAVITY = -9.81;

    printf("Simulando um passo (versao sections)...\n");

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for (long long i = 0; i < NUM_PARTICLES; i++) {
                particles[i].x += particles[i].vx * DT;
                particles[i].y += old_vy[i] * DT;
                particles[i].z += particles[i].vz * DT;
            }
        }

        #pragma omp section
        {
            for (long long i = 0; i < NUM_PARTICLES; i++) {
                particles[i].vy += GRAVITY * DT;
            }
        }
    }

    double end_time = omp_get_wtime();

    printf("\nSimulacao concluida.\n");
    printf("Particula 0: Pos(%.2f, %.2f, %.2f) Vel(%.2f, %.2f, %.2f)\n",
           particles[0].x, particles[0].y, particles[0].z,
           particles[0].vx, particles[0].vy, particles[0].vz);
    printf("Tempo de execucao (simulacao): %.4f segundos\n",
           end_time - start_time);

    free(old_vy);
    free(particles);
    return 0;
}
