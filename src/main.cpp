/* --------------------------------------------------------------------------
Copyright 2012 by Richard Albrecht
richard.albrecht@rleofield.de
www.rleofield.de

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------------------------------------
*/


#include <string>
#include <iostream>

#include "rBin.h"
#include "rList.h"
#include "wList.h"
#include "wBin.h"


using namespace std;
#pragma GCC diagnostic ignored "-Wunused-variable"


bool hasMinusSign( std::string const& s ) {
   if( s.find( "-" ) != string::npos ) {
      return true;
   }

   return false;
}




int main( void ) {



   // test: read/write a binary file

   string testfilenameIn = "readascii.txt";
   string testfilenameOut = "test_bin_write.txt";
   vector<uint8_t> buffer;

   try {
      // read the file in one line of C++
      cout << "read " << testfilenameIn << endl;
      bin_read::t_bin_read()( testfilenameIn,  buffer );

   }  catch( bin_read::bad_bin_read& ex ) {
      cout << ex.what() << endl;
   }

   try {

      // write the file in one line of C++
      cout << "write " << testfilenameOut << endl;
      bin_write::t_bin_write()( testfilenameOut, buffer );

   }  catch( bin_write::bad_bin_write& ex ) {
      cout << ex.what() << endl;
   }

   // test: read/filter/write an ascii file

   list<string> text;
   string textAsString;

   try {
      // read text file in one line of C++
      // result is in std::list
      cout << "read " << testfilenameIn << endl;
      text_read::t_text_read()( testfilenameIn, text );

      // convert to string
      // result is in std::string with lineends
      textAsString = text_read::to_string( text );
   } catch( text_read::bad_text_read& br ) {
      cout << "read error: " << br.what() << endl;
      return 1;
   }
   cout << text.size() << " lines read " << testfilenameIn << endl;

   // do a filter operation with 'remove_if()'
   cout << text.size() << ", remove lines with \"--\" in text" << endl;

   // process text, remove certain lines as an example
   text.remove_if( hasMinusSign );

   // ---------------- out ---------------

   testfilenameOut = "test_ascii_write.txt";

   try {
      // write the file in one line of C++
      cout << "write " << testfilenameOut << endl;

      // with 'unnamed' object und operator()
      text_write::t_write_ascii()( text, testfilenameOut );

   } catch( text_write::bad_text_write& bw ) {
      string err = bw.what();
      cout << "error writing: " << err << endl;
      return 1;
   }

   cout << " end " << testfilenameOut << endl;
}

