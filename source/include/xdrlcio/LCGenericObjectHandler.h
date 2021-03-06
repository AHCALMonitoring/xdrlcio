  /// \file LCGenericObjectHandler.h
/*
 *
 * LCGenericObjectHandler.h header template automatically generated by a class generator
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


#ifndef LCGENERICOBJECTHANDLER_H
#define LCGENERICOBJECTHANDLER_H

#include "xdrlcio/LCObjectHandler.h"

namespace xdrlcio
{

/** 
 *  @brief  LCGenericObjectHandler class
 */
class LCGenericObjectHandler : public LCObjectHandler
{
public:
	/**
	 *  @brief  Destructor
	 */
	~LCGenericObjectHandler() { /* nop */ }

	/**
	 *  @brief  Initialize the handler and/or the collection.
	 *           Read/write the flag and the parameters.
	 */
	xdrstream::Status init(xdrstream::IODevice *pDevice,
			xdrstream::StreamingMode mode,
			EVENT::LCCollection *pLCCollection,
			xdrstream::xdr_version_t version);

	/**
	 *  @brief  Reads calo hit objects from an XDR device.
	 */
	xdrstream::Status read(xdrstream::IODevice *pDevice, EVENT::LCObject *&pLCObject);

	/**
	 *  @brief  Writes calo hit objects to an SIO stream.
	 */
	xdrstream::Status write(xdrstream::IODevice *pDevice, const EVENT::LCObject *const pLCObject);

private:

	bool                m_isFixedSize;
	uint32_t            m_nInt;
	uint32_t            m_nFloat;
	uint32_t            m_nDouble;
};

} 

#endif  //  LCGENERICOBJECTHANDLER_H
