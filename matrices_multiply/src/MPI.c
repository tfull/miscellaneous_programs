#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    int n;
    int size;
    int rank;
    float **m1;
    float **m2;
    float **m;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(argc != 3){
        fprintf(stderr, "wrong number of arguments\n");
    }

    if(rank == 0){
        struct timeval time_s;
        struct timeval time_g;
        FILE *file1;
        FILE *file2;
        int n1, n2;
        int i, j, k;

        if((file1 = fopen(argv[1], "r")) == NULL){
            fprintf(stderr, "cannot open file");
            exit(1);
        }
        if((file2 = fopen(argv[2], "r")) == NULL){
            fprintf(stderr, "cannot open file");
            exit(1);
        }

        fscanf(file1, "%d", &n1);
        fscanf(file2, "%d", &n2);

        if(n1 != n2){
            fprintf(stderr, "size mismatch");
            exit(1);
        }

        n = n1;

        for(i = 1; i < size; i++){
            MPI_Send(&n, 1, MPI_INTEGER, i, 0, MPI_COMM_WORLD);
        }

        m1 = (float**)calloc(n, sizeof(float*));
        m2 = (float**)calloc(n, sizeof(float*));
        m = (float**)calloc(n, sizeof(float*));

        for(i = 0; i < n; i++){
            m1[i] = (float*)calloc(n, sizeof(float));
            for(j = 0; j < n; j++){
                fscanf(file1, "%f", &m1[i][j]);

            }
        }

        for(i = 0; i < n; i++){
            m2[i] = (float*)calloc(n, sizeof(float));
            for(j = 0; j < n; j++){
                fscanf(file2, "%f", &m2[i][j]);
            }
        }

        for(i = 0; i < n; i++){
            m[i] = (float*)calloc(n, sizeof(float));
        }

        gettimeofday(&time_s, NULL);

        for(i = 1; i < size; i++){
            for(j = i * (n / size); j < (i + 1) * (n / size); j++){
                MPI_Send(m1[j], n, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
            }
            for(j = 0; j < n; j++){
                MPI_Send(m2[j], n, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
            }
        }

        for(i = 0; i < (n / size); i++){
            for(j = 0; j < n; j++){
                float nf = 0.0;

                for(k = 0; k < n; k++){
                    nf += m1[i][k] * m2[k][j];
                }

                m[i][j] = nf;
            }
        }

        for(i = 1; i < size; i++){
            for(j = i * (n / size); j < (i + 1) * (n / size); j++){
                MPI_Recv(m[j], n, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &status);
            }
        }

        gettimeofday(&time_g, NULL);

        int timesec = (time_g.tv_sec - time_s.tv_sec) * 1000000 + (time_g.tv_usec - time_s.tv_usec);

        fprintf(stderr, "%d.%06d [second]\n", timesec / 1000000, timesec % 1000000);

        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                fprintf(stdout, "%.2f ", m[i][j]);
            }
            fprintf(stdout, "\n");
        }

        for(i = 0; i < n; i++){
            free(m1[i]);
            free(m2[i]);
            free(m[i]);
        }
        free(m);
        free(m1);
        free(m2);

    }else{
        int i, j, k;

        MPI_Recv(&n, 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD, &status);

        m1 = (float**)calloc(n / size, sizeof(float*));
        m2 = (float**)calloc(n, sizeof(float*));
        m = (float**)calloc(n / size, sizeof(float*));

        for(i = 0; i < n / size; i++){
            m1[i] = (float*)calloc(n, sizeof(float));
            m[i] = (float*)calloc(n, sizeof(float));
        }

        for(i = 0; i < n; i++){
            m2[i] = (float*)calloc(n, sizeof(float));
        }

        for(i = 0; i < (n / size); i++){
            MPI_Recv(m1[i], n, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
        }
        for(i = 0; i < n; i++){
            MPI_Recv(m2[i], n, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
        }

        for(i = 0; i < (n / size); i++){
            for(j = 0; j < n; j++){
                float nf = 0.0;

                for(k = 0; k < n; k++){
                    nf += m1[i][k] * m2[k][j];
                }

                m[i][j] = nf;
            }
        }

        for(i = 0; i < (n / size); i++){
            MPI_Send(m[i], n, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        }

        for(i = 0; i < (n / size); i++){
            free(m1[i]);
            free(m[i]);
        }
        for(i = 0; i < n; i++){
            free(m2[i]);
        }
        free(m);
        free(m1);
        free(m2);
    }

    MPI_Finalize();

    return 0;
}
