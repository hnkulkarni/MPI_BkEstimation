/*
 ============================================================================
 Name        : ParBkground.c
 Author      : Hrushikesh N. Kulkarni
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
 */
#include <math.h> 

#include <iostream>

#include "Includes.h"

#include "CWork.h" //Does All the Work.


using namespace std;

int main(int argc, char *argv[])
{
	int rank, size;

	cout << "Given Arguments " <<  argc << endl;

	cout << argv[1] << endl;
	cout << argv[2] << endl;
	cout << argv[3] << endl;



	string arguments;

	// Extract Mode
	arguments = argv[1];

	MPI::Init(argc, argv);
	size = MPI::COMM_WORLD.Get_size();
	rank = MPI::COMM_WORLD.Get_rank();

	CWork oWork;

	oWork.iTotalImages = atoi( argv[3] );

	if(rank == ROOT)
	{
		if( !arguments.compare("file") )
		{
			//Extract DirName
			arguments = argv[2];

			oWork.ReadFiles( arguments , atoi( argv[3] ));
		}

	}

	oWork.BroadCastParams();

	MPI::COMM_WORLD.Barrier();

	oWork.AllocateSortVector();


	MPI::COMM_WORLD.Barrier();


	oWork.FindBackground();


	if(rank == ROOT)
	{
		oWork.WriteData( arguments );

	}
	MPI::Finalize();
	return 0;
}

