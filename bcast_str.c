#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int  myid, numprocs;
    char buf[80];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    /* { */
        /* char *bcstr = "hello world"; */
        /* size_t len = strlen(bcstr); */

        /* if (myid == 0) */
            /* memcpy(buf, bcstr, len+1); */

        /* MPI_Bcast(buf, len+1, MPI_CHAR, 0, MPI_COMM_WORLD); */

        /* fprintf(stdout, "[%d]buf is: %s\n", myid, buf); */
        /* fflush(stdout); */
    /* } */

    // MPI_Barrier(MPI_COMM_WORLD);

    {
        char *bcstr = "too young too simple.";
        // 注意：一定要保证所有的进程len值相同，不然bcast肯定出错。
        size_t len = strlen(bcstr);

        if (myid == 0)
        {
            memcpy(buf, bcstr, len+1);
        }

        MPI_Bcast(buf, len+1, MPI_CHAR, 0, MPI_COMM_WORLD);
        fprintf(stdout, "[%d]buf is: %s\n", myid, buf);
        fflush(stdout);
    }

    return 0;
}
