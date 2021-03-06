  /// \file LCGenericObjectHandler.cc
/*
 *
 * LCGenericObjectHandler.cc source template automatically generated by a class generator
 * Creation date : sam. f�vr. 20 2016
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


#include "xdrlcio/LCGenericObjectHandler.h"

#include "IOIMPL/LCGenericObjectIOImpl.h"
#include "IMPL/LCFlagImpl.h"
#include "EVENT/LCIO.h"

namespace xdrlcio
{

class XdrLCGenericObject : public IOIMPL::LCGenericObjectIOImpl
{
	friend class LCGenericObjectHandler;
};

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

xdrstream::Status LCGenericObjectHandler::init(xdrstream::IODevice *pDevice, xdrstream::StreamingMode mode,
		EVENT::LCCollection *pLCCollection, xdrstream::xdr_version_t version)
{
	if( xdrstream::XDR_READ_STREAM == mode )
	{
	      XDR_STREAM( LCObjectHandler::init( pDevice , mode , pLCCollection , version ) )

	      m_isFixedSize = IMPL::LCFlagImpl(m_lcFlag).bitSet( EVENT::LCIO::GOBIT_FIXED ) ;

	      if( m_isFixedSize )
	      {
	    	  XDR_STREAM( pDevice->read( & m_nInt ) )
			  XDR_STREAM( pDevice->read( & m_nFloat ) )
			  XDR_STREAM( pDevice->read( & m_nDouble ) )
	      }
	}
	else
	{
		m_isFixedSize = true;

		uint32_t nObj = pLCCollection->getNumberOfElements();

		for( uint32_t i=0 ; i < nObj ; i++ )
		{
			const EVENT::LCGenericObject *const pLCGenericObject
				= dynamic_cast< const EVENT::LCGenericObject * const>( pLCCollection->getElementAt( i ) );

			if( ! pLCGenericObject->isFixedSize() )
			{
				m_isFixedSize = false;
				break;
			}
		}

		IMPL::LCFlagImpl lcFlag( pLCCollection->getFlag() );

		const EVENT::LCGenericObject * pLCGo = 0;

		if( pLCCollection->getNumberOfElements() > 0 )
			pLCGo = dynamic_cast< const EVENT::LCGenericObject* >( pLCCollection->getElementAt( 0 ) ) ;

		if( m_isFixedSize )
		{
			lcFlag.setBit( EVENT::LCIO::GOBIT_FIXED );

			if( pLCCollection->parameters().getStringVal( "DataDescription" ).empty()
					&& NULL != pLCGo)
				pLCCollection->parameters().setValue( "DataDescription" , pLCGo->getDataDescription() );
		}

		m_lcFlag = lcFlag.getFlag();

		XDR_STREAM( LCObjectHandler::init( pDevice , mode , pLCCollection , version ) )

		if( m_isFixedSize )
		{
			if( pLCGo )
			{
				m_nInt = pLCGo->getNInt();
				m_nFloat = pLCGo->getNFloat();
				m_nDouble = pLCGo->getNDouble();
			}
			else
			{
				m_nInt = 0;
				m_nFloat = 0;
				m_nDouble = 0;
			}

			XDR_STREAM( pDevice->write( & m_nInt ) )
			XDR_STREAM( pDevice->write( & m_nFloat ) )
			XDR_STREAM( pDevice->write( & m_nDouble ) )
		}
	}

	return xdrstream::XDR_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

xdrstream::Status LCGenericObjectHandler::read(xdrstream::IODevice *pDevice, EVENT::LCObject *&pLCObject)
{
	XdrLCGenericObject *pLCGenericObject = new XdrLCGenericObject();
	pLCObject = pLCGenericObject;

	pLCGenericObject->_isFixedSize = m_isFixedSize;

	if( ! m_isFixedSize )
	{
		XDR_STREAM( pDevice->read( & m_nInt ) )
		XDR_STREAM( pDevice->read( & m_nFloat ) )
		XDR_STREAM( pDevice->read( & m_nDouble ) )
	}

	pLCGenericObject->_intVec.resize( m_nInt );
	pLCGenericObject->_floatVec.resize( m_nFloat );
	pLCGenericObject->_doubleVec.resize( m_nDouble );

	for( uint32_t i=0 ; i<m_nInt ; i++ )
		XDR_STREAM( pDevice->read<int32_t>( & pLCGenericObject->_intVec[i] ) )

	for( uint32_t i=0 ; i<m_nFloat ; i++ )
		XDR_STREAM( pDevice->read( & pLCGenericObject->_floatVec[i] ) )

	for( uint32_t i=0 ; i<m_nDouble ; i++ )
		XDR_STREAM( pDevice->read( & pLCGenericObject->_doubleVec[i] ) )

	XDR_STREAM( pDevice->readPointerTag( pLCGenericObject ) )

	return xdrstream::XDR_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

xdrstream::Status LCGenericObjectHandler::write(xdrstream::IODevice *pDevice, const EVENT::LCObject *const pLCObject)
{
	const EVENT::LCGenericObject *const pLCGenericObject
		= dynamic_cast< const EVENT::LCGenericObject * const>( pLCObject );

	if( ! m_isFixedSize )
	{
		m_nInt = pLCGenericObject->getNInt();
		m_nFloat = pLCGenericObject->getNFloat();
		m_nDouble = pLCGenericObject->getNDouble();

		XDR_STREAM( pDevice->write( & m_nInt ) )
		XDR_STREAM( pDevice->write( & m_nFloat ) )
		XDR_STREAM( pDevice->write( & m_nDouble ) )
	}

	for( uint32_t i=0 ; i<m_nInt ; i++ )
	{
		int32_t value = pLCGenericObject->getIntVal(i);
		XDR_STREAM( pDevice->write<int32_t>( & value ) )
	}

	for( uint32_t i=0 ; i<m_nFloat ; i++ )
	{
		float value = pLCGenericObject->getFloatVal(i);
		XDR_STREAM( pDevice->write( & value ) )
	}

	for( uint32_t i=0 ; i<m_nDouble ; i++ )
	{
		double value = pLCGenericObject->getDoubleVal(i);
		XDR_STREAM( pDevice->write( & value ) )
	}

	XDR_STREAM( pDevice->writePointerTag( pLCGenericObject ) )

	return xdrstream::XDR_SUCCESS;
}

} 

