#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

int numeros[100] = {100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
int TAM = 100;

static pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;


void *bubbleSort(void *param)
{
    int aux;
    int* vetor = static_cast<int*>(param);
    for (int i=0; i<TAM/4; ++i){
        for (int j=i; j<TAM/4; ++j){
            if (vetor[i]>vetor[j]){
                pthread_mutex_lock(&exclusao_mutua);
                 aux = vetor[i];
                 vetor[i] = vetor[j];
                 vetor[j] = aux;
                pthread_mutex_unlock(&exclusao_mutua);
            }
        }
    }
}


int main() {
// Bubble Sort - O(n^2)
    //divide();
    int part1[25];
    int part2[25];
    int part3[25];
    int part4[25];
    int aux = 0;
    pthread_t tid_parte1, tid_parte2, tid_parte3, tid_parte4;

    //sem_init(&mutex,0,4);
    for(int i = 0; i < 25; i++){
        part1[i] = numeros[i];
    }

    pthread_create(&tid_parte1, NULL, bubbleSort, &part1);

    for(int i = 25; i < 50; i++)
    {
        part2[aux] = numeros[i];
        aux++;
    }
    aux = 0;
    pthread_create(&tid_parte2, NULL, bubbleSort, &part2);
    for(int i = 50; i < 75; i++)
    {
        part3[aux] = numeros[i];
        aux++;
    }
    aux = 0;
    pthread_create(&tid_parte3, NULL, bubbleSort, &part3);
    for(int i = 75; i < 100; i++)
    {
        part4[aux] = numeros[i];
        aux++;
    }
    pthread_create(&tid_parte4, NULL, bubbleSort, &part4);
    pthread_join(tid_parte1, NULL);
    pthread_join(tid_parte2, NULL);
    pthread_join(tid_parte3, NULL);
    pthread_join(tid_parte4, NULL);
    //sem_destroy(&mutex);

    for(int i = 0; i < 100; i++)
    {
        if (aux >= 25)
            aux = 0;

        if(i < 25)
        {
            numeros[i] = part4[aux];

        }else if(i < 50)
        {
            numeros[i] = part3[aux];
        }else if(i < 75)
        {
           numeros[i] = part2[aux];
        }else
        {
            numeros[i] = part1[aux];
        }
        aux++;
    }

    for (int i=0; i<TAM; ++i)
        cout << numeros[i] << " ";
    return 0;
}
