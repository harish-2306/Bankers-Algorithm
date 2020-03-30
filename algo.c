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
pthread_mutex_t mutex;

//Function Declrations
void printAvailVector();
void printAllocMatrix();
void printMaxMatrix();
void printNeedMatrix();
void printAll();
bool isSafe();

//main function
int main()
{
    printf("Enter number of unique processes and resources: \n");
    printf("Processes: ");
    scanf("%d", &noofProcesses);
    printf("Resources: ");
    scanf("%d", &noofResources);

    //getting entries for Available Resources
    printf("\nEnter the number of available entities of each resource: \n");
    for(int i=0; i<noofResources; i++) {
        printf("Resource %d: ", i+1);
        scanf("%d", &availVector[i]);
    }

    //getting entries for Maximum Matrix
    printf("\n\nEnter the number of resources of each type MAXIMUM NEEDED for each process: \n");
    for(int i=0; i<noofProcesses; i++){
        printf("Process %d: \n", i+1);
        for(int j=0; j<noofResources; j++) {
            printf("Resource %d: ", j+1);
            scanf("%d", &maxMatrix[i][j]);
        }
        printf("\n");
    }

    //getting entries for Allocated Matrix
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

    //printing everything
    printf("AVALILABLE:\n");
    print1d(availVector);
    printf("\n\nmaxMatrix:\n");
    print2d(maxMatrix);
    printf("\n\n\nallocatedMatrix:\n");
    print2d(allocatedMatrix);
    printf("\n\n\nneedMatrix\n");
    print2d(needMatrix);

    //checking if initial state is safe
    if(isSafe(availVector, maxMatrix, allocatedMatrix, needMatrix))
        printf("\n\nThe given input IS SAFE\n");
    else
        printf("\n\nThe given input IS NOT SAFE\n");

    return 0;
}


//Function Definition

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
        printf(" }\n");
    }
}

//Printing Maximum Matrix
void printMaxMatrix() {

    printf("Maximum Matrix : \n");
    for(int i = 0;i < noofProcesses;i++) {
        printf("{ ");
        for(int j = 0;j < noofResources;j++)
            printf("%d, ", maxMatrix[i][j]);
        printf(" }\n");
    }
}

//Printing Need Matrix
void printNeedMatrix() {

    printf("Need Matrix : \n");
    for(int i = 0;i < noofProcesses;i++) {
        printf("{ ");
        for(int j = 0;j < noofResources;j++)
            printf("%d, ", needMatrix[i][j]);
        printf(" }\n");
    }
}

//Printing All Data Structres
void printAll() {

    printAvailVector();
    printAllocMatrix();
    printMaxMatrix();
    printNeedMatrix();
    
}

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