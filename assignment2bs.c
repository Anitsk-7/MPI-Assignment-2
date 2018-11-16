#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define A 81

int main(int argc, char* argv[])
{
	int inArr[]={51, 34, 6, 49, 62, 77, 66, 41, 61, 2, 50, 35, 57, 71, 24, 53, 43, 54, 8, 60, 58, 56, 29, 13, 75, 3, 11, 38, 1, 65, 14, 80, 0, 27, 9, 15, 72, 28, 42, 10, 67, 36, 23, 76, 31, 18, 30, 5, 26, 44, 22, 64, 45, 52, 17, 40, 78, 37, 79, 12, 21, 20, 69, 25, 73, 55, 48, 68, 33, 19, 7, 4, 46, 74, 32, 47, 63, 59, 39, 70, 16};
	int outArr[A],*buckS,rank,size,*ccc,*disp;
  	MPI_Status status;
  	int i,j,n,inc,min,max,tmp;

  	MPI_Init(&argc,&argv);

  	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  	MPI_Comm_size(MPI_COMM_WORLD,&size);

 	if (rank == 0)
	{
    		for (i = 1; i < size; i++)
		{
      			MPI_Send(inArr,A,MPI_INT,i,1,MPI_COMM_WORLD);
    		}
  	}
  
	else 
	{
    		MPI_Recv(inArr,A,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
  	}

  	inc = 0;
  	min = rank * (A/size);
  	max = (rank + 1) * (A/size);

  	for (i = 0; i < A; i++)
	{
    		if ((inArr[i] >= min) && (inArr[i] < max))
		{
      			inc += 1;
    		}
  	}

  	buckS = malloc(inc * sizeof(int));
  	inc = 0;
  	for (i = 0; i < A; i++)
	{
    		if ((inArr[i] >= min) && (inArr[i] < max))
		{
      			buckS[inc] = inArr[i];
      			inc += 1;
    		}
  	}
  	for (i = 0; i < inc; i++)
	{
    		for (j = i+1; j < inc; j++)
		{
      			if (buckS[i] > buckS[j])
			{
        			tmp = buckS[i];
        			buckS[i] = buckS[j];
        			buckS[j] = tmp;
      			}
    		}
  	}
  	if (rank == 0)
	{
      		ccc = malloc(size * sizeof(int));
    		disp = malloc(size * sizeof(int));
  	}
  	MPI_Gather(&inc,1,MPI_INT,ccc,1,MPI_INT,0,MPI_COMM_WORLD);

  	if (rank == 0)
	{
    		disp[0] = 0;
    		for (i = 0; i < size-1; i++)
		{
      			disp[i+1] = disp[i] + ccc[i];
    		}
  	}

  	MPI_Gatherv(buckS,inc,MPI_INT,outArr,ccc,disp,MPI_INT,0,MPI_COMM_WORLD);

  	if (rank == 0)
	{
    		printf("Input Array: \n");
    		for (i = 0; i < A; i++) 
			printf("%d ",inArr[i]);
    		printf("\nSorted Output: \n");
    		for (i = 0; i < A; i++) 
			printf("%d ",outArr[i]);
  	}
	printf("\n");

  	MPI_Finalize();
  	return 0;
}
