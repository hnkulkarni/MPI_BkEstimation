/*
 * CWork.cpp
 *
 *  Created on: May 9, 2012
 *      Author: hrushikesh
 */

#include "CWork.h"

CWork::CWork() {
	// TODO Auto-generated constructor stub


	//Open Default Device.
#ifdef cv
	Camera.open(0);
#endif

}

CWork::~CWork() {
	// TODO Auto-generated destructor stub

	delete [] Medians;
}


/*
 * Captures Images from the default camera on the Laptop, saves them in the Images Folder.
 */
/*int CWork::CaptureImages(string DirPath ,  int iNumCaptures )
{


	for(int iNum = 0; iNum < iNumCaptures; iNum++ )
	{

		if(!Camera.isOpened())  // check if we succeeded
		{
			cout << "Could not Initialize Camera " << endl;
			return -1;
		}

		Camera >> Camframe; // Get a new frame from the camera

		PPMFileName.str("");

		cv::imshow("CamFeed", Camframe);

		cv::cvtColor( Camframe , CamGrayFrame , CV_BGR2GRAY );

		PPMFileName << DirPath  << "/Image" << iNum << ".ppm";

		WritePPMFile( PPMFileName.str() , CamGrayFrame );

		cout << PPMFileName.str() << endl;

		cv::waitKey(10);
	}

	return 0;
}
 */

int CWork::ReadFiles(string DirPath, int iNumReads)
{


	PPMFileName.str("");
	PPMFileName << DirPath  << "/Image" << 0 << ".ppm";

	GetSampleImageParams( PPMFileName.str() , &iPPMRows , &iPPMCols );

	StripImage.resize( iPPMCols * iPPMRows );

	int iStripSize; //Length of the strip
	iStripSize = StripImage.size();
	StripCatImage.resize( iStripSize , vector< int > ( iNumReads , 0 )   );

	for(int iNum = 0; iNum < iNumReads; iNum++ )
	{

		PPMFileName.str("");

		PPMFileName << DirPath  << "/Image" << iNum << ".ppm";

		ReadPPMFile( PPMFileName.str() , StripImage );

		CatImages( StripCatImage , StripImage , iNum );

		//DEBUG

		/*for(int iDebug = 0; iDebug < 10; iDebug++)
		{
			cout << StripCatImage[iDebug][iNum] << " " <<  StripImage[iDebug] << endl;
		}*/
	}

	return 0;

}

int CWork::CatImages(vector< vector< int > > & CatImage , vector<int> Strip , int iNumImage  )
{
	int iStripSize = StripImage.size();

	for( int iPixel = 0; iPixel < iStripSize; iPixel++  )
	{

		StripCatImage[ iPixel ][ iNumImage ] = StripImage[iPixel];
	}


	return 0;
}


int CWork::AllocateSortVector()
{
	vToSort.resize( iTotalImages );

	return 0;
}

int CWork::ScatterData(int iStartRows, int iEndRows)
{
	MPI::Status INFO;

	//MPI::COMM_WORLD.Scatter( &StripCatImage[0][0] ,  iTotalImages , MPI::INT , &vToSort[0] , iTotalImages , MPI::INT , ROOT  );

	int iDestinationRank = 0;

	if(rank == ROOT)
	{
		for(int i = iStartRows; i < iEndRows; i++ )
		{
			MPI::COMM_WORLD.Send( &StripCatImage[i][0], iTotalImages, MPI::INT , iDestinationRank , TAG );
			iDestinationRank++;

		}
	}

	MPI::COMM_WORLD.Recv(&vToSort[0] , iTotalImages ,  MPI::INT , ROOT, TAG, INFO );

	return 0;
}


int CWork::FindNthElement()
{
	int middle = (double)iTotalImages/2;

	nth_element(vToSort.begin() , vToSort.end(), vToSort.begin() + middle );

	cout << "P:" << MPI::COMM_WORLD.Get_rank() << " Value at " << middle << " ";

	iNthElement = vToSort[middle];

	cout << iNthElement << " ";

	cout << endl;

	return 0;
}

int CWork::FindBackground()
{
	int iStart = 0;
	int iEnd = 0;

	iNumProcesses = MPI::COMM_WORLD.Get_size();
	rank = MPI::COMM_WORLD.Get_rank();

	Medians = new (nothrow) int [ iNumProcesses ];

	iStart = 0;

	int iPixels = iPPMRows * iPPMCols;
	cout << "IPixels " << iPixels << endl;

	while(1)
	{
		iEnd = iStart + iNumProcesses;

		if(iEnd > iPixels)
		{
			iEnd = iPixels;
		}

		cout << "Statter from " << iStart << " to " << iEnd << endl;

		ScatterData(iStart, iEnd);

		MPI::COMM_WORLD.Barrier();

		FindNthElement();


		MPI::COMM_WORLD.Gather( &iNthElement , 1, MPI::INT, Medians , 1, MPI::INT, ROOT );

		if( rank == ROOT)
		{
			for( int i = 0; i < iNumProcesses; i++ )
			{
				imBackground.push_back( Medians[i] );
				cout << "Collected Pixels " << imBackground.size() << endl;
			}
		}


		MPI::COMM_WORLD.Barrier();

		iStart = iEnd;

		if(iStart == iPixels)
		{
			break;
		}
	}


	return 0;
}


int CWork::BroadCastParams()
{

	MPI::COMM_WORLD.Bcast( &iPPMCols, 1, MPI::INT, ROOT );
	MPI::COMM_WORLD.Bcast( &iPPMRows, 1, MPI::INT, ROOT );

	return 0;
}

int CWork::WriteData(string DirPath )
{

	PPMFileName.str("");
	PPMFileName << DirPath  << "/Background" << 0 << ".ppm";


	WriteStrip( PPMFileName.str() , imBackground, iPPMRows , iPPMCols  );

	return 0;
}
