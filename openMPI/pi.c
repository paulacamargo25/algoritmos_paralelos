#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int numRocas;
double x,y;
int i;
double contador = 0.0;
double distancia;
double pi;
int numthreads;

double monteCarloPi()
{
    #pragma omp parallel firstprivate(x, y, distancia, i) reduction(+:contador) num_threads(numthreads)
    {
        srand48((int)time(NULL) ^ omp_get_thread_num());
        for (i=0; i<numRocas; ++i)
        {
            x = (double)drand48();
            y = (double)drand48();
            distancia = sqrt((x*x)+(y*y));
            if (distancia<=1)
            {
                contador++;
            }
        }
    }
    pi = 4.0 * contador/((double) numRocas*numthreads);
    return pi;
}


int main(int argc, char* argv[])
{
    numthreads= strtol(argv[1], NULL, 10);
    numRocas= strtol(argv[2], NULL, 10);
    printf("El valor de pi es:%f \n ", monteCarloPi());
    return 0;
}

