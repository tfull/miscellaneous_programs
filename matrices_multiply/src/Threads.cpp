#include <iostream>
#include <cstdlib>
#include <fstream>
#include <pthread.h>
#include <sys/time.h>

typedef struct{
    int size;
    float **a;
    float **b;
    float **c;
    int top;
    int bottom;
}Data;

void* thread_function(void *arg){
    Data *data = (Data*)arg;

    int n = data->size;
    float **a = data->a;
    float **b = data->b;
    float **c = data->c;
    int top = data->top;
    int bottom = data->bottom;

    for(int i = top; i < bottom; i++){
        for(int j = 0; j < n; j++){
            float nf = 0.0f;

            for(int k = 0; k < n; k++){
                nf += a[i][k] * b[k][j];
            }

            c[i][j] = nf;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]){
    int thread_size;

    if(argc != 4){
        std::cerr << "wrong number of arguments" << std::endl;
        return 1;
    }else{
        int num = std::atoi(argv[1]);

        if(num > 0 && num <= 16){
            thread_size = num;
        }else{
            std::cerr << "illegal number of threads" << std::endl;
            return 1;
        }
    }

    std::ifstream in1("data1.txt");
    std::ifstream in2("data2.txt");

    if(! in1 || ! in2){
        std::cerr << "cannot open files" << std::endl;
        return 1;
    }

    Data data;
    Data *data_ts;
    pthread_t *threads;
    struct timeval time_s, time_g;
    int n;

    {
        int n1, n2;

        in1 >> n1;
        in2 >> n2;

        if(n1 != n2){
            std::cerr << "size mismatch" << std::endl;
            return 1;
        }

        n = n1;

        data.size = n;

        data.a = new float*[n];
        data.b = new float*[n];
        data.c = new float*[n];

        for(int i = 0; i < n; i++){
            data.a[i] = new float[n];
            data.b[i] = new float[n];
            data.c[i] = new float[n];

            for(int j = 0; j < n; j++){
                in1 >> data.a[i][j];
                in2 >> data.b[i][j];
            }
        }
    }

    data_ts = new Data[thread_size];

    for(int i = 0; i < thread_size; i++){
        data_ts[i].size = data.size;
        data_ts[i].a = data.a;
        data_ts[i].b = data.b;
        data_ts[i].c = data.c;
        data_ts[i].top = i * (n / thread_size);
        data_ts[i].bottom = (i + 1) * (n / thread_size);
    }

    gettimeofday(&time_s, NULL);

    threads = new pthread_t[thread_size];

    for(int i = 0; i < thread_size; i++){
        pthread_create(&threads[i], NULL, thread_function, (void*)&data_ts[i]);
    }

    for(int i = 0; i < thread_size; i++){
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&time_g, NULL);

    long long tm = (long long)(time_g.tv_sec - time_s.tv_sec) * 1000000LL + (long long)(time_g.tv_usec - time_s.tv_usec);

    std::fprintf(stderr, "%lld.%06lld [second]\n", tm / 1000000LL, tm % 1000000LL);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            std::fprintf(stdout, "%.2f ", data.c[i][j]);
        }
        std::fprintf(stdout, "\n");
    }

    for(int i = 0; i < n; i++){
        delete data.a[i];
        delete data.b[i];
        delete data.c[i];
    }
    delete data.a;
    delete data.b;
    delete data.c;

    delete threads;
    delete data_ts;

    return 0;
}