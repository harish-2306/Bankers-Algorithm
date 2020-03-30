#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<pthread.h>
#include<unistd.h>
#include<windows.h>
#include<stdbool.h>


//global variables
int p, r;
pthread_mutex_t mutex;

//FUNCTIONS
void print1d(int a[]);
void print2d(int a[p][r]);
bool isSafe(int available[], int max[p][r], int allocated[p][r], int need[p][r]);

//main function
int main()
{
    printf("Enter number of unique processes and resources: \n");
    printf("Processes: ");
    scanf("%d", &p);
    printf("Resources: ");
    scanf("%d", &r);

    int avaliable[r];
    int allocated[p][r];
    int max[p][r];
    int need[p][r];

    //getting available resources
    printf("\nEnter the number of AVAILABLE entities of each resource: \n");
    for(int i=0; i<r; i++) {
        printf("Resource %d: ", i+1);
        scanf("%d", &avaliable[i]);
    }

    //getting entries for max
    printf("\n\nEnter the number of resources of each type MAXIMUM NEEDED for each process: \n");
    for(int i=0; i<p; i++){
        printf("Process %d: \n", i+1);
        for(int j=0; j<r; j++) {
            printf("Resource %d: ", j+1);
            scanf("%d", &max[i][j]);
        }
        printf("\n");
    }

    //getting entries for allocated
    printf("\n\nEnter the number of resources of each type ALLOCATED for each process: \n");
    for(int i=0; i<p; i++){
        printf("Process %d: \n", i+1);
        for(int j=0; j<r; j++) {
            printf("Resource %d: ", j+1);
            scanf("%d", &allocated[i][j]);
        }
        printf("\n");
    }

    //calculating need
    for(int i=0; i<p; i++)
        for(int j=0; j<r; j++)
            need[i][j] = max[i][j] - allocated[i][j];

    //printing everything
    system("cls");
    printf("AVALILABLE:\n");
    print1d(avaliable);
    printf("\n\nMAX:\n");
    print2d(max);
    printf("\n\n\nALLOCATED:\n");
    print2d(allocated);
    printf("\n\n\nNEED\n");
    print2d(need);

    //checking if initial state is safe
    if(isSafe(avaliable, max, allocated, need))
        printf("\n\nThe given input IS SAFE\n");
    else
        printf("\n\nThe given input IS NOT SAFE\n");


//    //creating threads
//	pthread_mutex_init(&mutex,NULL);
//	pthread_attr_t attrDefault;
//	pthread_attr_init(&attrDefault);
//	pthread_t *tid = (pthread_t*)malloc(sizeof(pthread_t) * p);
//	int *pid = (int*)malloc(sizeof(int)*p);//customer's ID
//
//	//initialize pid
//	for(i = 0; i<p; i++){
//		*(pid + i) = i;
//		pthread_create((tid+i), &attrDefault, customer, (pid+i));
//	}
//	//join threads
//	for(i = 0; i<p; i++){
//		pthread_join(*(tid+i),NULL);
//	}
    return 0;
}


//FUNCTION
void print1d(int a[])
{
    for(int i=0; i<r; i++)
        printf("Resource %d: %d\t", i+1, a[i]);
}

void print2d(int a[p][r])
{
    printf("\t");
    for(int i=0; i<r; i++)
        printf("Resource %d   ", i+1);
    for(int i=0; i<p; i++){
        printf("\nProcess %d: ", i+1);
        for(int j=0; j<r; j++){
                printf("%d\t\t", a[i][j]);
        }
    }
}


bool isSafe(int available[], int max[p][r], int allocated[p][r], int need[p][r])
{
	int completed[p];
	for(int i=0; i<p; i++)
        completed[i] = 0;

	int work[r];
	for(int i = 0; i<r; i++)
		work[i] = available[i];

	int check, sequence[p], k=0, kprev=0;
	while(k < p)
    {
        for(int i=0; i<p; i++)
        {
            if (completed[i] == 0)
            {
                check = 0;
                for(int j=0; j<r; j++)
                    if(need[i][j] <= work[j])
                        check++;
                if(check == r)
                {
                    completed[i] = 1;
                    sequence[k] = i+1;
                    k++;
                    for(int j=0; j<r; j++)
                        work[j] += allocated[i][j];
                }
            }
        }
        if(kprev != k)
            kprev = k;
        else if(kprev == k)
            break;
    }
    if(k == p)
    {
        printf("\n\nAll processes can complete");
        printf("\nThe safe sequence is:\n");
        for(int i=0; i<k; i++)
            printf("Process %d ;", sequence[i]);
        return true;
    }
    else
    {
        printf("\n\nThe processes that can complete are:\n");
        for(int i=0; i<k; i++)
            printf("Process %d   ;", sequence[i]);
        printf("\nThe remaining processes cannot complete");
        return false;
    }
}