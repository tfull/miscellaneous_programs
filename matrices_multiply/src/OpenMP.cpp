#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sys/time.h>
#include <omp.h>

int main(int argc, char *argv[]){
    float **a;
    float **b;
    float **c;
    struct timeval s, t;
    int n;

    if(argc != 4){
        std::cerr << "wrong number of arguments" << std::endl;
        return 1;
    }else{
        int num = std::atoi(argv[1]);
        if(num > 0 && num <= 64){
            omp_set_num_threads(num);
        }else{
            std::cerr << "wrong number of threads" << std::endl;
            return 1;
        }
    }

    std::ifstream in1(argv[2]);
    std::ifstream in2(argv[3]);

    if(! in1 || ! in2){
        std::cerr << "cannot open files" << std::endl;
    }

    {
        int n1, n2;

        in1 >> n1;
        in2 >> n2;

        if(n1 != n2){
            std::cerr << "size mismatch" << std::endl;
            return 1;
        }

        n = n1;

        a = new float*[n];
        b = new float*[n];
        c = new float*[n];

        for(int i = 0; i < n; i++){
            a[i] = new float[n];
            b[i] = new float[n];
            c[i] = new float[n];

            for(int j = 0; j < n; j++){
                in1 >> a[i][j];
                in2 >> b[i][j];
            }
        }
    }

    gettimeofday(&s, NULL);

    #pragma omp parallel for
    for(int i = 0; i < n; i++){
        #pragma omp parallel for
        for(int j = 0; j < n; j++){
            float nf = 0.0f;

            for(int k = 0; k < n; k++){
                nf += a[i][k] * b[k][j];
            }

            c[i][j] = nf;
        }
    }

    gettimeofday(&t, NULL);

    long long tm = (long long)(t.tv_sec - s.tv_sec) * 1000000LL + (long long)(t.tv_usec - s.tv_usec);
    std::fprintf(stderr, "%lld.%06lld [second]\n", tm / 1000000LL, tm % 1000000LL);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            std::fprintf(stdout, "%.2f ", c[i][j]);
        }
        std::fprintf(stdout, "\n");
    }

    return 0;
}