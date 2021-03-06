  /// \file LCEventHeaderBlock.cc
/*
 *
 * LCEventHeaderBlock.cc source template automatically generated by a class generator
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


#include "xdrlcio/LCEventHeaderBlock.h"
#include "xdrlcio/LCParametersHandler.h"
#include "xdrlcioConfig.h"

#include "IOIMPL/LCCollectionIOVec.h"

namespace xdrlcio
{

LCEventHeaderBlock::LCEventHeaderBlock(xdrstream::XdrStream *pXdrStream, const std::string &name) :
		Block(pXdrStream, name)
{
	/* nop */
}

//----------------------------------------------------------------------------------------------------

LCEventHeaderBlock::~LCEventHeaderBlock() 
{
	/* nop */
}

//----------------------------------------------------------------------------------------------------

void LCEventHeaderBlock::setEvent(const EVENT::LCEvent *pLCEvent)
{
	m_pLCEvent = pLCEvent;
}

//----------------------------------------------------------------------------------------------------

void LCEventHeaderBlock::setEventPtr(XdrLCEvent **pLCEventPtr)
{
	m_pLCEventPtr = pLCEventPtr;
}

//----------------------------------------------------------------------------------------------------

xdrstream::Status LCEventHeaderBlock::stream(xdrstream::StreamingMode mode, xdrstream::IODevice *pDevice, xdrstream::xdr_version_t version)
{
	if( mode == xdrstream::XDR_READ_STREAM )
	{
		if( ! m_pLCEventPtr )
			return xdrstream::XDR_IO_ERROR;

		if( *m_pLCEventPtr )
			delete *m_pLCEventPtr;

		*m_pLCEventPtr = new XdrLCEvent();

		int32_t runNumber = 0;
		XDR_STREAM( pDevice->read( &runNumber ) )
		(*m_pLCEventPtr)->setRunNumber( runNumber );

		int32_t eventNumber = 0;
		XDR_STREAM( pDevice->read( &eventNumber ) )
		(*m_pLCEventPtr)->setEventNumber( eventNumber );

		uint64_t timeStamp = 0;
		XDR_STREAM( pDevice->read( &timeStamp ) )
		(*m_pLCEventPtr)->setTimeStamp( timeStamp );

		std::string detectorName;
		XDR_STREAM( pDevice->read( &detectorName ) )
		(*m_pLCEventPtr)->setDetectorName( detectorName );

		uint32_t nCollections;
		XDR_STREAM( pDevice->read( &nCollections ) )

		for( uint32_t i=0 ; i<nCollections ; i++ )
		{
			std::string collectionName;
			XDR_STREAM( pDevice->read( &collectionName ) )

			std::string collectionType;
			XDR_STREAM( pDevice->read( &collectionType ) )

			if( m_readCollectionNames.empty()
			 || m_readCollectionNames.find( collectionName ) != m_readCollectionNames.end() )
			{
				(*m_pLCEventPtr)->addCollection( new IOIMPL::LCCollectionIOVec(collectionType) , collectionName );
			}
		}

		XDR_STREAM( LCParametersHandler::read( pDevice , (*m_pLCEventPtr)->parameters() , version ) )
	}
	else if( mode == xdrstream::XDR_WRITE_STREAM )
	{
		if( m_pLCEvent )
		{
			int32_t runNumber = m_pLCEvent->getRunNumber();
			XDR_STREAM( pDevice->write( &runNumber ) )

			int32_t eventNumber = m_pLCEvent->getEventNumber();
			XDR_STREAM( pDevice->write( &eventNumber ) )

			uint64_t timeStamp = m_pLCEvent->getTimeStamp();
			XDR_STREAM( pDevice->write( &timeStamp ) )

			std::string detectorName = m_pLCEvent->getDetectorName();
			XDR_STREAM( pDevice->write( &detectorName ) )

			const EVENT::StringVec *pCollectionNames = m_pLCEvent->getCollectionNames();
			uint32_t nCollections = pCollectionNames->size();

			for(uint32_t i=0 ; i<pCollectionNames->size() ; i++)
				if( m_pLCEvent->getCollection( pCollectionNames->at(i) )->isTransient() )
					nCollections --;

			XDR_STREAM( pDevice->write( &nCollections ) )

			for(uint32_t i=0 ; i<pCollectionNames->size() ; i++)
			{
				const EVENT::LCCollection *pLCCollection = m_pLCEvent->getCollection( pCollectionNames->at(i) );

				if( pLCCollection->isTransient() )
					continue;

				XDR_STREAM( pDevice->write( &pCollectionNames->at(i) ) )

				std::string collectionType = pLCCollection->getTypeName();
				XDR_STREAM( pDevice->write( &collectionType ) )
			}

			XDR_STREAM( LCParametersHandler::write( pDevice , m_pLCEvent->getParameters() ) )
		}
	}

	return xdrstream::XDR_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

xdrstream::xdr_version_t LCEventHeaderBlock::getVersion() const
{
	return xdrlcio_VERSION;
}

//----------------------------------------------------------------------------------------------------

void LCEventHeaderBlock::setReadCollectionNames(const std::vector<std::string> &collectionNames)
{
	m_readCollectionNames.clear();
	m_readCollectionNames.insert( collectionNames.begin(), collectionNames.end());
}

} 

