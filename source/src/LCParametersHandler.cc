  /// \file LCParametersHandler.cc
/*
 *
 * LCParametersHandler.cc source template automatically generated by a class generator
 * Creation date : ven. f�vr. 12 2016
 *
 * This file is part of xdrlcio libraries.
 * 
 * xdrlcio is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * xdrlcio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with xdrlcio.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#include "xdrlcio/LCParametersHandler.h"

namespace xdrlcio
{

xdrstream::Status LCParametersHandler::read(xdrstream::IODevice *pDevice, EVENT::LCParameters &lcParameters,
		xdrstream::xdr_version_t version)
{
	uint32_t nIntParameters;
	XDR_STREAM( pDevice->read(&nIntParameters) )

	for( uint32_t i=0 ; i<nIntParameters ; i++ )
	{
		std::string key;
		XDR_STREAM( pDevice->read(&key) )

		uint32_t nInt;
		XDR_STREAM( pDevice->read(&nInt) )

		EVENT::IntVec intVec( nInt );

		for( uint32_t j=0 ; j<nInt ; j++ )
		{
			int32_t value;
			XDR_STREAM( pDevice->read(&value) )
			intVec[j] = value;
		}

		lcParameters.setValues( key, intVec );
	}

	uint32_t nFloatParameters;
	XDR_STREAM( pDevice->read(&nFloatParameters) )

	for( uint32_t i=0 ; i<nFloatParameters ; i++ )
	{
		std::string key;
		XDR_STREAM( pDevice->read(&key) )

		uint32_t nFloat;
		XDR_STREAM( pDevice->read(&nFloat) )

		EVENT::FloatVec floatVec( nFloat );

		for( uint32_t j=0 ; j<nFloat ; j++ )
			XDR_STREAM( pDevice->read( &floatVec[j] ) )

		lcParameters.setValues( key, floatVec );
	}

	uint32_t nStringParameters;
	XDR_STREAM( pDevice->read(&nStringParameters) )

	for( uint32_t i=0 ; i<nStringParameters ; i++ )
	{
		std::string key;
		XDR_STREAM( pDevice->read(&key) )

		uint32_t nString;
		XDR_STREAM( pDevice->read(&nString) )

		EVENT::StringVec stringVec( nString );

		for( uint32_t j=0 ; j<nString ; j++ )
			XDR_STREAM( pDevice->read( &stringVec[j] ) )

		lcParameters.setValues( key, stringVec );
	}

	return xdrstream::XDR_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

xdrstream::Status LCParametersHandler::write(xdrstream::IODevice *pDevice, const EVENT::LCParameters &lcParameters)
{
	EVENT::StringVec intKeys;
	uint32_t nIntKeys = lcParameters.getIntKeys(intKeys).size();

	XDR_STREAM( pDevice->write( &nIntKeys ) )

	for( uint32_t i=0 ; i<nIntKeys ; i++ )
	{
		XDR_STREAM( pDevice->write( &intKeys[i] ) )

		EVENT::IntVec intVec;
		lcParameters.getIntVals( intKeys[i] , intVec );

		uint32_t nInt = intVec.size();
		XDR_STREAM( pDevice->write( &nInt ) )

		for( uint32_t j=0 ; j<nInt ; j++ )
			XDR_STREAM( pDevice->write<int32_t>( &intVec[j] ) )
	}

	EVENT::StringVec floatKeys;
	uint32_t nFloatKeys = lcParameters.getFloatKeys(floatKeys).size();

	XDR_STREAM( pDevice->write( &nFloatKeys ) )

	for( uint32_t i=0 ; i<nFloatKeys ; i++ )
	{
		XDR_STREAM( pDevice->write( &floatKeys[i] ) )

		EVENT::FloatVec floatVec;
		lcParameters.getFloatVals( floatKeys[i] , floatVec );

		uint32_t nFloat = floatVec.size();
		XDR_STREAM( pDevice->write( &nFloat ) )

		for( uint32_t j=0 ; j<nFloat ; j++ )
			XDR_STREAM( pDevice->write( &floatVec[j] ) )
	}

	EVENT::StringVec stringKeys;
	uint32_t nStringKeys = lcParameters.getStringKeys(stringKeys).size();

	XDR_STREAM( pDevice->write( &nStringKeys ) )

	for( uint32_t i=0 ; i<nStringKeys ; i++ )
	{
		XDR_STREAM( pDevice->write( &stringKeys[i] ) )

		EVENT::StringVec stringVec;
		lcParameters.getStringVals( stringKeys[i] , stringVec );

		uint32_t nString = stringVec.size();
		XDR_STREAM( pDevice->write( &nString ) )

		for( uint32_t j=0 ; j<nString ; j++ )
			XDR_STREAM( pDevice->write( &stringVec[j] ) )
	}

	return xdrstream::XDR_SUCCESS;
}

} 
