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


#ifndef asciitext_tWriteAscii_H
#define asciitext_tWriteAscii_H


/*! \file wList.h
 *  \brief class tWriteAscii, writes ascii files
 *  \author Richard Albrecht
 *  Header file for class tWriteAscii.
 *  \sa wList
 */



#include <fstream>
#include <list>
#include <stdexcept>
#include <string>
#include <algorithm>


//using std::string;


namespace asciitext {

   /*! BadRead,
      Exception, wenn ein text falsch geschrieben wurde
      \param [in] msg  Fehlermeldung
      */
   class BadAsciiWrite: public std::runtime_error {
   public:
      BadAsciiWrite( const std::string& msg )
         : std::runtime_error( msg ) { }
   };

   namespace {
      class writer {
         std::ofstream& _fp;
      public:
         writer( std::ofstream& fp ): _fp( fp ) {}
         void operator()( std::string const& s ) {
            _fp << s << std::endl;

            if( _fp.bad() ) {
               throw BadAsciiWrite( "file  is bad" );
            }
         }
      };
   }

   /*! \class tWriteAscii
       *  \brief writes ascii files
       *
       * an asciifile is stored in a <b>string</b> list<br>
       */
   class tWriteAscii  {
      // no copy allowed
      tWriteAscii( const tWriteAscii& in );
      tWriteAscii& operator= ( const tWriteAscii& in );

   public:
      tWriteAscii() {}
      ~tWriteAscii() {}
      size_t operator()( std::list<std::string> const& lines , const std::string& file ) {
         if( lines.size() > 0 ) {
            std::ofstream fp( file.c_str() );

            if( fp.bad() ) {
               // -- replace this with your error message
               throw BadAsciiWrite( "file: " + file + " is bad found." );
            }

            for_each( lines.begin(), lines.end(), writer( fp ) );
         }

         return lines.size();
      }

   };

}// end of namespace

#endif

//EOF

