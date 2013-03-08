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



#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include <cstdint>




namespace bin_data{

	namespace {
        const std::string marker = "%s";
        std::string replace( std::string const& msg, std::string const& s0 = "" ) {
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
		public:
            writer( std::ofstream& fp ): _fp( fp ) {}
            void operator()( std::string const& s ) {
                _fp << s << std::endl;

				if( _fp.bad() ) {
					throw BadBinWrite( "file  is bad" );
				}
			}
		};
	}

	/*! \class tWriteBin
	*  \brief writes bin files
	*
	* an asciifile is stored in a <b>string</b> list<br>
	*/
	class tWriteBin  {
		// no copy allowed
		tWriteBin( const tWriteBin& in );
		tWriteBin& operator= ( const tWriteBin& in );

	public:
		tWriteBin() {}
		~tWriteBin() {}
        size_t operator()( const std::string& file, std::vector<uint8_t> const& buf ) {

            std::ofstream fp( file.c_str(), std::ios::out | std::ios::binary );

			if( fp.bad() ) {
				// -- replace this with your error message
				throw BadBinWrite( "file: " + file + " is bad." );
			}

			char const* p = "";

			if( buf.size() > 0 ) {
				p = reinterpret_cast<char const*>( &buf[0] );
			}

			fp.write( p, buf.size() );
			return buf.size();
		}
        size_t operator()( const std::string& file, uint8_t const* p, size_t size ) {
            std::ofstream fp( file.c_str(), std::ios::out | std::ios::binary );
			if( fp.bad() ) {
				throw BadBinWrite( "error: open_file" );
			}
			fp.write( (char const*)(p), size );
			return size;
		}


	};

}// end of namespace

#endif

//EOF

