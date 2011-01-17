#pragma once

#include "iregister.h"

namespace g2io
	{
	template < class Worker >
	class FDDistributor :public IRegister
		{
		public:
			void SetWorkers( Worker *workers, size_t count )
				{
				workers_ = workers;
				count_ = count;
				}
			
			void Register( int fd, int events, epoll_data_t *data )
				{
				size_t i = __sync_fetch_and_add( &seq_, 1 ) % count_;
				( workers_ + i )->Register( fd, events, data );
				}
			
		private:
			Worker *workers_;
			size_t count_;
			size_t seq_;
		};
	}
