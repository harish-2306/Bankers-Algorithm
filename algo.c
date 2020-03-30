#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>

//global variables
int noofProcesses, noofResources;
int *availVector;
int **allocatedMatrix;
int **maxMatrix;
int **needMatrix;
pthread_mutex_t lock;

//Function Declrations
void scanProc_Resc_Alloc();
void scanAvailVector();
void scanAllocMatrix();
void scanMaxMatrix();
void scanAll();
void printAvailVector();
void printAllocMatrix();
void printMaxMatrix();
void printNeedMatrix();
void printAll();
bool isSafe();
void* master();

//main function
int main()
{
    //getting everything
    scanAll();

    //printing everything
    printAll();

    //checking if initial state is safe
    if(isSafe())
        printf("\n\nThe given input IS SAFE\n");
    else
        printf("\n\nThe given input IS NOT SAFE\n");

    // pthread_mutex_init(&lock, NULL);
    // pthread_t *tid = (pthread_t *) malloc(noofProcesses*sizeof(pthread_t));
    // for(int i = 0;i < noofProcesses;i++)
    //     pthread_create((tid+i), NULL, master, &i);

    return 0;
}


//Function Definition

//The Resource Request and Resource Release happens
void* master(void* ID) {
    int reqID = *(int *) ID;
    int resourceRequestVector[noofResources];
    for(int i = 0;i < noofResources;i++) {

    }
}

//getting #process, #resources and allocating memory for data structres
void scanProc_Resc_Alloc() {

    printf("Enter number of unique processes and resources: \n");
    printf("Processes: ");
    scanf("%d", &noofProcesses);
    printf("Resources: ");
    scanf("%d", &noofResources);

    //Allocating memory for the data structres
    availVector     = (int *)  malloc(noofResources*sizeof(int));
    allocatedMatrix = (int **) malloc(noofProcesses*sizeof(int *));
    maxMatrix       = (int **) malloc(noofProcesses*sizeof(int *));
    needMatrix      = (int **) malloc(noofProcesses*sizeof(int *));
    for(int i = 0;i < noofProcesses;i++) {
        allocatedMatrix[i] = (int *) malloc(noofResources*sizeof(int));
        maxMatrix[i]       = (int *) malloc(noofResources*sizeof(int));
        needMatrix[i]      = (int *) malloc(noofResources*sizeof(int));
    }

}

//getting entries for Available Resources
void scanAvailVector() {

    printf("\nEnter the number of available entities of each resource: \n");
    for(int i=0; i<noofResources; i++) {
        printf("Resource %d: ", i+1);
        scanf("%d", &availVector[i]);
    }

}

//getting entries for Maximum Matrix
void scanMaxMatrix() {

    printf("\n\nEnter the number of resources of each type MAXIMUM NEEDED for each process: \n");
    for(int i=0; i<noofProcesses; i++){
        printf("Process %d: \n", i+1);
        for(int j=0; j<noofResources; j++) {
            printf("Resource %d: ", j+1);
            scanf("%d", &maxMatrix[i][j]);
        }
        printf("\n");
    }
    
}

//getting entries for Allocated Matrix
void scanAllocMatrix() {

    printf("\n\nEnter the number of resources of each type ALLOCATED for each process: \n");
    for(int i=0; i<noofProcesses; i++){
        printf("Process %d: \n", i+1);
        for(int j=0; j<noofResources; j++) {
            printf("Resource %d: ", j+1);
            scanf("%d", &allocatedMatrix[i][j]);
        }
        printf("\n");
    }

    //calculating Need Matrix
    for(int i=0; i<noofProcesses; i++)
        for(int j=0; j<noofResources; j++)
            needMatrix[i][j] = maxMatrix[i][j] - allocatedMatrix[i][j];

}

void scanAll() {

    scanProc_Resc_Alloc();
    scanAvailVector();
    scanMaxMatrix();
    scanAllocMatrix();

}

//Printing Available Resources Vector
void printAvailVector() {

    printf("Available Resources : { ");
    for(int i = 0;i < noofResources;i++)
        printf("%d, ", availVector[i]);
    printf("}\n");

}

//Printing Allocated Matrix
void printAllocMatrix() {

    printf("Allocated Matrix : \n");
    for(int i = 0;i < noofProcesses;i++) {
        printf("{ ");
        for(int j = 0;j < noofResources;j++)
            printf("%d, ", allocatedMatrix[i][j]);
        printf("}\n");
    }
}

//Printing Maximum Matrix
void printMaxMatrix() {

    printf("Maximum Matrix : \n");
    for(int i = 0;i < noofProcesses;i++) {
        printf("{ ");
        for(int j = 0;j < noofResources;j++)
            printf("%d, ", maxMatrix[i][j]);
        printf("}\n");
    }
}

//Printing Need Matrix
void printNeedMatrix() {

    printf("Need Matrix : \n");
    for(int i = 0;i < noofProcesses;i++) {
        printf("{ ");
        for(int j = 0;j < noofResources;j++)
            printf("%d, ", needMatrix[i][j]);
        printf("}\n");
    }
}

//Printing All Data Structres
void printAll() {

    printAvailVector();
    printAllocMatrix();
    printMaxMatrix();
    printNeedMatrix();
    
}

//Checking if the system is safe
bool isSafe()
{
	int completed[noofProcesses];
	for(int i=0; i<noofProcesses; i++)
        completed[i] = 0;

	int work[noofResources];
	for(int i = 0; i<noofResources; i++)
		work[i] = availVector[i];

	int check, sequence[noofProcesses], k=0, kprev=0;
	while(k < noofProcesses)
    {
        for(int i=0; i<noofProcesses; i++)
        {
            if (completed[i] == 0)
            {
                check = 0;
                for(int j=0; j<noofResources; j++)
                    if(needMatrix[i][j] <= work[j])
                        check++;
                if(check == noofResources)
                {
                    completed[i] = 1;
                    sequence[k] = i+1;
                    k++;
                    for(int j=0; j<noofResources; j++)
                        work[j] += allocatedMatrix[i][j];
                }
            }
        }
        if(kprev != k)
            kprev = k;
        else if(kprev == k)
            break;
    }
    if(k == noofProcesses)
    {
        // printf("\n\nAll processes can complete");
        // printf("\nThe safe sequence is:\n");
        // for(int i=0; i<k; i++)
        //     printf("Process %d ;", sequence[i]);
        return true;
    }
    else
    {
        // printf("\n\nThe processes that can complete are:\n");
        // for(int i=0; i<k; i++)
        //     printf("Process %d   ;", sequence[i]);
        // printf("\nThe remaining processes cannot complete");
        return false;
    }
}