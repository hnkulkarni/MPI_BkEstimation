/*
 * CPPM.cpp
 *
 *  Created on: May 9, 2012
 *      Author: hrushikesh
 *
 *      Reads and Writes the PPM Files.
 */

#include "CPPM.h"

CPPM::CPPM()
{
	// TODO Auto-generated constructor stub

}

CPPM::~CPPM()
{
	// TODO Auto-generated destructor stub
}


#ifdef CV
int CPPM:: WritePPMFile( string PPMFileName, cv::Mat Image )
{

	int iRows = Image.rows;
	int iCols = Image.cols ;

	int iData = 0;


	ToPPMFile.open( PPMFileName.c_str() );

	if( !ToPPMFile.is_open() )
	{
		cerr << "Could not Open PPM File " << PPMFileName << endl;
		return 1;
	}

	/*
	 * Write the Header
	 */
	if( ToPPMFile.good() )
	{
		ToPPMFile << "P3 " << iCols << " " << iRows  << " " << 255 << endl;
	}


	for(int i = 0; i < iRows; i++)
	{
		for(int j = 0; j < iCols; j++ )

		{

			iData = Image.at<uchar>(i,j );
			ToPPMFile << iData << " ";

			iData = Image.at<uchar>(i,j );
			ToPPMFile << iData << " ";

			iData =  Image.at<uchar>(i,j );
			ToPPMFile << iData << " ";

		}

		ToPPMFile << endl;
	}

	/*
	 * Close the Open Files
	 */
	ToPPMFile.close();



	return 0;
}

#endif

//Reads the Image File into a strip
int CPPM::ReadPPMFile(  string PPMFileName, vector<int>& ImageStrip )
{


	int iRows = 0;
	int iCols = 0;
	int iMaxVal = 0;

	int iData_B = 0;
	int iData_G = 0;
	int iData_R = 0;

	cout << "PPM File Name " << PPMFileName << endl;
	FromPPMFile.open( PPMFileName.c_str() );

	if( !FromPPMFile.is_open() )
	{
		cerr << "Could not Open PPM File " << PPMFileName << endl;
		return 1;
	}

	/*
	 * Write the Header
	 */
	string PPMMagicNumber; //Yes, they call it a magic number P3 in our case

	if( FromPPMFile.good() )
	{
		FromPPMFile >> PPMMagicNumber;
		FromPPMFile >> iCols;
		FromPPMFile >> iRows;
		FromPPMFile >> iMaxVal;

		for(int i = 0; i < iRows; i++)
		{
			for(int j = 0; j < iCols; j++ )

			{

				FromPPMFile >> iData_B;
				FromPPMFile >> iData_G;
				FromPPMFile >> iData_R;

				//Since we know that we are operating on Gray Data, we will take only say Blue component
				ImageStrip[ i *  iRows + j ] = iData_B;

			}
		}


	}


	FromPPMFile.close();

	return 0;
}

int CPPM::GetSampleImageParams(string PPMFileName , int  *iRows, int *iCols )
{

	int iMaxVal = 0;

	int iData = 0;

	cout << "PPM File Name " << PPMFileName << endl;
	cout << "Num Rows " << iRows << endl;
	cout << "Num Cols " << iCols << endl;;

	FromPPMFile.open( PPMFileName.c_str() );

	if( !FromPPMFile.is_open() )
	{
		cerr << "Could not Open PPM File " << PPMFileName << endl;
		return 1;
	}

	/*
	 * Write the Header
	 */
	string PPMMagicNumber; //Yes, they call it a magic number P3 in our case

	if( FromPPMFile.good() )
	{
		FromPPMFile >> PPMMagicNumber;
		FromPPMFile >> *iCols;
		FromPPMFile >> *iRows;
		FromPPMFile >> iMaxVal;

		cout << PPMMagicNumber << " " << *iCols << " " <<  *iRows << " " << iMaxVal << endl;

	}


	//Closing file
	FromPPMFile.close();
	return 0;
}

int CPPM::WriteStrip( string PPMFileName , vector<int> imBackgrnd , int iNumRows, int iNumCols )
{

	int iData;

	ToPPMFile.open( PPMFileName.c_str() );

		if( !ToPPMFile.is_open() )
		{
			cerr << "Could not Open PPM File " << PPMFileName << endl;
			return 1;
		}

		/*
		 * Write the Header
		 */
		if( ToPPMFile.good() )
		{
			ToPPMFile << "P3 " << iNumCols << " " << iNumRows  << " " << 255 << endl;
		}


		for(int i = 0; i < iNumRows; i++)
		{
			for(int j = 0; j < iNumCols; j++ )

			{

				iData = imBackgrnd[ i * iNumRows + j ];
				ToPPMFile << iData << " ";


				ToPPMFile << iData << " ";


				ToPPMFile << iData << " ";

			}

			ToPPMFile << endl;
		}

		/*
		 * Close the Open Files
		 */
		ToPPMFile.close();


	return 0;
}
