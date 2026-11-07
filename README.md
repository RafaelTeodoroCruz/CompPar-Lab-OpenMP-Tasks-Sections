
# Laboratório de Computação Paralela — OpenMP Tasks e Sections

  Rafael Teodoro Cruz  
**RA:** 10723258   
**Disciplina:** Computação Paralela

## Descrição

Este projeto apresenta três atividades práticas de programação paralela utilizando OpenMP, com foco nas diretivas `sections` e `task`.

Foram implementadas versões iniciais e modificadas dos programas, permitindo comparar a organização do código, o desempenho e os efeitos da quantidade de threads.

## Atividade 1 — Análise Estatística de Dados

Calcula o mínimo, máximo, soma, média e desvio padrão de um vetor de inteiros.

Arquivos:
- `ativ1/analise_inicial.c`
- `ativ1/analise_sections.c`

A versão inicial utiliza múltiplos laços paralelos. A versão modificada utiliza `sections` para distribuir os cálculos estatísticos.

## Atividade 2 — Simulação de Partículas

Realiza a atualização das posições e velocidades de partículas.

Arquivos:
- `ativ2/simulacao_inicial.c`
- `ativ2/simulacao_sections.c`

A versão modificada utiliza duas seções paralelas. Foi utilizado um vetor auxiliar para preservar os valores anteriores das velocidades durante a atualização das posições.

## Atividade 3 — Contagem de Linhas

Conta as linhas de arquivos `.c` e `.h` em um diretório e seus subdiretórios.

Arquivos:
- `ativ3/contalinhas_inicial.c`
- `ativ3/contalinhas_tasks.c`

A versão modificada utiliza tarefas OpenMP para percorrer diretórios e processar arquivos, com atualização sincronizada do contador por meio de `atomic`.

## Ambiente de Execução

- GitHub Codespaces
- Ubuntu Linux
- GCC com suporte a OpenMP
- Compilação com `-O3 -fopenmp`
- Ambiente de testes com 2 CPUs lógicas disponíveis
- Aproximadamente 7,8 GiB de RAM

## Compilação

Execute os comandos a partir da pasta principal do projeto.

**Atividade 1**

```bash
gcc -O3 -fopenmp ativ1/analise_inicial.c -o ativ1/analise_inicial -lm
gcc -O3 -fopenmp ativ1/analise_sections.c -o ativ1/analise_sections -lm
```

**Atividade 2**

```bash
gcc -O3 -fopenmp ativ2/simulacao_inicial.c -o ativ2/simulacao_inicial
gcc -O3 -fopenmp ativ2/simulacao_sections.c -o ativ2/simulacao_sections
```

**Atividade 3**

```bash
gcc -O3 -fopenmp ativ3/contalinhas_inicial.c -o ativ3/contalinhas_inicial
gcc -O3 -fopenmp ativ3/contalinhas_tasks.c -o ativ3/contalinhas_tasks
```

## Execução

```bash
OMP_NUM_THREADS=2 ./ativ1/analise_inicial
OMP_NUM_THREADS=2 ./ativ1/analise_sections

OMP_NUM_THREADS=2 ./ativ2/simulacao_inicial
OMP_NUM_THREADS=2 ./ativ2/simulacao_sections

OMP_NUM_THREADS=2 ./ativ3/contalinhas_inicial .
OMP_NUM_THREADS=2 ./ativ3/contalinhas_tasks .
```

Para a Atividade 3, substitua `.` pelo diretório que deseja analisar.

## Testes de Desempenho

Os tempos foram medidos utilizando `omp_get_wtime()`.

Foram realizadas 10 execuções por configuração, utilizando 1, 2, 4 e 8 threads.

O script `bench.sh` automatiza as medições e calcula os tempos médios.

Exemplo:

```bash
chmod +x bench.sh
./bench.sh ./ativ1/analise_inicial
./bench.sh ./ativ1/analise_sections
```

Os resultados coletados são utilizados no relatório da atividade para comparar desempenho, clareza das implementações, sincronização e overhead.

