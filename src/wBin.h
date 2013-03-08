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


#ifndef bin_data_tWriteBin_H
#define bin_data_tWriteBin_H


/*! \file wList.h
*  \brief class tWriteAscii, writes ascii files
*  \author Richard Albrecht
*  Header file for class tWriteAscii.
*  \sa wList
*/


#include <vector>
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>


namespace bin_write {
   namespace wb {
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
         const std::string msg_buffer_empty = "Buffer has zero size: '%s'";

         inline std::string write_file( std::string const& s0 ) {
            return replace( msg_write_file, s0 );
         }
         inline std::string file_exists( std::string const& s0 ) {
            return replace( msg_file_exists, s0 );
         }
         inline std::string file_open( std::string const& s0 ) {
            return replace( msg_open_file, s0 );
         }
         inline std::string buffer_empty( std::string const& s0 ) {
            return replace( msg_buffer_empty, s0 );
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

      inline char const* toCharPtr( std::vector<uint8_t> const& b ) {
         return reinterpret_cast<char const* >( static_cast<uint8_t const* >( &b[0] ) );
      }
   }
   /*! BadBinWrite,
   \param [in] msg  Message
   */
   class BadBinWrite: public std::runtime_error {
   public:
      BadBinWrite( const std::string& msg )
         : std::runtime_error( msg ) { }
   };

   namespace {
      class writer {
         std::ofstream& _fp;
         std::string _f;
      public:
         writer( std::ofstream& fp, std::string const& f ): _fp( fp ), _f( f ) {}
         void operator()( std::string const& s ) {
            _fp << s << std::endl;

            if( _fp.bad() ) {
               throw BadBinWrite( wb::err::write_file( _f ) );
            }
         }
      };
   }

   /*! \class tWriteBin
   *  \brief writes bin files
   */
   class tWriteBin  {
      tWriteBin( const tWriteBin& in );
      tWriteBin& operator= ( const tWriteBin& in );

   public:
      tWriteBin() {}
      ~tWriteBin() {}
      void operator()( const std::string& file, std::vector<uint8_t> const& buf ) {
         boost::filesystem::path pa( file );

         if( wb::file_exists( pa ) ) {
            throw BadBinWrite( wb::err::file_exists( file ) );
         }

         if( buf.size() == 0 ) {
            throw BadBinWrite( wb::err::buffer_empty( file ) );
         }

         std::ios_base::openmode mode = std::ios::out | std::ios::binary;
         std::ofstream fp( pa.c_str(), mode );

         if( fp.bad() ) {
            throw BadBinWrite( wb::err::file_open( file ) );
         }

         char const* p = wb::toCharPtr( buf );
         fp.write( p, buf.size() );

         if( fp.bad() ) {
            throw BadBinWrite( wb::err::write_file( file ) );
         }

      }
      void operator()( const std::string& file, uint8_t const* buf, size_t size ) {
         boost::filesystem::path pa( file );

         if( wb::file_exists( pa ) ) {
            throw BadBinWrite( wb::err::file_exists( file ) );
         }

         if( size == 0 ) {
            throw BadBinWrite( wb::err::buffer_empty( file ) );
         }

         std::ofstream fp( file.c_str(), std::ios::out | std::ios::binary );

         if( fp.bad() ) {
            throw BadBinWrite( wb::err::file_open( file ) );
         }

         char const* p =  reinterpret_cast<char const*>( buf );
         fp.write( p, size );

         if( fp.bad() ) {
            throw BadBinWrite( wb::err::write_file( file ) );
         }
      }


   };

}// end of namespace

#endif

//EOF

