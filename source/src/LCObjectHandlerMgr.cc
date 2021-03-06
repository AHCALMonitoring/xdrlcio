  /// \file LCObjectHandlerMgr.cc
/*
 *
 * LCObjectHandlerMgr.cc source template automatically generated by a class generator
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


// -- xdrlcio headers
#include "xdrlcio/LCObjectHandlerMgr.h"

// -- xdrlcio handler headers
#include "xdrlcio/ClusterHandler.h"
#include "xdrlcio/CaloHitHandler.h"
#include "xdrlcio/RawCaloHitHandler.h"
#include "xdrlcio/SimCaloHitHandler.h"
#include "xdrlcio/LCStlVecHandler.h"
#include "xdrlcio/LCGenericObjectHandler.h"
#include "xdrlcio/LCRelationHandler.h"
#include "xdrlcio/MCParticleHandler.h"
#include "xdrlcio/RecParticleHandler.h"
#include "xdrlcio/TrackerDataHandler.h"

// -- lcio headers
#include "EVENT/LCIO.h"

namespace xdrlcio
{

LCObjectHandlerMgr::LCObjectHandlerMgr() 
{
	m_objectHandlerMap[EVENT::LCIO::CLUSTER] = new ClusterHandler();
	m_objectHandlerMap[EVENT::LCIO::CALORIMETERHIT] = new CaloHitHandler();
	m_objectHandlerMap[EVENT::LCIO::RAWCALORIMETERHIT] = new RawCaloHitHandler();
	m_objectHandlerMap[EVENT::LCIO::SIMCALORIMETERHIT] = new SimCaloHitHandler();
	m_objectHandlerMap[EVENT::LCIO::LCFLOATVEC] = new LCFloatVecHandler();
	m_objectHandlerMap[EVENT::LCIO::LCINTVEC] = new LCIntVecHandler();
	m_objectHandlerMap[EVENT::LCIO::LCSTRVEC] = new LCStrVecHandler();
	m_objectHandlerMap[EVENT::LCIO::LCGENERICOBJECT] = new LCGenericObjectHandler();
	m_objectHandlerMap[EVENT::LCIO::LCRELATION] = new LCRelationHandler();
	m_objectHandlerMap[EVENT::LCIO::MCPARTICLE] = new MCParticleHandler();
	m_objectHandlerMap[EVENT::LCIO::RECONSTRUCTEDPARTICLE] = new RecParticleHandler();
	m_objectHandlerMap[EVENT::LCIO::TRACKERDATA] = new TrackerDataHandler();

}

//----------------------------------------------------------------------------------------------------

LCObjectHandlerMgr::~LCObjectHandlerMgr()
{
	for( std::map<std::string, LCObjectHandler *>::iterator iter = m_objectHandlerMap.begin(), endIter = m_objectHandlerMap.end() ;
			endIter != iter ; ++iter)
	{
		delete iter->second;
	}

	m_objectHandlerMap.clear();
}

//----------------------------------------------------------------------------------------------------

bool LCObjectHandlerMgr::addHandler( const std::string &handlerName , LCObjectHandler *pHandler )
{
	std::map<std::string, LCObjectHandler *>::iterator findIter = m_objectHandlerMap.find(handlerName);

	if( m_objectHandlerMap.end() != findIter )
	{
		delete pHandler;
		return false;
	}

	m_objectHandlerMap[ handlerName ] = pHandler;

	return true;
}

//----------------------------------------------------------------------------------------------------

LCObjectHandler *LCObjectHandlerMgr::getHandler( const std::string &handlerName ) const
{
	std::map<std::string, LCObjectHandler *>::const_iterator findIter = m_objectHandlerMap.find(handlerName);

	if( m_objectHandlerMap.end() != findIter )
		return findIter->second;

	return 0;
}

} 

