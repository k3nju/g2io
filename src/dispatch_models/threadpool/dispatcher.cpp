#include "dispatcher.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	Dispatcher::Dispatcher( size_t pollCount, size_t threadCount )
		:threadCount_( threadCount ),
		 pollManager_(),
		 threadPool_()
		{
		pollManager_.CreatePolls( pollCount, &threadPool_ );
		}

	//-----------------------------------------------------------------------------------------//
	Dispatcher::~Dispatcher()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Register( int fd, int events, IHandlerBase *handler )
		{
		pollManager_.Register( fd, events, handler );
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Dispatch()
		{
		for( size_t i = 0; i < threadCount_; ++i )
			{
			threadPool_.Add< Worker >( pollManager_ );
			}

		threadPool_.Join();
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Stop()
		{
		threadPool_.Stop();
		pollManager_.Stop();
		}
	
	//-----------------------------------------------------------------------------------------//
	Dispatcher::Worker::Worker( PollManager &pollManager )
		:pollManager_( pollManager )
		{
		}
	
	//-----------------------------------------------------------------------------------------//
	int Dispatcher::Worker::Thread( void *args )
		{
		g2::WorkerArgs *wargs = (g2::WorkerArgs*)args;

		while( wargs->IsRunnable() == true )
			{
			try
				{
				struct epoll_event actives[EPOLL_EVENT_SIZE];
				select_result_t result = pollManager_.Select( actives, EPOLL_EVENT_SIZE );
				int count = result.first;
				if( count <= 0 )
					{
					continue;
					}
				
				for( size_t i = 0; i < (size_t)count; ++i )
					{
					int events = actives[i].events;
					IHandlerBase *handler = (IHandlerBase*)(actives[i].data.ptr);
					if( handler == NULL )
						{
						continue;
						}
					
					if( handler->Handle( events, *(result.second) ) == IHandlerBase::HANDLED )
						{
						delete handler;
						}
					}
				}
			catch( g2::Exception &E )
				{
				E.Output();
				}
			catch( ... )
				{
				// TODO: log
				}
			}
		
		return 0;
		}
	}
