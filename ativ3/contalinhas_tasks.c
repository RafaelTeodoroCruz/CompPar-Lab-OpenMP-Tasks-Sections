
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <omp.h>

// Rafael Teodoro Cruz - RA: 10723258
// Atividade 3 - Contagem de Linhas com OpenMP Tasks

long long total_lines = 0;

long long count_lines_in_file(const char *filepath) {
    FILE *fp = fopen(filepath, "r");

    if (!fp) {
        return 0;
    }

    long long count = 0;
    int ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    fclose(fp);
    return count;
}

void traverse_directory(const char *dirpath) {
    struct dirent **namelist;

    int n = scandir(dirpath, &namelist, NULL, alphasort);

    if (n < 0) {
        fprintf(stderr, "Erro ao escanear %s: %s\n",
                dirpath, strerror(errno));
        return;
    }

    for (int i = 0; i < n; i++) {
        if (strcmp(namelist[i]->d_name, ".") == 0 ||
            strcmp(namelist[i]->d_name, "..") == 0) {
            free(namelist[i]);
            continue;
        }

        char *fullpath = NULL;

        if (asprintf(&fullpath, "%s/%s",
                     dirpath, namelist[i]->d_name) == -1) {
            free(namelist[i]);
            continue;
        }

        struct stat path_stat;

        if (stat(fullpath, &path_stat) == 0) {
            if (S_ISDIR(path_stat.st_mode)) {
                #pragma omp task firstprivate(fullpath)
                {
                    traverse_directory(fullpath);
                    free(fullpath);
                }

                fullpath = NULL;
            } else if (S_ISREG(path_stat.st_mode)) {
                size_t len = strlen(namelist[i]->d_name);

                if (len > 2 &&
                    (strcmp(namelist[i]->d_name + len - 2, ".c") == 0 ||
                     strcmp(namelist[i]->d_name + len - 2, ".h") == 0)) {

                    #pragma omp task firstprivate(fullpath)
                    {
                        long long count =
                            count_lines_in_file(fullpath);

                        #pragma omp atomic update
                        total_lines += count;

                        free(fullpath);
                    }

                    fullpath = NULL;
                }
            }
        }

        if (fullpath != NULL) {
            free(fullpath);
        }

        free(namelist[i]);
    }

    free(namelist);
}

int main(int argc, char *argv[]) {
    const char *start_dir = ".";

    if (argc > 1) {
        start_dir = argv[1];
    }

    printf("Contando linhas em .c/.h a partir de: %s (versao tasks)\n",
           start_dir);

    total_lines = 0;
    double inicio = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            traverse_directory(start_dir);
        }
    }

    double fim = omp_get_wtime();

    printf("\n--- Resultado ---\n");
    printf("Total de linhas encontradas: %lld\n", total_lines);
    printf("Tempo de execucao: %.4f segundos\n", fim - inicio);

    return 0;
}
