#include <iostream>
#include <pthread.h>

using namespace std;

const int tamanho = 3;
const int MAX_THREADS = 3;
int matrizA[tamanho][tamanho] = {{0, 8, 6}, {1, 9, 15}, {13, 25, 6}};
int matrizB[tamanho][tamanho] = {{9, 17, 20}, {32, 11, 6}, {10, 20, 30}};
pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;

struct ThreadData{
    int linhaInicio;
    int linhaFim;
    float **resultado;
};


void multiplicacaoPosicional(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->linhaInicio; i < data->linhaFim; i++) {
        for (int j = 0; j < tamanho; j++) {
            pthread_mutex_lock(&exclusao_mutua);
                data->resultado[i][j] = matrizA[i][j] * matrizB[i][j];
            pthread_mutex_unlock(&exclusao_mutua);
        }
    }
}

int main() {

    float **resultado = new float *[tamanho];

    for (int i = 0; i < tamanho; ++i) {
        resultado[i] = new float[tamanho];
    }
    ThreadData dados[MAX_THREADS];

    pthread_t threads[MAX_THREADS];
    int linhasPorThread = tamanho / MAX_THREADS;
    int linhaInicio = 0;
    int linhaFim = linhasPorThread;



    for(int i = 0; i < MAX_THREADS; i++){
        dados[i].linhaInicio = linhaInicio;
        dados[i].linhaFim = linhaFim;
        dados[i].resultado = resultado;
        pthread_create(&threads[i], NULL, (void *(*)(void *))multiplicacaoPosicional,&dados[i]);
        linhaInicio = linhaFim;
        linhaFim += linhasPorThread;
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    cout << "Resultado da multiplicação posicional:" << endl;
    for (int i = 0; i < tamanho; ++i) {
        for (int j = 0; j < tamanho; ++j) {
            cout << resultado[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
