#include "dispatcher.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	Dispatcher::Dispatcher( size_t pollCount, size_t threadCount )
		:pollCount_( pollCount ),
		 threadCount_( threadCount ),
		 pollManager_(),
		 threadPool_()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Dispatch()
		{
		pollManager_.CreatePolls( pollCount_, &threadPool_ );
		
		for( size_t i = 0; i < threadCount_; ++i )
			{
			threadPool_.Add( &pollManager_ );
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
		PollManager *pollManager = (PollManager*)( wargs->GetArgs() );
		
		while( wargs->IsRunnable() )
			{
			poll_ptr_t poll = pollManager->GetPoll();
			struct epoll_event entries[1024];
			
			int count = poll->Select( entries, 1024 );
			if( count <= 0 )
				{
				continue;
				}

			for( size_t i = 0; i < (size_t)count; ++i )
				{
				int events = entries[i].events;
				IHandlerBase *handler = (IHandlerBase*)(entries[i].data.ptr);
				
				if( handler == NULL )
					{
					continue;
					}

				IHandlerBase::result r = handler->Handle( events, *poll );
				if( r == IHandlerBase::HANDLED )
					{
					delete handler;
					}
				}
			}

		return 0;
		}
	}


