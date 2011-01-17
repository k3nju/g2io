#pragma once

#include "uncopyable.h"
#include "threading.h"
#include "epoll.h"
#include "config.h"

namespace g2io
	{
	using namespace g2;
	
	//-----------------------------------------------------------------------------------------//
	class DispatchWorker :public g2::Threading
		{
		public:
			void Register( int fd, int events, epoll_data_t *data )
				{
				epoll_.Register( fd, events, data );
				}
			
		private:
			virtual int Thread( void *argv );
			
			Epoll epoll_;
		};
	}
