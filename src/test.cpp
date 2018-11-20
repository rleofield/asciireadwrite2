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
#include "test.h"


using namespace std;

using rlf_txtrw::t_text_read;
using rlf_txtrw::t_text_read_string;
using rlf_txtrw::t_write_text;
using rlf_txtrw::t_write_text;
//using rlf_txtrw::t_write_ascii_string;

namespace test_rw {


uint32_t rw_binary( ) {

    // test: read/write an asci file as binary file

    string testfilenameIn = "readascii.txt";
    string testfilenameOut = "test_bin_write.txt";
    vector<uint8_t> buffer;

    try {
        // read the file in one char C++ array
        cout << "read " << testfilenameIn << endl;
        buffer = bin_read::t_bin_read( testfilenameIn );
    }  catch( std::runtime_error& ex ) {
        cout << ex.what() << endl;
        return 1;
    }

    try {

        // write the file in one line of C++
        cout << "write " << testfilenameOut << endl;
        bool overwrite = true;
        bin_write::t_bin_write (testfilenameOut,  buffer, overwrite );

    }  catch( std::runtime_error& ex ) {
        cout << ex.what() << endl;
        return 1;
    }
    return 0;
}

uint32_t r_partofbinary( ) {

    string testfilenameIn = "readascii.txt";
    string testfilenameOut = "test_bin_write.txt";
    vector<uint8_t> buffer;

    try {
       // read part of file in one line of C++
       cout << "read " << testfilenameIn << endl;
       buffer = bin_read::t_bin_read( testfilenameIn,  5 );

    }  catch( std::runtime_error& ex ) {
       cout << ex.what() << endl;
       return 1;
    }
    return 0;
}


bool hasMinusSign( std::string const& s ) {
    if( s.find( "-" ) != string::npos ) {
        return true;
    }
    return false;
}


uint32_t rw_read_filter_ascii_file( ) {

    string testfilenameIn = "readascii.txt";
//    vector<uint8_t> buffer;

    // test: read/filter/write an ascii file

    vector<string> text_as_vector;
    list<string> text_as_list;
    string text_as_string;

    try {
        // read text file in one line of C++ into vector
        // result is in std::list
        cout << "read " << testfilenameIn << endl;
        text_as_vector = rlf_txtrw::text_read_t< std::vector<std::string>>()( testfilenameIn );
        text_as_list = rlf_txtrw::text_read_t< std::list<std::string>>()( testfilenameIn );

        // convert to string
        // result is in std::string with lineends '\n'
        text_as_string = rlf_hstring::merge( text_as_vector, "\n" );


        cout << "read as string, \r\n is converted to \n  " << testfilenameIn << endl;
        string text_as_string2 = t_text_read_string( testfilenameIn );
        cout << text_as_string2 << endl;


    } catch( std::runtime_error& br ) {
        cout << "read error: " << br.what() << endl;

    }

    cout << text_as_vector.size() << " lines read " << testfilenameIn << endl;

    // do a filter operation with 'remove_if()'
    cout << text_as_vector.size() << ", remove lines with \"--\" in text" << endl;

    // filter a text
    // process text, remove certain lines as an example
    text_as_vector.erase( remove_if( text_as_vector.begin(), text_as_vector.end(), hasMinusSign ), text_as_vector.end() );

    text_as_list.remove_if( hasMinusSign );
    // ---------------- out ---------------

    string testfilenameOut = "test_ascii_write.txt";

    try {
        // write the file in one line of C++
        cout << "write " << testfilenameOut << endl;

        // write vector with 'unnamed' object und operator()
        bool overwrite = true;
        t_write_text( testfilenameOut, text_as_vector, overwrite );

        // write list with 'unnamed' object und operator()
        t_write_text( testfilenameOut, text_as_list, true );


    } catch( std::runtime_error& bw ) {
        string err = bw.what();
        cout << "error writing: " << err << endl;

    }

    cout << " end " << testfilenameOut << endl;
    return 0;
}


uint32_t test( ) {

    // test: read/write an asci file as binary file
    uint32_t ret = rw_binary();
    if( ret == 1) return ret;

    ret = r_partofbinary();
    if( ret == 1) return ret;

    ret = rw_read_filter_ascii_file();
    if( ret == 1) return ret;

    cout << endl;
    return ret;
}
} // end of namespace test_rw

//EOF

