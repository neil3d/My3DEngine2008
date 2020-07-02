/**
 *	nexus nshared - ndb_utility.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	April, 2010
 */

#ifndef _NSHARED_NDB_UTILITY_H_
#define _NSHARED_NDB_UTILITY_H_

#include "ncommon.h"

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/pool/object_pool.hpp>

//config-win.h必须增加和mysql.h同步包含(windows平台)
#include "config-win.h"
#include "mysql.h"
#include "errmsg.h"
#include "mysql_com.h"

//template<typename T>
//T* ndb_alloc(boost::object_pool<T>& obj_container)
//{
//	return (T*)( object_pool::construct() );
//}
//
//template<typename T>
//void ndb_free(boost::object_pool<T>& obj_container, T* ndb_obj)
//{
//	if(ndb_obj) obj_container.destroy(ndb_obj);
//}

#endif	_NSHARED_NDB_UTILITY_H_
