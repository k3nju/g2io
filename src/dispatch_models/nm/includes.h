#pragma once

//-----------------------------------------------------------------------------------------//
#include <stdint.h>

//-----------------------------------------------------------------------------------------//
#include <map>
#include <vector>

//-----------------------------------------------------------------------------------------//
#include "uncopyable.h"
#include "epoll.h"
#include "epolldata.h"
#include "threading.h"
#include "mutexlock.h"
#include "conditonvalue.h"
#include "threadpool.h"

//-----------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//-----------------------------------------------------------------------------------------//
typedef boost::shared_ptr< g2::Epoll > epoll_ptr_t;

