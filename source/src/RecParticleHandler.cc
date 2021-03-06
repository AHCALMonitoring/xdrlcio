  /// \file RecParticleHandler.cc
/*
 *
 * RecParticleHandler.cc source template automatically generated by a class generator
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


#include "xdrlcio/RecParticleHandler.h"
#include "xdrlcio/ParticleIDHandler.h"

#include "IOIMPL/ReconstructedParticleIOImpl.h"
#include "IMPL/LCFlagImpl.h"
#include "EVENT/LCIO.h"

namespace xdrlcio
{

class XdrRecParticle : public IOIMPL::ReconstructedParticleIOImpl
{
	friend class RecParticleHandler;
};

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

xdrstream::Status RecParticleHandler::read(xdrstream::IODevice *pDevice, EVENT::LCObject *&pLCObject)
{
	XdrRecParticle *pRecParticle = new XdrRecParticle();
	pLCObject = pRecParticle;

	XDR_STREAM( pDevice->read<int32_t>( & pRecParticle->_type ) )
	XDR_STREAM( pDevice->readStaticArray( & pRecParticle->_momentum , 3 ) )
	XDR_STREAM( pDevice->read( & pRecParticle->_energy ) )

	float covMatrix[ NCOVARIANCE ];
	XDR_STREAM( pDevice->readStaticArray( & covMatrix , NCOVARIANCE ) )
	pRecParticle->setCovMatrix( covMatrix );

	XDR_STREAM( pDevice->read( & pRecParticle->_mass ) )
	XDR_STREAM( pDevice->read( & pRecParticle->_charge ) )
	XDR_STREAM( pDevice->readStaticArray( & pRecParticle->_reference , 3 ) )

	uint32_t nPids;
	XDR_STREAM( pDevice->read( & nPids ) )

	for( uint32_t i=0 ; i<nPids ; i++ )
	{
		EVENT::ParticleID *pParticleID = NULL;
		XDR_STREAM( ParticleIDHandler::read( pDevice, pParticleID , m_version ) )

		pRecParticle->addParticleID( pParticleID );
	}

	XDR_STREAM( pDevice->read( & pRecParticle->_pidUsed ) )
	XDR_STREAM( pDevice->read( & pRecParticle->_goodnessOfPID ) )

	uint32_t nRecParts;
	XDR_STREAM( pDevice->read( & nRecParts ) )

	pRecParticle->_particles.resize( nRecParts );

	for( uint32_t i=0 ; i<nRecParts ; i++ )
		XDR_STREAM( pDevice->readPointerReference( (void **) & pRecParticle->_particles[i] ) )

	uint32_t nTracks;
	XDR_STREAM( pDevice->read( & nTracks ) )

	pRecParticle->_tracks.resize( nTracks );

	for( uint32_t i=0 ; i<nTracks ; i++ )
		XDR_STREAM( pDevice->readPointerReference( (void **) & pRecParticle->_tracks[i] ) )

	uint32_t nClusters;
	XDR_STREAM( pDevice->read( & nClusters ) )

	pRecParticle->_tracks.resize( nClusters );

	for( uint32_t i=0 ; i<nClusters ; i++ )
		XDR_STREAM( pDevice->readPointerReference( (void **) & pRecParticle->_clusters[i] ) )

	XDR_STREAM( pDevice->readPointerReference( (void **) & pRecParticle->_sv ) )

	return xdrstream::XDR_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

xdrstream::Status RecParticleHandler::write(xdrstream::IODevice *pDevice, const EVENT::LCObject *const pLCObject)
{
	const EVENT::ReconstructedParticle *const pRecParticle = dynamic_cast<const EVENT::ReconstructedParticle *const>( pLCObject );

	int32_t type = pRecParticle->getType();
	XDR_STREAM( pDevice->write<int32_t>( & type ) )

	XDR_STREAM( pDevice->writeArray( pRecParticle->getMomentum() , 3 ) )

	float energy = pRecParticle->getEnergy();
	XDR_STREAM( pDevice->write( & energy ) )

	XDR_STREAM( pDevice->writeArray( & pRecParticle->getCovMatrix()[0] , NCOVARIANCE ) )

	float mass = pRecParticle->getMass();
	XDR_STREAM( pDevice->write( & mass ) )

	float charge = pRecParticle->getCharge();
	XDR_STREAM( pDevice->write( & charge ) )

	XDR_STREAM( pDevice->writeArray( pRecParticle->getReferencePoint() , 3 ) )

	uint32_t nPids = pRecParticle->getParticleIDs().size();
	XDR_STREAM( pDevice->read( & nPids ) )

	for( uint32_t i=0 ; i<nPids ; i++ )
		XDR_STREAM( ParticleIDHandler::write( pDevice, pRecParticle->getParticleIDs()[i] ) )

	XDR_STREAM( pDevice->writePointerReference( (void *) pRecParticle->getParticleIDUsed() ) )
	float goodnessOfPID = pRecParticle->getGoodnessOfPID();
	XDR_STREAM( pDevice->write( & goodnessOfPID ) )

	uint32_t nRecParts = pRecParticle->getParticles().size();
	XDR_STREAM( pDevice->write( & nRecParts ) )

	for( uint32_t i=0 ; i<nRecParts ; i++ )
		XDR_STREAM( pDevice->writePointerReference( (void *) pRecParticle->getParticles()[i] ) )

	uint32_t nTracks = pRecParticle->getTracks().size();
	XDR_STREAM( pDevice->write( & nTracks ) )

	for( uint32_t i=0 ; i<nTracks ; i++ )
		XDR_STREAM( pDevice->writePointerReference( (void *) pRecParticle->getTracks()[i] ) )

	uint32_t nClusters = pRecParticle->getClusters().size();
	XDR_STREAM( pDevice->write( & nClusters ) )

	for( uint32_t i=0 ; i<nClusters ; i++ )
		XDR_STREAM( pDevice->writePointerReference( (void *) pRecParticle->getClusters()[i] ) )

	XDR_STREAM( pDevice->writePointerReference( (void *) pRecParticle->getStartVertex() ) )

	return xdrstream::XDR_SUCCESS;
}


} 

