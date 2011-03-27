#include "dispatcher.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	Dispatcher::Dispatcher( uint_t pollCount, uint_t threadCount )
		:pollManager_( pollCount ),
		 threadCount_( threadCount ),
		 threadPool_()
		{
		for( uint_t i = 0; i < threadCount; ++i )
			{
			threadPool_.Add( &pollManager_, NULL );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Dispatch()
		{
		for( uint_t i = 0; i < threadCount; ++i )
			{
			threadPool_.Add( &pollManager_, NULL );
			}

		threadPool_.Join();
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Stop()
		{
		threadPool_.Stop();
		}

	//-----------------------------------------------------------------------------------------//
	int Dispatcher::Worker::Thread( void *args )
		{
		g2::WorkerArgs *wargs = (g2::WorkerArgs*)args;
		PollManager *pollManager = wargs->GetArgs();
		
		while( wargs->IsRunnable() )
			{
			epoll_ptr_t poll = pollManager->GetPoll();
			struct epoll_event events[1024];
			
			int count = poll->Select( events, 1024, -1 );
			if( count <= 0 )
				{
				continue;
				}

			for( size_t i = 0; i < (size_t)count; ++i )
				{
				int events = events[i].events;
				IHandlerBase *handler = (IHandlerBase*)(events[i].data.ptr);

				if( 
				}
			}

		return 0;
		}
	}


