#include "dispatch.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	Dispatcher::Dispatcher( uint_t threadCount )
		:threads_()
		{
		assert( threadCount != 0 );
		
		for( uint_t i = 0; i < threadCount; ++i )
			{
			ThreadWorker *t = new ThreadWorker();
			t.SetArgv( (void*)i );
			threads_.push_back( t );
			}
		}

	//-----------------------------------------------------------------------------------------//
	Dispatcher::~Dispatcher()
		{
		try
			{
			thread_vector_t::iterator begin = threads_.begin();
			thread_vector_t::iterator end = threads_.end();
			for(; head != end; ++head )
				{
				delete *head;
				}
			}
		catch( ... )
			{
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Dispatch()
		{
		}

	//-----------------------------------------------------------------------------------------//
	int Dispatcher::WorkerThread::Thread( void *argv )
		{
		uint_t threadNumber = (uint_t)argv;
		epoll_event actives[EPOLL_EVENT_SIZE];
		
		while( true )
			{
			int count = epoll_.Select( actives, EPOLL_EVENT_SIZE, -1 );
			if( count <= 0 )
				{
				continue;
				}

			for( uint_t i = 0; i < (uint_t)count; ++i )
				{
				int e = actives[i].events;
				handler_t *handler = (handler_t*)( actives[i].data.ptr );
				handler->Handle( e );
				}
			}
		}
	}
