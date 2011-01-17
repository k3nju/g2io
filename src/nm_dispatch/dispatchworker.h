#pragma once

#include <boost/shared_array.hpp>
#include "g2includes.h"

namespace g2io
	{
	using namespace g2;

	typedef boost::shared_array< epoll_event > epoll_event_array_t;

	typedef struct
		{
			size_t begin;
			size_t size;
			epoll_event_array_t events;
		} work_t;

	struct Controller;
	
	//-----------------------------------------------------------------------------------------//
	class DispatchWorker :public g2::Threading
		{
		public:
			work_t work_;
			
			DispatchWorker()
				:lock_(),
				 cond_( lock_ )
				{
				}
			
			void SetWork( size_t begin,
						  size_t size,
						  epoll_event_array_t epollEvents )
				{
				work_.begin = begin;
				work_.size = size;
				work_.events = epollEvents;
				
				cond_.Signal();
				}

			void Next()
				{
				work_.begin = 0;
				work_.size = 0;
				work_.events.reset();
				
				cond_.Signal();
				}

			bool WaitWork()
				{
				cond_.Wait();
				if( size_ == 0 )
					return false;
				return true;
				}
			
		private:
			virtual int Thread( void *argv );
			void Dispatch( Controller *c );
			void Schedule( Controller *c,
						   epoll_event_array_t epollEvents,
						   int epollCount );

			MutexLock lock_;
			ConditionValue cond_;
			epoll_event_array_t workEvents_;
			size_t begin_;
			size_t size_;
		};
	}
