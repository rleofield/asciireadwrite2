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


#ifndef bin_data_tReadBin_H
#define bin_data_tReadBin_H



/*! \file rList.h
*  \brief class tReadBin, reads binary files
*  \author Richard Albrecht
*  Header file for class tReadBin.
*/


#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>



namespace bin_read {


   namespace rb {

      namespace err {
         const std::string marker = "%s";
         inline std::string replace( std::string const& msg, std::string const& s0 = "" ) {
            ::std::string temp = msg;

            if( s0.size() > 0 ) {
               size_t pos = msg.find( marker );

               if( pos != ::std::string::npos ) {
                  temp.erase( pos, marker.size() );
                  temp.insert( pos, s0 );
               }
            }

            return temp;

         }

         const std::string msg_file_not_exists = "File doesn't exist: '%s'";
         const std::string msg_read_file = " Couldn't read file '%s'";

         inline std::string read_file_( std::string const& s0 ) {
            return replace( msg_read_file, s0 );
         }
         inline std::string file_not_exists( std::string const& s0 ) {
            return replace( msg_file_not_exists, s0 );
         }
      } // end of ns err


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
      inline char* toCharPtr( std::vector<uint8_t> &b ) {
         return reinterpret_cast<char* >( static_cast<uint8_t* >( &b[0] ) );
      }

   } // end of ns rb


   /*! BadBinRead
       \param [in] msg  error message
       */
   class BadBinRead: public std::runtime_error {
   public:
      BadBinRead( const std::string& msg )
         : std::runtime_error( msg ) { }
   };

   /*! \class tReadBin
       *  \brief reads a binary file
       *
       */
   class tReadBin  {
      tReadBin& operator= ( const tReadBin& in );
      tReadBin( const tReadBin& in );
   public:

      /*! \brief default ctor
          */
      tReadBin() {}
      ~tReadBin() {}
      /*! \brief read a file
          */
      void operator()( const std::string& file, std::vector<uint8_t> &buf )  {
         boost::filesystem::path p( file );

         if( !rb::file_exists( p ) ) {
            std::string s = rb::err::file_not_exists( file );
            throw BadBinRead( s );
         }

         std::uintmax_t size = boost::filesystem::file_size( p );

         std::ios_base::openmode mode = std::ios::in | std::ios::binary;
         std::ifstream fp( p.c_str(), mode );

         if( fp.bad() ) {
            std::string s = rb::err::read_file_( file );
            throw BadBinRead( s );
         }

         buf.resize( static_cast<size_t>(size), 0 );
         char* const buffer = rb::toCharPtr( buf );
         fp.read( buffer, size );

         if( fp.eof() ) {
            std::string s = rb::err::read_file_( file );
            throw BadBinRead( s );
         }

      }
   };


} // end of ns bin_read

#endif

//EOF

