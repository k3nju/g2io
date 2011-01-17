#include "dispatchworker.h"
#include "controller.h"
#include "handlerbase.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
/*	int DispatchWorker::Thread( void *argv )
		{
		Controller *c = (Controller*)argv;

		try
			{
			while( true )
				{
				if( c->epollLock.TryAcquire() )
					{
					// Lock epoll
					scoped_lock_t locked;
					locked.SetAcquiredLock( c->epollLock );
					
					// select
					epoll_event_array_t epollEvents( new epoll_event[1024] );
					int eveSize = c->epoll.Select( epollEvents.get(),
												   1024,
												   -1 );
					
					if( eveSize == -1 )
						{
						continue;
						}

					printf( "%x:eveSize = %d\n", GetThreadID(), eveSize );
					
					// dequeue threads
					scoped_op_t so( c->waitThreads );
					wait_thread_queue_t::std_queue_t &q = c->waitThreads.GetStdQueue();
					
					size_t qsize = q.size();
					size_t n = 0;
					
					if( qsize == 0 )
						{
						// if no queue is waiting, process all event by current thread
						workEvents_ = epollEvents;
						begin_ = 0;
						size_ = 1;
						}
					else if( ( n = eveSize / qsize ) == 0 )
						{
						assert( (unsigned int)eveSize < qsize );
						
						// set work events to current thread
						workEvents_ = epollEvents;
						begin_ = 0;
						size_ = 1;
						--eveSize;
						
						// set work events to threads queued
						for( int i = 0; i < eveSize; ++i, --qsize )
							{
							((DispatchWorker*)q.front())->SetWork( epollEvents, i+1, 1 );
							q.pop();
							}

						// rest of threads don't work just re-acquire the epoll lock
						for( size_t i = 0; i < qsize; ++i )
							{
							((DispatchWorker*)q.front())->Next();
							q.pop();
							}
						}
					else
						{
						// set work events to threads queued
						size_t i = 0;
						for(; i < qsize-1; ++i )
							{
							((DispatchWorker*)q.front())->SetWork( epollEvents, i * n, n );
							q.pop();
							eveSize -= n;
							}
						
						workEvents_ = epollEvents;
						begin_ = i * n;
						size_ = eveSize;
						}
					}
				else
					{
					c->waitThreads.Enqueue( this );
					if( WaitWork() == false )
						{
						continue;
						}
					}
				
				for( size_t i = 0; i < size_; ++i )
					{
					size_t n = i*begin_;
					int e = workEvents_[n].events;
					HandlerBase *hb = (HandlerBase*)(workEvents_[n].data.ptr);

					printf( "%x:size_  = %d\n", GetThreadID(), size_ );
p					printf( "%x:begin_ = %d\n", GetThreadID(), begin_ );
					printf( "%x:hb     = %x\n", GetThreadID(), (HandlerBase*)(workEvents_[n].data.ptr) );
					
					int ret = hb->Handle( e, &( c->epoll ) );
					if( ret == -1 )
						{
						delete hb;
						}
					}
				}
			}
		catch( Exception &E )
			{
			E.Output();
			}

		puts( "returning" );
		return 0;
		}
*/


	//-----------------------------------------------------------------------------------------//
	int DispatchWorker::Thread( void *argv )
		{
		try
			{
			Dispatch( (Controller*)argv );
			}
		catch( Exception &E )
			{
			E.Output();
			}

		return 0;
		}

	//-----------------------------------------------------------------------------------------//
	void DispatchWorker::Dispatch( Controller *c )
		{
		while( true )
			{
			puts( "----------------" );
			if( c->epollLock.TryAcquire() )
				{
				scoped_lock_t locked;
				locked.SetAcquiredLock( c->epollLock );
				epoll_event_array_t epollEvents( new epoll_event[1024] );
				int epollCount = c->epoll.Select( epollEvents.get(),
												  1024,
												  -1 );
				
				printf( "epollCount = %d\n", epollCount );
				if( epollCount <= 0 )
					{
					continue;
					}

				Schedule( c, epollEvents, epollCount );
				}
			else
				{
				c->waitThreads.Enqueue( this );
				if( WaitWork() == false )
					{
					puts( "continue" );
					continue;
					}
				}

			printf( "work_.begin = %d\n", work_.begin );
			printf( "work_.size  = %d\n", work_.size );

			for( size_t i = 0; i < work_.size; ++i )
				{
				size_t n = work_.begin + i;
				printf( "handler %d\n", n );
				int e = work_.events[n].events;
				HandlerBase *hb = (HandlerBase*)( work_.events[n].data.ptr );
				int ret = hb->Handle( e, &( c->epoll ) );
				if( ret == -1 )
					{
					delete hb;
					}
				}
			}
		}

	//-----------------------------------------------------------------------------------------//
	void DispatchWorker::Schedule( Controller *c,
								   epoll_event_array_t epollEvents,
								   int epollCount )
		{
		scoped_op_t locked( c->waitThreads );
		wait_thread_queue_t::std_queue_t &q( c->waitThreads.GetStdQueue() );
		size_t qsize = q.size();
		size_t n = 0;
		
		if( qsize == 0 )
			{
			puts( "qsize == 0" );
			work_.begin = 0;
			work_.events = epollEvents;
			work_.size = epollCount;
			}
		else if( ( n = epollCount / qsize ) == 0 )
			{
			puts( "n == 0\n" );
			assert( epollCount < qsize );
			
			work_.begin = 0;
			work_.size = 1;
			work_.events = epollEvents;
			--epollCount;
			
			for( int i = 0; i < epollCount; ++i )
				{
				((DispatchWorker*)q.front())->SetWork( i + 1, 1, epollEvents );
				q.pop();
				}

			for( int i = 0; i < ( qsize - epollCount ); ++i )
				{
				((DispatchWorker*)q.front())->Next();
				q.pop();
				}
			
			assert( q.size() == 0 );
			}
		else
			{
			printf( "qsize = %d\n", qsize );
			int i = 0;
			for( i = 0; i < qsize; ++i )
				{
				((DispatchWorker*)q.front())->SetWork( i * n, n, epollEvents );
				q.pop();
				epollCount -= n;
				}
			
			work_.begin = i * n;
			work_.size = epollCount;
			work_.events = epollEvents;
			}
		}
	}


