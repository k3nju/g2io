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
			Controller( size_t s )
				:epollSeq( 0 ),
				 epollList( NULL )
				{
				epollList = new g2::Epoll[ s ];
				epollSeq = s;
				}

			g2::Epoll* GetEpoll()
				{
				epollSeq = 
				return epollList->
				}
			
			size_t epollSeq;
			g2::Epoll *epollList;
		};
	}
