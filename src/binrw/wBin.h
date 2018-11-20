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


#ifndef T_BIN_WRITE
#define T_BIN_WRITE


/*! \file wBin.h
*  \brief class t_bin_write, writes a binary file
*  \author Richard Albrecht
*/

#include <fstream>
#include <vector>
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>



namespace bin_write {


   namespace whelper {
      template <typename T>
      char const* to_char_ptr( std::vector<T> const& b ) {
         return reinterpret_cast<char const* >( b.data()  );
      }
      const std::string marker = "%s";
      std::string FindAndReplace( const std::string& source,
                                  const std::string& find,
                                  const std::string& replace ) {
         size_t i;
         size_t start = 0;
         std::string ret = source;

         for( ; ( i = ret.find( find, start ) ) != std::string::npos; ) {
            ret.replace( i, find.length(), replace );
            start = i + replace.length();
         }

         return ret;
      }

      std::string replace( std::string const& msg, std::string const& s0 = "" ) {

         if( s0.size() > 0 ) {
            return FindAndReplace( msg, whelper::marker, s0 );
         }

         return msg;

      }

      bool file_exists( boost::filesystem::path const& p ) {
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


   namespace err {
      const std::string msg_file_exists = "File exists: '" + whelper::marker + "'";
      const std::string msg_write_file = "Couldn't write file: '" + whelper::marker + "'";
      const std::string msg_open_file = "Couldn't open file for write: '" + whelper::marker + "'";
      const std::string msg_buffer_empty = "Buffer has zero size: '" + whelper::marker + "'";

      inline std::string write_file( std::string const& s0 ) {
         return whelper::replace( msg_write_file, s0 );
      }
      inline std::string file_exists( std::string const& s0 ) {
         return whelper::replace( msg_file_exists, s0 );
      }
      inline std::string file_open( std::string const& s0 ) {
         return whelper::replace( msg_open_file, s0 );
      }
      inline std::string buffer_empty( std::string const& s0 ) {
         return whelper::replace( msg_buffer_empty, s0 );
      }
   }

   /*! \class t_bin_write
   *  \brief writes a binary file
   */
   class t_bin_write  {
         t_bin_write( const t_bin_write& in );
         t_bin_write& operator= ( const t_bin_write& in );


         std::string _filename = std::string();

         void operator()( std::vector<uint8_t> const& buf, bool overwrite  ) {
            std::string file = _filename;


            if( ! overwrite ) {
            if( whelper::file_exists( file ) ) {
               throw std::runtime_error( err::file_exists( file ) );
            }
            }

            if( buf.size() == 0 ) {
               throw std::runtime_error( err::buffer_empty( file ) );
            }

            std::ios_base::openmode mode = std::ios::out | std::ios::binary;
            std::ofstream fp( file.c_str(), mode );

            if( !fp.is_open() ) {
               throw std::runtime_error( err::file_open( file ) );
            }

            auto p = whelper::to_char_ptr<uint8_t>( buf ) ;

            fp.write( p, static_cast<long>(buf.size()) );

            if( fp.bad() ) {
               throw std::runtime_error( err::write_file( file ) );
            }

         }

   public:
      t_bin_write( const std::string& filename, std::vector<uint8_t> const& buf, bool overwrite = true ): _filename( filename ){
         operator()( buf, overwrite );
      }

      ~t_bin_write() {}

   };

}// end of namespace

#endif

//EOF

