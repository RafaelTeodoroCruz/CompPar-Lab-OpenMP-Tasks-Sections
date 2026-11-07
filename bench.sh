
#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Uso: $0 <executavel> [argumentos]"
    exit 1
fi

exe="$1"
shift

echo "Programa: $exe"

for t in 1 2 4 8; do
    for r in $(seq 1 10); do
        OMP_NUM_THREADS="$t" "$exe" "$@" |
        awk '/Tempo de execucao/ {
            for (i = 1; i <= NF; i++) {
                if ($i == "segundos") {
                    print $(i-1)
                }
            }
        }'
    done | awk -v t="$t" '
        {
            soma += $1
            quantidade++
        }
        END {
            if (quantidade == 10) {
                printf "threads=%d media=%.4f s (n=%d)\n",
                       t, soma/quantidade, quantidade
            } else {
                printf "ERRO: threads=%d, somente %d tempos coletados\n",
                       t, quantidade > "/dev/stderr"
                exit 1
            }
        }'
done
