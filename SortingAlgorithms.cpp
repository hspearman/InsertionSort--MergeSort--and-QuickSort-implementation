// Hannah Spearman

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using std::string;
using std::cout;
using std::vector;
using std::ifstream;
using std::ofstream;

// Function forward declarations
void printSortedList( vector< int >& A, string filename, string sortName, int opsCount, int cmpCount );
void InsertionSort( vector< int >& A, int end );
void MergeSort( vector< int >& A, int first, int last );
void QuickSort( vector< int >& A, int first, int last );

const int MAX_DIGITS = 10;

/*	Counters for number of operations/comparisions for each sorting function.
		- Number of operations counts all operations (addition, subtraction, assignment, comparisions, etc.).
		- Number of comparisons counts comparisons between elements of the array. */
int IS_numOfOps, IS_numOfCmp, 
	MS_numOfOps, MS_numOfCmp,
	QS_numOfOps, QS_numOfCmp = 0;

// This is the file I'm outputting the results to
ofstream output_file;

int main()
{
	output_file.open( "output_file.txt", ofstream::trunc );

	string fileNames [] = {	"10_Random.txt",
							"10_Reverse.txt",
							"10_Sorted.txt",
							"100_Random.txt",
							"100_Reverse.txt",
							"100_Sorted.txt",
							"1000_Random.txt",
							"1000_Reverse.txt",
							"1000_Sorted.txt" };

	int numOfFiles = sizeof( fileNames ) / sizeof( string );

	// For every file in the list, run each sort (insertion, merge, and quick) on its contents and print the results
	for ( int index = 0; index < numOfFiles; index++ )
	{
		vector< int > listToSort;

		// Open the file
		string filename = fileNames[ index ];
		ifstream file;
		file.open( filename );

		// Parse out each line in the file, convert it to an integer, and add it to an array
		char line [ MAX_DIGITS ];
		while ( file.getline( line, MAX_DIGITS ) )
		{
			int number = atoi( line );
			listToSort.push_back( number );
		}

		file.close();

		int numOfElements = listToSort.size();

		// Prepare a copy of the list, send it through InsertionSort, and print out the results
		vector< int > tempList( listToSort );
		InsertionSort( tempList, numOfElements - 1 );
		IS_numOfOps += IS_numOfCmp; // add the comparisons count to the final operations count
		printSortedList( tempList, filename, "Insertion Sort", IS_numOfOps, IS_numOfCmp );

		// Do the same for MergeSort
		tempList = listToSort;
		MergeSort( tempList, 0, numOfElements - 1 );
		MS_numOfOps += MS_numOfCmp; 
		printSortedList( tempList, filename, "Merge Sort", MS_numOfOps, MS_numOfCmp );

		// And the same for QuickSort
		tempList = listToSort;
		QuickSort( tempList, 0, numOfElements - 1 );
		QS_numOfOps += QS_numOfCmp;
		printSortedList( tempList, filename, "Quick Sort", QS_numOfOps, QS_numOfCmp );

		IS_numOfOps, IS_numOfCmp, 
		MS_numOfOps, MS_numOfCmp,
		QS_numOfOps, QS_numOfCmp = 0; // Resets the operation/comparison counters
	}

	output_file.close();
	cout << "Please see output_file.txt for program results.\n";
	return 0;
}

/*-------------------
	INSERTION SORT
--------------------*/

void InsertionSort( std::vector< int >& A, int end )
{
	// Cycle through every element in the array (except the 0th element)
	for ( int i = 1; i <= end; i++ )
	{
		int key = A[ i ];
		int j	= i - 1;
		IS_numOfOps += 3; // x1 subtraction, x2 assignment of key and j
		
		/*	While element j is larger than key (and we haven't exceed the array bounds yet),
			keep shifting over the jth element to make room for key */
		while ( ( j > -1 ) && ( key < A[ j ] ) )
		{
			A[ j + 1 ]	= A [ j ];
			j			= j - 1;

			IS_numOfOps += 4; // x1 addition, x1 subtraction, and x2 assignments
			IS_numOfCmp += 1; // x1 while loop comparisons
		}

		// Key >= jth element, so place key into the index after j
		A[ j + 1 ] = key;
		IS_numOfOps += 2; // x1 addition, x1 assignment
	}
}

/*-------------------
	MERGE SORT
--------------------*/

void Merge( vector< int >& A, int first, int middle, int last )
{
	int i = first;
	int j = middle + 1;
	int r = first;
	MS_numOfOps += 4; // x1 addition, x3 assignment

	vector< int > C( last + 1 );
	MS_numOfOps += 2; // x1 addition, x1 assignment

	// Compare the elements of the two segments
	while ( ( i <= middle ) && ( j <= last ) )
	{
		// If element i is smaller, place it into C's next available index 
		if ( A[ i ] <= A[ j ] )
		{
			C[ r ] = A [ i ];
			i += 1;
			r += 1;
			MS_numOfOps += 5; // x3 assignment, x2 addition
		}
		// Otherwise element j is smaller, so place it instead
		else
		{
			C[ r ] = A[ j ];
			j += 1;
			r += 1;
			MS_numOfOps += 5; // x3 assignment, x2 addition
		}

		MS_numOfCmp += 1; // x1 if statement comparison
	}

	// If we already placed all the elements of the first half into C, and elements of the second half still remain ...
	if ( i > middle )
	{
		// ... drop the rest of the second half's elements into the array
		while ( j <= last )
		{
			C[ r ] = A[ j ];
			j += 1;
			r += 1;
			MS_numOfOps += 5; // x2 addition, x3 assignment
		}
	}
	// Otherwise, do the same if elements of the first half still remain
	else
	{
		while ( i <= middle )
		{
			C[ r ] = A[ i ];
			i += 1;
			r += 1;
			MS_numOfOps += 5; // x2 addition, x3 assignment
		}
	}

	MS_numOfCmp += 1; // x1 if statement comparison
	
	// Copy the sorted elements of C back into A
	for ( r = first; r <= last; r++ )
	{
		A[ r ] = C[ r ];
		MS_numOfOps += 1; // x1 assignment
	}
}

void MergeSort( vector< int >& A, int first, int last )
{
	if ( first < last )
	{
		int middle = ( first + last ) / 2; // integer division

		// Recursively split the array and sort the two halves
		MergeSort( A, first, middle );
		MergeSort( A, middle + 1, last );

		// Merge the halves back together in sorted order
		Merge( A, first, middle, last );

		MS_numOfOps += 4; // x2 additions, x1 division, x1 assignment
	}
}

/*-------------------
	QUICK SORT
--------------------*/

void Swap( vector< int >& A, int h, int k )
{
	int temp = A[ k ];
	A[ k ] = A[ h ];
	A[ h ] = temp;
	QS_numOfOps += 3; // x3 assignments
}

int Partition( vector< int >& A, int first, int last )
{
	int pivot = A[ first ];
	int h = first;
	QS_numOfOps += 2; // x2 assignments

	// Cycle through the array partition (excluding the first element)
	for ( int k = first + 1; k <= last; k++ )
	{
		// If the current element < pivot, move it behind the h marker (via swapping)
		if ( A[ k ] < pivot )
		{
			h = h + 1;
			Swap( A, h, k );
			QS_numOfOps += 2; // x1 addition, x1 assignment
		}

		QS_numOfCmp += 1; // x1 if statement comparison
	}

	// Finally, place the pivot where the h marker is
	Swap( A, first, h );
	return h;
}

void QuickSort( vector< int >& A, int first, int last )
{
	if ( first < last )
	{
		// Organizes an array partition by placing elements < the pivot to the left, and elements >= pivot to the right
		int oldPivot = Partition( A, first, last );

		// Recursively partition into two halves (excluding the last used pivot)
		QuickSort( A, first, oldPivot - 1 );
		QuickSort( A, oldPivot + 1, last );

		QS_numOfOps += 3; // x1 addition, x1 subtraction, x1 assignment
	}
}

/*-------------------
	HELPER FUNCTIONS
--------------------*/

void printSortedList( vector< int >& A, string filename, string sortName, int opsCount, int cmpCount )
{
	output_file	<< "\nSorted " << A.size() << " elements via " << sortName << " in file " << filename << ".\n"
				<< "Number of operations: " << opsCount << "\n"
				<< "Number of comparisons: " << cmpCount << "\n";

	for ( int i = 0; i < A.size(); i++ )
	{
		output_file << A[ i ] << "\n";
	}
}