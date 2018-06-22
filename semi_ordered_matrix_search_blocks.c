/** Commands

gcc -o semi_ordered_matrix_search -fopenmp semi_ordered_matrix_search.c
./semi_ordered_matrix_search 
export OMP_NUM_THREADS=3

**/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

//Function declaration
void search(int** matrix, int row_l, int row_r, int col_l, int col_r, int x, int* i, int* j);
void divideMatrix(int index, int *rowLow, int* rowHigh, int* colLow, int* colHigh, int p);

int main (int argc, char *argv[]) 
{
	//Number of threads
	int nthreads = atoi(argv[1]);

	//Index of the searched number in the matrix
	int i = -1, j = -1;

	//Number to be searched
	int K = 9999;

	//Input matrix size
	int N = 10000;

	//Setting the number of threads
	omp_set_num_threads(nthreads);
	
	//Generate matrix
	int** matrix = (int**)malloc(N*sizeof(int*));

	//Create a semi-ordered matrix
	for(int x = 0;x<N;x++){
		matrix[x] = (int*)malloc(N*sizeof(int));

		for(int y = 0;y<N;y++){
			matrix[x][y] = (x*(int)N)+(y+1);
		}
	}

	int rowLow[nthreads],rowHigh[nthreads],colLow[nthreads],colHigh[nthreads];
	rowLow[0] = 0;
	rowHigh[0] = N-1;
	colLow[0] = 0;
	colHigh[0] = N-1;

	double divideTime = -omp_get_wtime();

	//Divide the matrix blocks among threads
	divideMatrix(0,rowLow,rowHigh,colLow,colHigh,nthreads);
	
	divideTime += omp_get_wtime();

	double start_time = omp_get_wtime();

	/* Fork a team of threads giving them their own copies of variables */
	#pragma omp parallel
	{	
		int threadNo = omp_get_thread_num();
		// printf("%d %d %d %d\n",rowLow[threadNo], rowHigh[threadNo], colLow[threadNo], colHigh[threadNo]);
		search(matrix, rowLow[threadNo], rowHigh[threadNo], colLow[threadNo], colHigh[threadNo], K, &i, &j);
	}  /* All threads join master thread and disband */

	
	double time = omp_get_wtime() - start_time;

	printf("%d %d\n",i,j);

	printf("Time: %lf time:%lf\n", time, divideTime);

	return 0;
}


void search(int** matrix, int row_l, int row_r, int col_l, int col_r, int x, int* i, int* j)
{
	if((*i == (-1)) || (*j == (-1)))
	{
		if(row_l>row_r || col_l>col_r)
			return;
			
		int row_mid = (row_l+row_r)/2;
		int col_mid = (col_l+col_r)/2;


		int value = matrix[row_mid][col_mid];

		//If element found
		if(value==x)
		{
			*i = row_mid;
			*j = col_mid;
			return;
		}

		//Divide and conquer
		if(value>x)
		{
			search(matrix, row_l, row_mid-1, col_l, col_r, x, i,j);
			search(matrix, row_mid, row_r, col_l, col_mid-1, x,i,j);
		}
		else
		{
			search(matrix, row_l, row_r, col_mid+1, col_r, x,i,j);
			search(matrix, row_mid+1, row_r, col_l, col_mid, x,i,j);
		}
	}
}

void divideMatrix(int index, int *rowLow, int* rowHigh, int* colLow, int* colHigh, int p)
{
	// printf("index %d\n",index);
	if(p>1)
	{
		int indexHigh = index + p/2;
		if((colHigh[index] - colLow[index]) > (rowHigh[index] - rowLow[index]))
		{	
			colHigh[indexHigh] = colHigh[index];
			colHigh[index] = (colLow[index] + colHigh[index])/2;
			rowLow[indexHigh] = rowLow[index];
			rowHigh[indexHigh] = rowHigh[index];
			colLow[indexHigh] = colHigh[index]+1;
		}	
		else
		{
			rowHigh[indexHigh] = rowHigh[index];
			rowHigh[index] = (rowLow[index] + rowHigh[index])/2;
			colLow[indexHigh] = colLow[index];
			colHigh[indexHigh] = colHigh[index];
			rowLow[indexHigh] = rowHigh[index]+1;
		}
		divideMatrix(index,rowLow,rowHigh,colLow,colHigh,p/2);
		divideMatrix(indexHigh,rowLow,rowHigh,colLow,colHigh,p/2);
	}
}