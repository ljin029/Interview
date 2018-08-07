#include <iostream>
#include <cstring>
#include <string>

using namespace std;

// Function to find longest common substring.
string LCSubStr(string X, string Y)
{
    // Find length of both the strings.
    int m = X.length();
    int n = Y.length();
 
    // Variable to store length of longest
    // common substring.
    int result = 0;
 
    // Variable to store ending point of
    // longest common substring in X.
    int end;
 
    // Matrix to store result of two
    // consecutive rows at a time.
    int len[2][n];
 
    // Variable to represent which row of
    // matrix is current row.
    int currRow = 0;
 
    // For a particular value of i and j,
    // len[currRow][j] stores length of longest
    // common substring in string X[0..i] and Y[0..j].
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                len[currRow][j] = 0;
            }
            else if (X[i - 1] == Y[j - 1]) {
                len[currRow][j] = len[1 - currRow][j - 1] + 1;
                if (len[currRow][j] > result) {
                    result = len[currRow][j];
                    end = i - 1;
                }
            }
            else {
                len[currRow][j] = 0;
            }
        }
 
        // Make current row as previous row and
        // previous row as new current row.
        currRow = 1 - currRow;
    }
 
    // If there is no common substring, print -1.
    if (result == 0) {
        return "";
    }
 
    // Longest common substring is from index
    // end - result + 1 to index end in X.
    return X.substr(end - result + 1, result);
}
void printAllSubstr(string s1, string s2)
{
	for(int i=0; i<s1.size(); ++i)
	{
		for(int j=1; j<=(s1.size()-i); ++j)
		{
			string substr("");
			substr = LCSubStr(s1.substr(i, j), s2);
			if(!substr.empty())
			{
				printf("%s\n", substr.c_str());
			}
			else
			{
				printf("cannot find common substring\n");
			}
			
		}
	}
}
int main( int argc, const char * argv [] )
{
	printf( "argc = %d\n", argc);
	if (argc>3)
	{
		printf("Wrong input arguments\n");
		return 0;
	}
	for( int i = 0; i < argc; ++i )
	{
		printf( "argv[ %d ]: %s\n", i, argv[ i ] );
	}

	string str1(argv[1]);
	string str2(argv[2]);
        printf("str1: %s\n", str1.c_str());
        printf("str2: %s\n", str2.c_str());
	if (str1.size() < str2.size())
	{
		printAllSubstr(str1, str2);
	}
	else
	{
		printAllSubstr(str2, str1);
	}
}
