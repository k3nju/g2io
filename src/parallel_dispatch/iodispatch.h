#pragma once

#include "uncopyable.h"
#include "iregistrar.h"

namespace g2io
	{

	//-----------------------------------------------------------------------------------------//
	template < class Worker, size_t Count >
	class IODispatch :public IRegistrar
		{
			G2_MARK_UNCOPYABLE( IODispatch );
			
		public:
			IODispatch()
				:seq_( 0 )
				{
				}

			void Dispatch()
				{
				for( size_t i = 0; i < Count; ++i )
					{
					workers_[i].Create();
					}
				for( size_t i = 0; i < Count; ++i )
					{
					workers_[i].Join();
					}
				}
			
			virtual void Register( int fd, int events, epoll_data_t *data )
				{
				size_t i = __sync_fetch_and_add( &seq_, 1 ) % Count;
				( workers_ + i )->Register( fd, events, data );
				}
			
		private:
			Worker workers_[ Count ];
			size_t seq_;
		};
	}

