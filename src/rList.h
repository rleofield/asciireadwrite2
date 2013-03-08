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


#ifndef asciitext_tReadAscii_H
#define asciitext_tReadAscii_H


/*! \file rList.h
*  \brief class tReadAscii, reads ascii files
*  \author Richard Albrecht
*  Header file for class tReadAscii.
*/



#include <fstream>
#include <sstream>
#include <iomanip>
#include <list>
#include <algorithm>
#include <stdexcept>



namespace asciitext {


   namespace {

   const std::string marker = "%s";
   std::string replace( std::string const& msg, std::string const& s0 = "") {
      std::string temp = msg;

      if( s0.size() > 0 ) {
         size_t pos = msg.find( marker );

         if( pos != std::string::npos ) {
            temp.erase( pos, marker.size() );
            temp.insert( pos, s0 );
         }
      }
    return temp;
   }

   const std::string msg_file_not_exist = "File doesn't exist: '%s'";
   const std::string msg_read_file = " Couldn't read file '%s'";

   std::string read_file_( std::string const& s0 ) {
      return replace( msg_read_file, s0 );
   }
   std::string file_not_exist( std::string const& s0 ) {
      return replace( msg_file_not_exist, s0 );
   }

      int access_( std::string const& fn, int mode ) {
         return ::access( fn.c_str(), mode );
      }
      inline bool file_exists( std::string const& in ) {
         if( access_( in, 0 ) ) {
            return false;
         }

         return true;
      }
   } // end of local namespace


   /*! BadAsciiRead
   \param [in] msg  Message
   */
   class BadAsciiRead: public std::runtime_error {
   public:
      BadAsciiRead( const std::string& msg )
         : std::runtime_error( msg ) { }
   };

   /*! \class tReadAscii
   *  \brief reads an ascii file
   *
   * an asciifile is stored in a <b>string</b> list<br>
   */
   class tReadAscii  {
      // no copy allowed
      tReadAscii& operator= ( const tReadAscii& in );
      tReadAscii( const tReadAscii& in );

      std::list<std::string> _lines;
   public:

      /*! \brief default ctor
      */
      tReadAscii(): _lines() {}
      ~tReadAscii() {}
      /*! \brief read a file
      */
      std::list<std::string> operator()( const std::string& file )  {
         if( !file_exists( file ) ) {
             std::string s = file_not_exist( file );
            throw BadAsciiRead( s );
         }

         std::ifstream fp( file.c_str() );

         if( fp.bad() ) {
            std::string s = read_file_( file );
            throw BadAsciiRead( s );
         }

         while( !fp.eof() ) {
            std::string temp;
            getline( fp, temp );

            if( !fp.fail() ) {
               _lines.push_back( temp );
            } else {
               if( !fp.eof() ) {
                   std::string s = read_file_( file );
                  throw BadAsciiRead( s );
               }
            }
         }

         return _lines;
      }
      struct merge {
         std::string s;
         const std::string sep;
         merge( std::string sep_ ): sep( sep_ ) {}
         void operator()( std::string const& temp ) {
            if( !s.empty() ) {
               s += sep;
               s += temp;
            } else {
               s += temp;
            }
         }
      };
      // converts the output list to one string, with linebreaks
      std::string to_string( const std::string& file ) {
         operator()( file );
         std::string temp = for_each( _lines.begin(), _lines.end(), merge( "\n" ) ).s;
         return temp;
      }
   };


}// end of namespace

#endif

//EOF

