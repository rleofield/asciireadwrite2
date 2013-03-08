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
 *  \brief class tWriteAscii, writes an text file
 *  \author Richard Albrecht
 *  Header file for class tWriteAscii.
 *  \sa wList
 */



#include <fstream>
#include <list>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>


namespace text_write {

namespace wl {
   namespace err {
      const std::string marker = "%s";
      inline std::string replace( std::string const& msg, std::string const& s0 = "" ) {
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

      const std::string msg_file_exists = "File exists: '%s'";
      const std::string msg_write_file = "Couldn't write file: '%s'";
      const std::string msg_open_file = "Couldn't open file for write: '%s'";
      const std::string msg_text_empty = "Text has no lines: '%s'";

      inline std::string write_file( std::string const& s0 ) {
         return replace( msg_write_file, s0 );
      }
      inline std::string file_exists( std::string const& s0 ) {
         return replace( msg_file_exists, s0 );
      }
      inline std::string file_open( std::string const& s0 ) {
         return replace( msg_open_file, s0 );
      }
      inline std::string text_empty( std::string const& s0 ) {
         return replace( msg_text_empty, s0 );
      }

   }

   inline bool file_exists( boost::filesystem::path const& p ) {
      if( !boost::filesystem::is_regular_file( p ) ) {
         return false;
      }

      boost::filesystem::file_status s = status( p );

      if( boost::filesystem::exists( s ) ) {
         return true;
      }

      return false;
   }

}


   /*! BadAsciiWrite,
      Exception, if text write fails
      \param [in] msg  error message
      */
   class BadAsciiWrite: public std::runtime_error {
   public:
      BadAsciiWrite( const std::string& msg )
         : std::runtime_error( msg ) { }
   };

   namespace {
      class writer {
         std::ofstream& _fp;
         std::string _f;
      public:
         writer( std::ofstream& fp, std::string f ): _fp( fp ),_f(f) {}
         void operator()( std::string const& s ) {
            _fp << s << std::endl;

            if( _fp.bad() ) {
                throw BadAsciiWrite( wl::err::write_file(_f) );
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
      void operator()( std::list<std::string> const& lines , const std::string& file ) {
          boost::filesystem::path pa( file );

          if( wl::file_exists( pa ) ) {
             throw BadAsciiWrite( wl::err::file_exists( file ) );
          }

          if( lines.size() == 0 ) {
             throw BadAsciiWrite( wl::err::text_empty( file ) );
          }

         if( lines.size() > 0 ) {
            std::ofstream fp( file.c_str() );

            if( fp.bad() ) {
                throw BadAsciiWrite( wl::err::file_open(file) );
            }

            for_each( lines.begin(), lines.end(), writer( fp, file ) );
         }


      }

   };

}// end of ns text_write

#endif

//EOF

