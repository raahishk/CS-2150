// Christian Kinzer
// cfk5ax
// 10/23/18
// 9:30 AM Lab
// wordPuzzle.cpp

#include "hashTable.h"
#include <fstream>

#define MAXROWS 500
#define MAXCOLS 500
char grid[MAXROWS][MAXCOLS];

bool readInGrid (string filename, int &rows, int &cols);
char* getWordInGrid (int startRow, int startCol, int dir, int len,
                     int numRows, int numCols);

int main(int argc, char ** argv) {
  string out = "";
  int numWords = 0;
  string direction = "";
  timer t;
  string dict = argv[1];
  ifstream file(dict.c_str());
  string line;
  int count = 0;
  while (file >> line) {
    count ++;
  }
  hashTable table = hashTable(count);
  file.close();
  ifstream file2(dict.c_str());
  while (file2 >> line) {
    table.insert(line);
  }
  file2.close();
  // dictionary is in the table
  int row, col;
  readInGrid(argv[2], row, col);
  // grid is read
  t.start();
  for (int r = 0; r < row; r++) {
    for (int c = 0; c < col; c++) {
      for (int d = 0; d < 8; d++) {
	for (int l = 3; l <= table.longest; l++) {
	  string word = getWordInGrid(r, c, d, l, row, col);
	  if (table.contains(word) && word.length() >= 3) {
	    if (d == 0 && l <= (r + 1)) {
	      out += "N (" + to_string(c) + ", " + to_string(r) + "):      " + word + "\n";
	      numWords++;
	    }
	    if (d == 2 && l <= (col - c)) {
	      out += "E (" + to_string(c) + ", " + to_string(r) + "):      " + word + "\n";
	      numWords++;
	    }
	    if (d == 4 && l <= (row - r)) {
	      out += "S (" + to_string(c) + ", " + to_string(r) + "):      " + word + "\n";
	      numWords++;
	    }
	    if (d == 6 && l <= (c + 1)) {
	      out += "W (" + to_string(c) + ", " + to_string(r) + "):      " + word + "\n";
	      numWords++;
	    }
	    if (d == 1 && l <= (r+1) && l <= (col - c)) {
	      out += "NE (" + to_string(c) + ", " + to_string(r) + "):     " + word + "\n";
	      numWords++;
	    }
	    if (d == 3 && l <= (col-c) && l <= (row-r)) {
	      out += "SE (" + to_string(c) + ", " + to_string(r) + "):     " + word + "\n";
	      numWords++;
	    }
	    if (d == 5 && l <= (row-r) && l <= (c+1)) {
	      out += "SW (" + to_string(c) + ", " + to_string(r) + "):     " + word + "\n";
	      numWords++;
	    }
	    if (d == 7 && l <= (c+1) && l <= (r+1)) {
	      out += "NW (" + to_string(c) + ", " + to_string(r) + "):     " + word + "\n";
	      numWords++;
	    }
	  }
	}
      }
    }
  }
  t.stop();
  cout << out << endl;
  cout << numWords << " words found." << endl;
  int time = (t.getTime() * 1000);
  cout << "Found all words in: " << t.getTime() << " seconds." << endl;
  cout << time << endl;	    
  return 0;
}

bool readInGrid (string filename, int &rows, int &cols) {
    // a C++ string to hold the line of data that is read in
    string line;
    // set up the file stream to read in the file (takes in a C-style
    // char* string, not a C++ string object)
    ifstream file(filename.c_str());
    // upon an error, return false
    if ( !file.is_open() )
        return false;
    // the first line is the number of rows: read it in
    file >> rows;
    cout << "There are " << rows << " rows." << endl;
    getline (file,line); // eats up the return at the end of the line
    // the second line is the number of cols: read it in and parse it
    file >> cols;
    cout << "There are " << cols << " cols." << endl;
    getline (file,line); // eats up the return at the end of the line
    // the third and last line is the data: read it in
    getline (file,line);
    // close the file
    file.close();
    // convert the string read in to the 2-D grid format into the
    // grid[][] array.  In the process, we'll print the grid to the
    // screen as well.
    int pos = 0; // the current position in the input data
    for ( int r = 0; r < rows; r++ ) {
        for ( int c = 0; c < cols; c++ ) {
            grid[r][c] = line[pos++];
            cout << grid[r][c] << " ";
        }
        cout << endl;
    }
    // return success!
    return true;
}

char* getWordInGrid (int startRow, int startCol, int dir, int len,
                     int numRows, int numCols) {
    // the static-ness of this variable prevents it from being
    // re-declared upon each function invocataion.  It also prevents it
    // from being deallocated between invocations.  It's probably not
    // good programming practice, but it's an efficient means to return
    // a value.
    static char output[256];
    // make sure the length is not greater than the array size.
    if ( len >= 255 )
        len = 255;
    // the position in the output array, the current row, and the
    // current column
    int pos = 0, r = startRow, c = startCol;
    // iterate once for each character in the output
    for ( int i = 0; i < len; i++ ) {
        // if the current row or column is out of bounds, then break
        if ( (c >= numCols) || (r >= numRows) || (r < 0) || (c < 0) )
            break;
        // set the next character in the output array to the next letter
        // in the grid
        output[pos++] = grid[r][c];
        // move in the direction specified by the parameter
        switch (dir) { // assumes grid[0][0] is in the upper-left
            case 0:
                r--;
                break; // north
            case 1:
                r--;
                c++;
                break; // north-east
            case 2:
                c++;
                break; // east
            case 3:
                r++;
                c++;
                break; // south-east
            case 4:
                r++;
                break; // south
            case 5:
                r++;
                c--;
                break; // south-west
            case 6:
                c--;
                break; // west
            case 7:
                r--;
                c--;
                break; // north-west
        }
    }
    // set the next character to zero (end-of-string)
    output[pos] = 0;
    // return the string (a C-style char* string, not a C++ string
    // object)
    return output;
}
