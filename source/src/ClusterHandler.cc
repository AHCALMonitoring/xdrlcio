  /// \file ClusterHandler.cc
/*
 *
 * ClusterHandler.cc source template automatically generated by a class generator
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


#include "xdrlcio/ClusterHandler.h"
#include "xdrlcio/ParticleIDHandler.h"

#include "IOIMPL/ClusterIOImpl.h"
#include "IMPL/LCFlagImpl.h"
#include "EVENT/LCIO.h"

namespace xdrlcio
{

class XdrCluster : public IOIMPL::ClusterIOImpl
{
	friend class ClusterHandler;
};

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

xdrstream::Status ClusterHandler::read(xdrstream::IODevice *pDevice, EVENT::LCObject *&pLCObject)
{
	XdrCluster *pCluster = new XdrCluster();
	pLCObject = pCluster;

	int32_t type;
	XDR_STREAM( pDevice->read<int32_t>( &type ) )
	pCluster->setType(type);

	XDR_STREAM( pDevice->read<float>( &pCluster->_energy ) )
	XDR_STREAM( pDevice->read<float>( &pCluster->_energyError ) )
	XDR_STREAM( pDevice->readStaticArray( &pCluster->_position[0] , 3 ) )

	float errpos[ NERRPOS ];
	XDR_STREAM( pDevice->readStaticArray( &errpos[0] , NERRPOS ) )
	pCluster->setPositionError( errpos );

	XDR_STREAM( pDevice->read<float>( &pCluster->_theta ) )
	XDR_STREAM( pDevice->read<float>( &pCluster->_phi ) )

	float errdir[ NERRDIR ];
	XDR_STREAM( pDevice->readStaticArray( &errdir[0] , NERRDIR ) )
	pCluster->setDirectionError( errdir );

	uint32_t nShapes;
	XDR_STREAM( pDevice->read( &nShapes ) )
	pCluster->_shape.resize( nShapes );
	XDR_STREAM( pDevice->readStaticArray( &pCluster->_shape[0] , nShapes ) )

	uint32_t nPid;
	XDR_STREAM( pDevice->read( &nPid ) )

	for(uint32_t i=0 ; i<nPid ; i++)
	{
		EVENT::ParticleID *pParticleID = NULL;
		XDR_STREAM( ParticleIDHandler::read( pDevice , pParticleID , m_version ) )

		pCluster->addParticleID( pParticleID );
	}

	uint32_t nClusters;
	XDR_STREAM( pDevice->read( &nClusters ) )
	pCluster->_clusters.resize( nClusters );

	for( uint32_t i=0 ; i<nClusters ; i++ )
		XDR_STREAM( pDevice->readPointerReference( (void **) &pCluster->_clusters[i] ) )

	if( IMPL::LCFlagImpl( m_lcFlag).bitSet( EVENT::LCIO::CLBIT_HITS ) )
	{
		uint32_t nHits;
		XDR_STREAM( pDevice->read( &nHits ) )
		pCluster->_hits.resize( nHits );
		pCluster->_weights.resize( nHits );

		for(uint32_t i=0 ; i<nHits ; i++)
		{
			XDR_STREAM( pDevice->readPointerReference( (void **) &pCluster->_hits[i] ) )
			XDR_STREAM( pDevice->read( &pCluster->_weights[i] ) )
		}
	}

	uint32_t nEnergies;
	XDR_STREAM( pDevice->read( &nEnergies ) )
	pCluster->_subdetectorEnergies.resize( nEnergies );

	for(uint32_t i=0 ; i<nEnergies ; i++)
		XDR_STREAM( pDevice->read( &pCluster->_subdetectorEnergies[i] ) )

	XDR_STREAM( pDevice->readPointerTag( (void *) pCluster ) )

	return xdrstream::XDR_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

xdrstream::Status ClusterHandler::write(xdrstream::IODevice *pDevice, const EVENT::LCObject *const pLCObject)
{
	const EVENT::Cluster *const pCluster = dynamic_cast<const EVENT::Cluster *const>( pLCObject );

	int32_t type = pCluster->getType();
	XDR_STREAM( pDevice->write<int32_t>( &type ) )

	float energy = pCluster->getEnergy();
	XDR_STREAM( pDevice->write<float>( &energy ) )

	float energyError = pCluster->getEnergyError();
	XDR_STREAM( pDevice->write<float>( &energyError ) )

	XDR_STREAM( pDevice->writeArray( pCluster->getPosition() , 3 ) )
	XDR_STREAM( pDevice->writeArray( &pCluster->getPositionError()[0] , NERRPOS ) )

	float theta = pCluster->getITheta();
	XDR_STREAM( pDevice->write<float>( &theta ) )

	float phi = pCluster->getIPhi();
	XDR_STREAM( pDevice->write<float>( &phi ) )

	XDR_STREAM( pDevice->writeArray( &pCluster->getDirectionError()[0] , NERRDIR ) )

	const EVENT::FloatVec &shape = pCluster->getShape();
	uint32_t nShapes = shape.size();
	XDR_STREAM( pDevice->write( &nShapes ) )

	XDR_STREAM( pDevice->writeArray( &shape[0] , nShapes ) )

	uint32_t nPid = pCluster->getParticleIDs().size();
	XDR_STREAM( pDevice->write( &nPid ) )

	for(uint32_t i=0 ; i<nPid ; i++)
		XDR_STREAM( ParticleIDHandler::write( pDevice , pCluster->getParticleIDs()[i] ) )

	uint32_t nClusters = pCluster->getClusters().size();
	XDR_STREAM( pDevice->write( &nClusters ) )

	for( uint32_t i=0 ; i<nClusters ; i++ )
		XDR_STREAM( pDevice->writePointerReference( pCluster->getClusters()[i] ) )

	if( IMPL::LCFlagImpl( m_lcFlag).bitSet( EVENT::LCIO::CLBIT_HITS ) )
	{
		uint32_t nHits = pCluster->getCalorimeterHits().size();
		XDR_STREAM( pDevice->write( &nHits ) )

		for(uint32_t i=0 ; i<nHits ; i++)
		{
			XDR_STREAM( pDevice->writePointerReference( pCluster->getCalorimeterHits()[i] ) )
			XDR_STREAM( pDevice->write( &pCluster->getHitContributions()[i] ) )
		}
	}

	uint32_t nEnergies = pCluster->getSubdetectorEnergies().size();
	XDR_STREAM( pDevice->write( &nEnergies ) )

	for(uint32_t i=0 ; i<nEnergies ; i++)
		XDR_STREAM( pDevice->write( &pCluster->getSubdetectorEnergies()[i] ) )

	XDR_STREAM( pDevice->writePointerTag( pCluster ) )

	return xdrstream::XDR_SUCCESS;
}

} 

