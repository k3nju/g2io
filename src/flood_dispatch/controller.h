#pragma once

#include "g2includes.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	typedef g2::ThreadSafeQueue< void* > wait_thread_queue_t;
	typedef g2::ScopedOperation< wait_thread_queue_t > scoped_op_t;
	typedef g2::CriticalScope< g2::MutexLock > scoped_lock_t;
	
	//-----------------------------------------------------------------------------------------//
	struct Controller
		{
			g2::Epoll epoll;
			g2::MutexLock epollLock;
			wait_thread_queue_t waitThreads;
		};
	}
