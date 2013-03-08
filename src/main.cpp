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
#ifdef __linux__
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Warning   30 warning C4291: 'void *txml::XmlComment::operator new(size_t,const nsl::tLfm &)' : no matching operator delete found; memory will not be freed if initialization throws an exception   c:\raprojekte\snippets\xmldemo\demo.cpp   218
#pragma warning( disable : 4800 ) // Warning   7  warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)   c:\raprojekte\snippets\xmldemo\mxml_base.h   57
#pragma warning( disable : 4996 ) // Warning   1  warning C4996: 'localtime': This function or variable may be unsafe. Consider using localtime_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.   c:\raprojekte\snippets\xmldemo\stringhelper.cpp 360
#pragma warning( disable : 4804 ) // Warning   12 warning C4804: '>' : unsafe use of type 'bool' in operation c:\raprojekte\snippets\xmldemo\mxml_document.cpp   407

#endif

bool hasMinusSign( std::string const& s ) {
   if( s.find( "-" ) != string::npos ) {
      return true;
   }

   return false;
}

int main( void ) {


   string fn = "readascii.txt";
   string textAsString;
   list<string> text;
   vector<uint8_t> buf;

   try {

      bin_read::tReadBin()( fn, buf );

   }  catch( bin_read::BadBinRead& ex ) {
      cout << ex.what() << endl;
   }

   try {

      // the testfile is written out, unchanged
      bin_write::tWriteBin()( "test_bin_write.txt", buf );

   }  catch( bin_write::BadBinWrite& ex ) {
      cout << ex.what() << endl;
   }



   try {
      cout << "read " << fn << endl;
      // with 'unnamed' Objekt and operator()
      text = text_read::tReadAscii()( fn );

      // or
      text_read::tReadAscii reader;
      // with 'named' Objekt 'reader' and operator()
      text = reader.operator()( fn );
      textAsString = reader.to_string( fn );
      //cout << textAsString;
   } catch( text_read::BadAsciiRead& br ) {
      cout << "read error: " << br.what() << endl;
      return 1;
   }

   cout << text.size() << " lines read " << fn << endl;

   cout << text.size() << ", remove lines with \"--\" in text" << endl;

   // process text, remove certain lines as an example
   text.remove_if( hasMinusSign );

   // ---------------- out ---------------
   string fnout = "test_ascii_write.txt";
   string fnout1 = "test_named_object_ascii_write.txt";


   try {
      cout << "write " << fnout << endl;

      // with 'unnamed' object und operator()
      text_write::tWriteAscii()( text, fnout );

      // with 'named' objekt 'reader' and operator()
      text_write::tWriteAscii writer;
      writer.operator()( text, fnout1 );
   } catch( text_write::BadAsciiWrite& bw ) {
      string err = bw.what();
      cout << "error writing" << err << endl;
      return 1;
   }

   cout << " end " << fn << endl;
}

