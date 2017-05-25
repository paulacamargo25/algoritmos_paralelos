#include <iostream>

using namespace std;

__global__
void matrixSumKernel(float* A, float* B, float* C, int n)
{
        int i = threadIdx.x + (blockDim.x * blockIdx.x);
        if(i<n)
        {
                for(int j=0;j<n;j++)
                         C[i+j*n] = A[i+j*n] + B[i+j*n];

        }

}

void matrixSum(float* A, float* B, float* C, int tam)
{
        int size = (tam*tam) * sizeof(float);
        float *d_A,*d_B,*d_C;

        cudaMalloc((void**)&d_A,size);
        cudaMalloc((void**)&d_B,size);
        cudaMalloc((void**)&d_C,size);

        cudaMemcpy(d_A,A,size,cudaMemcpyHostToDevice);
        cudaMemcpy(d_B,B,size,cudaMemcpyHostToDevice);

        sumaMatrizKernel<<<ceil(tam/256.0),256>>>(d_A,d_B,d_C,tam);
        cudaMemcpy(C,d_C,size,cudaMemcpyDeviceToHost);

        cudaFree(d_A);cudaFree(d_B);cudaFree(d_C);

}

int main()
{
        int n = 100;
        float *h_A,*h_B,*h_C;
        h_A = new float[n*n];
        h_B = new float[n*n];
        h_C = new float[n*n];

        for(int i = 0; i < n; i++)
        {
                for(int j = 0; j < n; j++)
                        h_A[i*n+j] = rand() % n;
        }
        for(int i = 0; i < n; i++)
        {
                for(int j = 0; j < n; j++)
                        h_B[i*n+j] = rand() % 100;
        }
        cout<<"Los vectores generados son: "<<endl;
        for(int i = 0; i < n; i++){
                cout<<h_A[i]<<" ; ";
        }
        cout<<endl;
        for(int i = 0; i < n; i++){
          cout<<h_B[i]<<" ; ";
        }
        cout<<endl;
        matrixSum(h_A,h_B,h_C,n);
        for(int i = 0; i < n; i++){
                cout<<h_C[i]<<" ; ";
        }
        cout<<endl;
        return 0;
}

