#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int numthreads;

void countSortSecuencial(int a [], int n)
{
    int i , j , count;
    int* temp = malloc(n*sizeof(int));
    for (i = 0; i < n ; i++) {
        count = 0;
        for (j = 0; j < n; j++)
            if (a[j] < a [i])
                count++;
            else if ( a[j ] == a[i] && j < i)
                count++;
        temp [count] = a [i];
    }
    for (i = 0; i < n; i++)
        a[i] = temp[i];
    free(temp );
}

void countSortParalelo(int a[],int n)
{
    int i, j, count;
    int *temp = malloc(n*sizeof(int));

    #pragma omp parallel for shared(a, n, temp) private(i, j, count)num_threads(numthreads)
      for (i = 0; i < n; i++) {
        count = 0;
        for (j = 0; j < n; j++)
            if (a[j] < a[i])
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        temp[count] = a[i];
    }

    #pragma omp parallel for shared(a, n, temp) private(i)num_threads(numthreads)
    for (i = 0; i < n; i++)
        a[i] = temp[i];

    free(temp);
}

int compare (const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

int main(int argc, char* argv[]){

    numthreads= strtol(argv[1], NULL, 10);
    clock_t time, time2, time3;
    int n = strtol(argv[2], NULL, 10);
    //printf("num %d, n %d",numthreads,n);
    int* lista;
    int* lista2;
    int* lista3;

    lista= malloc(n*sizeof(int));
    lista2= malloc(n*sizeof(int));
    lista3= malloc(n*sizeof(int));
    for (int i = 0; i < n; i++)
    {
       int x = rand()%n;
	lista[i] = x;
	lista2[i] = x;
	lista3[i] = x;
    }
    /*for (int i = 0; i < n; i++)
    {
        printf("%d;",lista[i]);
    }
    printf("\n");
    */
    time = clock();
    countSortParalelo(lista,n);
    time = clock() - time;
    time2 = clock();
    countSortSecuencial(lista2,n);
    time2 = clock() - time2;
    time3 = clock();
    qsort (lista3, n, sizeof(int), compare);
    time3 = clock() - time3;

    printf("El Tiempo secuencial  es: %f \n",(((float)time2)/CLOCKS_PER_SEC));
    printf("El Tiempo paralelo  es: %f \n",(((float)time)/CLOCKS_PER_SEC));
    printf("El Tiempo qsort  es: %f \n",(((float)time3)/CLOCKS_PER_SEC));

    /*for (int i = 0; i < n; i++)
    {
        printf("%d;",lista[i]);
    }
    printf("\n");
*/


    //Tiempos con qsort

   /* time = clock();
    qsort (lista, n, sizeof(int), compare);
    time = clock() - time;
    cout<<"El Tiempo  es: "<<(((float)time)/CLOCKS_PER_SEC)<<endl;
    for (int i = 0; i < n; i++)
    {
        cout<<lista[i]<<" ; ";
    }
    cout<<endl;*/
    return 0;
}
