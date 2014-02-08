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

/*

1.

Lesen von Ascii Files mit operator()

std::list<std::string> operator()( const std::string &file )

Diese Zeile:
text = tReadAscii()( fn );

erzeugt ein unnamed Temp-Objekt tReadAscii()
an diesem temp-Objekt wird
text = operator( filenem)
aufgerufen.

Damit bleibt eine Zeile übrig, um einen Text zu lesen und einem Container abzulegen.


2.
Einsatz  einer Hilfsklasse mit operator(),
um Text zu schreiben:

for_each( _lines.begin(), _lines.end(), writer(fp));

Hilfsklasse
writer()




*/


#include <string>
#include <iostream>


#include "stringhelper.h"
#include "rBin.h"
#include "rList.h"
#include "wList.h"
#include "wBin.h"

using namespace std;

using rlf_txtrw::t_text_read;
using rlf_txtrw::toString;
using rlf_txtrw::bad_text_read;
using rlf_txtrw::bad_text_write;
using rlf_txtrw::t_write_ascii;

namespace test_rw{

bool hasMinusSign( std::string const& s ) {
   if( s.find( "-" ) != string::npos ) {
      return true;
   }

   return false;
}

void test(  ) {

   /*  int i = 0100;
     if( 64 & 0b01000000 ){
        i = (64 & 0b01000000);
     }
     if( 64 & 1 << 6 ){
        i = 1 << 6;
     }*/

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
   string textAsString2;

   try {
      // read text file in one line of C++
      // result is in std::list
      cout << "read " << testfilenameIn << endl;
      t_text_read()( testfilenameIn, text );

      // convert to string
      // result is in std::string with lineends
      textAsString = toString( text );


      cout << "read direct as string " << testfilenameIn << endl;
      t_text_read()( testfilenameIn, textAsString2 );


   } catch( bad_text_read& br ) {
      cout << "read error: " << br.what() << endl;

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

      // write list with 'unnamed' object und operator()
      t_write_ascii()( testfilenameOut, text, true );

      // write string with 'unnamed' object und operator()
      t_write_ascii()( testfilenameOut, textAsString, true);


   } catch( bad_text_write& bw ) {
      string err = bw.what();
      cout << "error writing: " << err << endl;

   }

   cout << " end " << testfilenameOut << endl;
}
} // end of namespace test_rw

//EOF
