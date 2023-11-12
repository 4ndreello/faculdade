#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * @authors
 * Gabriel M. G. Andreello (202210183)
 * Henrique Scudeller de Oliveira (202210324)
 */

#define MATRIX_SIZE 3

typedef struct
{
   int matriz[MATRIX_SIZE][MATRIX_SIZE];
   int resultado;
} SharedData;

int detSubmatriz(int mat[MATRIX_SIZE][MATRIX_SIZE], int i, int j)
{
   return mat[i][j] * mat[(i + 1) % MATRIX_SIZE][(j + 1) % MATRIX_SIZE] - mat[i][(j + 1) % MATRIX_SIZE] * mat[(i + 1) % MATRIX_SIZE][j];
}

void *threadFunction(void *arg)
{
   SharedData *data = (SharedData *)arg;

   int x = 0, y = 0;
   for (int i = 0; i < MATRIX_SIZE; i++)
   {
      x = x + data->matriz[0][i] * data->matriz[1][(i + 1) % MATRIX_SIZE] * data->matriz[2][(i + 2) % MATRIX_SIZE];
      y = y + data->matriz[2][i] * data->matriz[1][(i + 1) % MATRIX_SIZE] * data->matriz[0][(i + 2) % MATRIX_SIZE];
   }

   data->resultado = (x - y);
}

int main()
{
   SharedData data;

   FILE *arquivo = fopen("matriz.txt", "r");
   if (arquivo == NULL)
   {
      perror("Erro ao abrir o arquivo");
      exit(EXIT_FAILURE);
   }

   for (int i = 0; i < MATRIX_SIZE; i++)
      for (int j = 0; j < MATRIX_SIZE; j++)
         fscanf(arquivo, "%d", &data.matriz[i][j]);

   fclose(arquivo);

   data.resultado = 0;

   pthread_t thread;

   if (pthread_create(&thread, NULL, threadFunction, (void *)&data) != 0)
   {
      printf("Erro ao criar thread");
      exit(EXIT_FAILURE);
   }

   if (pthread_join(thread, NULL) != 0)
   {
      printf("Erro ao aguardar thread");
      exit(EXIT_FAILURE);
   }

   printf("O determinante da matriz é: %d\n", data.resultado);

   return 0;
}
