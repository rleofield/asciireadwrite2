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
#include <string>



namespace bin_data{


	namespace rb{

		const std::string marker = "%s";
        std::string replace( std::string const& msg, std::string const& s0 = "" ) {
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

        const std::string msg_file_not_exist = "File doesn't exist: '%s'";
        const std::string msg_read_file = " Couldn't read file '%s'";

        ::std::string read_file_( std::string const& s0 ) {
			return replace( msg_read_file, s0 );
		}
        ::std::string file_not_exist( std::string const& s0 ) {
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
		// no copy allowed
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
			if( !rb::file_exists( file ) ) {
                std::string s = rb::file_not_exist( file );
				throw BadBinRead( s );
			}

            std::ifstream fp( file.c_str(), std::ios::in | std::ios::binary );

			if( fp.bad() ) {
                std::string s = rb::read_file_( file );
				throw BadBinRead( s );
			}

            fp.seekg( 0, std::ios::end );
            std::streamoff size = fp.tellg();
            fp.seekg( 0, std::ios::beg );
			buf.resize( static_cast<size_t>( size ), 0 );
			fp.read( ( char* )&buf[0], size );

			if( fp.eof() ) {
                std::string s = rb::read_file_( file );
				throw BadBinRead( s );
			}

		}
	};


}// end of namespace

#endif

//EOF

