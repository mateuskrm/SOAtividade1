#include <iostream>
#include <pthread.h>

using namespace std;

const int MAX_THREADS = 3;
const int tamanho = 3;
int matrizA[3][3] = {{0,8,6},{1,9,15}, {13,25,6}};
int matrizB[3][3] = {{9,17,20},{32,11,6}, {10,20,30}};
pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;

struct ThreadData{
    int linhaInicio;
    int linhaFim;
    int **resultado;
};


// Função para multiplicar uma parte das matrizes
void multiplicarParteMatrizes(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->linhaInicio; i < data->linhaFim; ++i) {
        for (int j = 0; j < tamanho; ++j) {
            data->resultado[i][j] = 0;
            for (int k = 0; k < tamanho; ++k) {
                pthread_mutex_lock(&exclusao_mutua);
                data->resultado[i][j] += matrizA[i][k] * matrizB[k][j];
                pthread_mutex_unlock(&exclusao_mutua);
            }
        }
    }
}



int main() {


    int **resultado = new int *[tamanho];

    for (int i = 0; i < tamanho; ++i) {
        resultado[i] = new int[tamanho];
    }
    // Multiplicar as matrizes usando threads
    pthread_t threads[MAX_THREADS];
    ThreadData threadData[MAX_THREADS];

    int linhasPorThread = tamanho / MAX_THREADS;
    int linhaInicio = 0;
    int linhaFim = linhasPorThread;

    for (int i = 0; i < MAX_THREADS; ++i) {
        threadData[i].linhaInicio = linhaInicio;
        threadData[i].linhaFim = linhaFim;
        threadData[i].resultado = resultado;
        //threads[i] = thread(multiplicarParteMatrizes, matrizA, matrizB, resultado, tamanho, linhaInicio, linhaFim);
        pthread_create(&threads[i], NULL, (void *(*)(void *))multiplicarParteMatrizes, &threadData[i]);
        linhaInicio = linhaFim;
        linhaFim += linhasPorThread;
    }

    // Aguardar todas as threads terminarem
    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Exibir o resultado
    cout << "Resultado da multiplicação:" << endl;
    for (int i = 0; i < tamanho; ++i) {
        for (int j = 0; j < tamanho; ++j) {
            cout << resultado[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
