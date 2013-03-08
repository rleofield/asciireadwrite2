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



#include "rList.h"
#include "wList.h"
#include "wBin.h"
#include "rBin.h"

using namespace std;

bool hasMinusSign( std::string const& s){
    if( s.find("-") != string::npos ) return true;
    return false;
}

int main( void ) {


   string fn = "../readascii.txt";
   string textAsString;
   list<string> text;
   vector<uint8_t> buf;

   try {

      bin_data::tReadBin()( fn, buf );

   }  catch( bin_data::BadBinRead& ex ) {
      cout << ex.what() << endl;
   }

   try {

      // der testfile von oben wird wieder geschrieben,
      // im Test ist es ein Text, Texte kann man auch als bin schreiben
      bin_data::tWriteBin()( "../test_bin_write.txt", buf );

   }  catch( bin_data::BadBinWrite& ex ) {
      cout << ex.what() << endl;
   }

   fn = "../readascii.txt";

   try {
      cout << "read " << fn << endl;
      // mit 'unnamed' Objekt und operator()
      text = asciitext::tReadAscii()( fn );

      // oder
      asciitext::tReadAscii reader;
      // mit 'named' Objekt 'reader' und operator()
      text = reader.operator()( fn );
      textAsString = reader.to_string( fn );
      //cout << textAsString;
   } catch( asciitext::BadAsciiRead& br ) {
      cout << "read error: " << br.what() << endl;
      return 1;
   }

   cout << text.size() << " lines read " << fn << endl;

   cout << text.size() << ", remove lines with \"--\" in text" << endl;

   // process taxt, remove certain lines as example
   text.remove_if(hasMinusSign);

   // ---------------- out ---------------
   string fnout = "../test_ascii_write.txt";
   string fnout1 = "../test_named_object_ascii_write.txt";
   size_t count = 0;

   try {
      cout << "write " << fnout << endl;

      // with 'unnamed' object und operator()
      count = asciitext::tWriteAscii()( text, fnout );

      // with 'named' objekt 'reader' and operator()
      asciitext::tWriteAscii writer;
      count = writer.operator()( text, fnout1 );
   } catch( asciitext::BadAsciiWrite& bw ) {
      string err = bw.what();
      cout << "error writing" << err << endl;
      return 1;
   }

   cout << count << " lines written " << fn << endl;
}

