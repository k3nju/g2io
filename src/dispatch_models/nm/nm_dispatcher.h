#pragma once

#include "includes.h"

namespace g2io
	{
	class Dispatcher
		{
			G2_MARK_UNCOPYABLE( Dispatcher );

		public:
			Dispatcher( uint32_t threadCount, uint32_t epollCount )
				:epollManager_( epollCount ),
				 dispatcherPool_( threadCount, epollManager_ )
				{
				}
			
			~Dispatcher();
			
			void Register( int fd, int events, IHandlerBase *handler )
				{
				epollManager_.Register( fd, events, handler );
				}
			
			void Dispatch()
				{
				dispPool_.Run();
				dispPool_.Join();
				}
			
			void Stop()
				{
				dispPool_.Stop();
				}

		private:
			EpollManager epollManager_;
			dispatcher_pool_t dispPool_;
		};
	}
