#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define N_BARR 0

struct comm_info 
{
    MPI_Comm       communicator;   /*   通信子           */
    MPI_Datatype   s_data_type;    /*   发送数据类型     */
    MPI_Datatype   r_data_type;    /*   接收数据类型     */
    int            rank;           /*   虚拟进程号       */
    void*          s_buffer;       /*   发送缓冲区       */
    size_t         s_alloc;        /*   发送缓冲区大小   */
    void*          r_buffer;       /*   接收缓冲区       */
    size_t         r_alloc;        /*   接收缓冲区大小   */
};

struct iter_schedule
{
    int      n_sample;       /*   采样次数   */
    int      s_cache_iter;   /*   可以忽略   */
    size_t   s_offs;         /*   可以忽略   */
};

void IMB_bcast(struct comm_info* c_info, int size, struct iter_schedule* ITERATIONS, double* time) 
{
    int ierr;
    double t1, t2;
    int    i,root;
    /*Type_Size s_size;*/
    int s_num;
    void* bc_buf;

    ierr = 0;

    if(c_info->rank!=-1)
    {
        /* 广播根进程 */
        root=0;
        /*   广播数据大小   */
        s_num = size;
        for(i=0; i<N_BARR; i++) 
            MPI_Barrier(c_info->communicator);

        t1 = MPI_Wtime();
        /* 运行广播函数 n_sample 次 */
        for(i=0; i< ITERATIONS->n_sample; i++)
        {
            /* 非根节点设定接收缓冲区，根节点设定发送缓冲区 */
            // 每个进程的rank是不一样的
            bc_buf = (root == c_info->rank) ? c_info->s_buffer : c_info->r_buffer;
            /* printf("rank is: %d\n", c_info->rank); */
            /* printf("root is: %d\n", root); */
            /* fflush(stdout); */

            ierr= MPI_Bcast((char*)bc_buf,
                                    s_num,                   
                                    c_info->s_data_type,
                                    root,
                                    c_info->communicator);
#if defined(IMB_DEBUG_EAGER) || defined(IMB_DEBUG_EAGERSHORT)
            printf("[%d]: %s\n", c_info->rank, bc_buf);
#endif
        }

        t2 = MPI_Wtime();
        /* printf("t1 is: %lf\t t2 is: %lf\t t2-t1 is: %lf\n", t1, t2, t2-t1); */
        *time=(t2 - t1)/(ITERATIONS->n_sample);
    }
    else
    {
        *time = 0.;
    }
}

#define SND_BUFFER_SIZE 1024
#define RCV_BUFFER_SIZE 1024

void IMB_init(struct comm_info* c_info, int size, struct iter_schedule* ITERATIONS, int msg_sz) 
{
    /*c_info->rank = 0;*/
    c_info->communicator = MPI_COMM_WORLD;
    c_info->s_data_type = c_info->r_data_type = MPI_CHAR;

    c_info->s_alloc = c_info->r_alloc = msg_sz+4;
    /* 进程不可能同时处于发送和接收状态，因此此处可以将优化缓冲区分配，
     * 只分配一块缓冲区即可 */
    c_info->s_buffer = malloc(c_info->s_alloc);
#ifdef IMB_DEBUG_EAGERSHORT
    memcpy(c_info->s_buffer, "hello world", strlen("hello world")+1);
#endif
#ifdef IMB_DEBUG_EAGER
    memcpy(c_info->s_buffer, "too young too simple, sometimes naive", strlen("too young too simple, sometimes naive")+1);
#endif
    c_info->r_buffer = malloc(c_info->r_alloc);
    
}

void IMB_finalize(struct comm_info* c_info, int size, struct iter_schedule* ITERATIONS, int msg_sz) 
{
    // free memory alloced on heap
}

int main(int argc, char *argv[])
{
    int proc_nums;
    int my_rank;

    struct comm_info C_INFO;
    struct iter_schedule iter_sdu;
    int msg_sz;
    int sample_times;
    double time;

    if (argc < 2)
        printf("Usage: %s MsgSize [SampleTimes]\n");

    if (argc == 2) 
    {
        msg_sz = atoi(argv[1]);
        assert(msg_sz <= 1024*64 && msg_sz >= 0);
    }
    else if (argc == 3)
    {
        msg_sz = atoi(argv[1]);
        assert(msg_sz <= 1024*64 && msg_sz >= 0);
        sample_times = atoi(argv[2]);
        assert(sample_times <= 10 && sample_times > 0);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_nums);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    /* 广播测试的采样次数 */
    iter_sdu.n_sample = sample_times;
    iter_sdu.s_cache_iter = iter_sdu.s_offs = 0;
    C_INFO.rank = my_rank;

    IMB_init(&C_INFO, 0, &iter_sdu, msg_sz);
    // 不要进行过barrier操作，否则会导致程序无法正常终止，索性影响不大
    // MPI_Barrier(MPI_COMM_WORLD);
    // do any stuff here
    IMB_bcast(&C_INFO, msg_sz, &iter_sdu, &time);
    printf("Msg size: %d\t Latancy: %lf\n", msg_sz, time);

    IMB_finalize(&C_INFO, 0, &iter_sdu, msg_sz);

    // 注意，目前不能调用该操作，因为里面会用到bcast操作，可能会失败
    // MPI_Finalize();

    return 0;
}
