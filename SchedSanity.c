/*
#include "types.h"
#include "stat.h"
#include "user.h"

#define mediumSized 200
#define bigSized 500


void type1() {
    int i = 0;
    int sum = 0;
    int count = 1;
    for(;i < mediumSized * 1000; i++)
    {
        sum += i * 2;
        count += sum % i;
    }
}

void type2() {
    int i = 0;
    int sum = 0;
    int count = 1;
    for(;i < bigSized * 1000; i++)
    {
        sum += i * 2;
        count += sum % i;
    }
}

void type3() {
    int i = 0;
    for(;i < mediumSized; i++)
    {
        printf(2,"mediumSize: %d\n",i);
    }
}

void type4() {
    int i = 0;
    for(;i < bigSized; i++)
    {
        printf(2,"bigSized: %d\n",i);

    }
}

int
main(int argc, char *argv[]) {
    int processesNumber = 10;
    int pid[processesNumber];
    int wtimeSum = 0;
    int rtimeSum = 0;
    int iotimeSum = 0;
    int wtime = 0;
    int rtime = 0;
    int iotime = 0;

    int i;
    for(i = 0;i < processesNumber; i++)
    {
        if((pid[i] = fork()) == 0)
        {
            //type1();
            //type2();
            type3();
            //type4();
            set_priority(i%3+1);
            exit();
        }
    }
    for(i = 0; i < processesNumber;i++)
    {
        wait2(pid[i],&wtime,&rtime,&iotime);
        wtimeSum += wtime;
        rtimeSum += rtime;
        iotimeSum+= iotime;
    }
    printf(1,"wtime avg is: %d\nrtime avg is: %d\niotime avg is:%d\n",
            wtimeSum/processesNumber,rtimeSum/processesNumber,iotimeSum/processesNumber);

    exit();
}

*/


#include "types.h"
#include "user.h"
#include "fcntl.h"

#define MEDIUM_SIZED_LOOP 10000
#define LARGE_SIZED_LOOP 100000
#define MEDIUM_SIZED_LOOP_IO 20
#define LARGE_SIZED_LOOP_IO 100

#define STD_OUT 2

int
calcOnlyTaskMedium(void)
{
    int people=0, pokemons=0;
    for(int i=0;i<MEDIUM_SIZED_LOOP;i++)
    {
        people++;
        pokemons+=2;
    }
    return (pokemons-people);
}

int
calcOnlyTaskLarge(void)
{
    int people=0, pokemons=0;
    for(int i=0;i<LARGE_SIZED_LOOP;i++)
    {
        people++;
        pokemons+=2;
    }
    return (people+pokemons);
}

void
calcOnlyTaskMediumIO(void)
{
    for(int i=0;i<MEDIUM_SIZED_LOOP_IO;i++)
    {
        printf(STD_OUT,"who runs the world? Pokemons%d!\n", i);
    }
}

void
calcOnlyTaskLargeIO(void)
{
    for(int i=0;i<LARGE_SIZED_LOOP_IO;i++)
    {
        printf(STD_OUT,"who run the world? Pokemons%d!\n", i);
    }
}

//SchedSanity
int
main(void)
{
    int numofProcs = 12;
    int wtimes[numofProcs];
    int rtimes[numofProcs];
    int iotimes[numofProcs];
    int pids[numofProcs];


    for(int i = 0;i < numofProcs;i++)
    {
        int pid;
        pid = fork();
        if(pid == 0)
        {
            switch(i%4)
            {
                case 0:
                    calcOnlyTaskMedium();
                    break;
                case 1:
                    calcOnlyTaskMediumIO();
                    break;
                case 2:
                    calcOnlyTaskLarge();
                    break;
                case 3:
                    calcOnlyTaskLargeIO();
                    break;
            }
            break;
        }
        pids[i] = pid;
    }

    for(int i = 0;i < numofProcs;i++)
    {
        int wtime,rtime,iotime;
        if(wait2(pids[i], &wtime,&rtime,&iotime)==0)
        {
            wtimes[i] = wtime;
            rtimes[i] = rtime;
            iotimes[i] = iotime;
        }
        else
        {
            exit();
        }
    }
    for(int i = 0;i < numofProcs;i++)
    {
        printf(STD_OUT,"reg-MEDIUM-IO{index: %d pid: %d, wtime: %d, rtime: %d, iotime: %d} \n",i, pids[i],wtimes[i],rtimes[i],iotimes[i]);
    }

    exit();
}


//SchedSanity
int
main2(void)
{
    int pid,wtime,rtime,iotime;
    if((pid = fork()) == 0)
    {
        calcOnlyTaskMediumIO();
    }
    wait2(pid,&wtime,&rtime,&iotime);
    if(pid!=0)
    {
        printf(STD_OUT,"reg-MEDIUM-IO{pid: %d, wtime: %d, rtime: %d, iotime: %d} \n",pid,wtime,rtime,iotime);
    }
    exit();
}
//SchedSanity
int
main3(void)
{
    int pid,wtime,rtime,iotime;
    if((pid = fork()) == 0)
    {
        //calcOnlyTaskLarge();
        printf(STD_OUT,"%d\n",calcOnlyTaskLarge());
    }
    wait2(pid,&wtime,&rtime,&iotime);
    if(pid!=0)
    {
        printf(STD_OUT,"reg-large{pid: %d, wtime: %d, rtime: %d, iotime: %d} \n",pid,wtime,rtime,iotime);
    }
    exit();
}

