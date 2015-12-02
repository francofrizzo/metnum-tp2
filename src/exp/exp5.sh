#!/bin/bash 

LC_NUMERIC="en_US.UTF-8"

cs=0.85 #c probabilidad de teletransportación
nodos=13
tolerancia=0.00001
algoritmo=0
cantLigas=5
k="$(seq 0 1 10)"

while getopts 'ch' opt; do
  case $opt in
    l) algoritmo=$OPTARG ;;
    h) echo ""
       echo "    Experimento 2. Calcula el tiempo de ejecución cuando variamos la "
       echo "    cantidad de nodos."
       echo ""
       echo "    Opciones disponibles:"
       echo "        -c        Elimina los archivos generados por el experimento."
       echo "        -h        Imprime este texto de ayuda."
       echo ""
       exit 0 ;;
    c) if [ -d $(dirname $0)/exp4 ]; then rm $(dirname $0)/exp4 -R; fi
       exit 0 ;;
  esac
done

echo "Compilando..."

make -s -C $(dirname $0)/..
if [ $? -ne 0 ]; then
    echo "ERROR: Error de compilación."
    exit 1
fi

echo "Generando datos de entrada...";
#mkdir -p $(dirname $0)/exp5 #crear carpeta

for i in $(seq 1 $cantLigas); do
  for j in $k; do

    $(dirname $0)/../tp 0 $cs 1 $(dirname $0)/exp5/liga$i.in $tolerancia -r $(dirname $0)/exp5/exp5-liga$i-ranking-K$j.out -o $(dirname $0)/exp5/exp5-liga$i-K$j.out -1 $j
  
  done
done