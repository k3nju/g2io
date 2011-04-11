#include "dispatcher.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	Dispatcher::Dispatcher( uint_t threadCount )
		:threadCount_( threadCount ),
		 threads_(),
		 run_( false ),
		 seq_( 0 )
		{
		assert( threadCount != 0 );
		
		for( uint_t i = 0; i < threadCount; ++i )
			{
			WorkerThread *t = new WorkerThread();
			t->SetArgs( this );
			threads_.push_back( t );
			}
		}

	//-----------------------------------------------------------------------------------------//
	Dispatcher::~Dispatcher()
		{
		try
			{
			threads_t::iterator head = threads_.begin();
			threads_t::iterator foot = threads_.end();
			for(; head != foot; ++head )
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
		run_ = true;
		threads_t::iterator head = threads_.begin();
		threads_t::iterator foot = threads_.end();

		try
			{
			for(; head != foot; ++head )
				{
				(*head)->Create();
				}
			
			head = threads_.begin();
			foot = threads_.end();
			for(; head != foot; ++head )
				{
				(*head)->Join();
				}
			}
		catch( g2::Exception &E )
			{
			head = threads_.begin();
			foot = threads_.end();
			
			for(; head != foot; ++head )
				{
				if( (*head)->IsRunning() == true )
					{
					(*head)->Cancel();
					}
				}

			head = threads_.begin();
			foot = threads_.end();

			for(; head != foot; ++head )
				{
				if( (*head)->IsRunning() == true )
					{
					(*head)->Join();
					}
				}
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Stop()
		{
		run_ = false;
		threads_t::iterator head = threads_.begin();
		threads_t::iterator foot = threads_.end();

		for(; head != foot; ++head )
			{
			(*head)->Stop();
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Register( int fd, int events, IHandlerBase *handler )
		{
		uint_t i = __sync_fetch_and_add( &seq_, 1 ) % threadCount_;
		threads_[i]->Register( fd, events, handler );
		}

	//-----------------------------------------------------------------------------------------//
	Dispatcher::WorkerThread::WorkerThread()
		:epoll_(),
		 stopEvent_( 0 )
		{
		epoll_.Register( stopEvent_.GetFD(), g2::event_type::READ, g2::EpollData( NULL ) );
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::WorkerThread::Register( int fd, int events, IHandlerBase *handler )
		{
		epoll_.Register( fd, events, g2::EpollData( handler ) );
		}

	//-----------------------------------------------------------------------------------------//
	int Dispatcher::WorkerThread::Thread( void *argv )
		{
		try
			{
			Dispatcher *disp = (Dispatcher*)argv;
			Poll poll( *disp, epoll_ );
			epoll_event actives[EPOLL_EVENT_SIZE];
		
			while( disp->run_ == true )
				{
				int count = epoll_.Select( actives, EPOLL_EVENT_SIZE, -1 );
				if( count <= 0 )
					{
					continue;
					}
			
				for( uint_t i = 0; i < (uint_t)count; ++i )
					{
					int e = actives[i].events;
					IHandlerBase *handler = (IHandlerBase*)(actives[i].data.ptr);
					if( handler == NULL )
						{
						continue;
						}
					
					if( handler->Handle( e, poll ) == IHandlerBase::HANDLED )
						{
						delete handler;
						}
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
		
		return 0;
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::WorkerThread::Stop()
		{
		stopEvent_.Write( 0 );
		}

	//-----------------------------------------------------------------------------------------//
	Dispatcher::Poll::Poll( Dispatcher &disp, g2::Epoll &ep )
		:disp_( disp ),
		 epoll_( ep )
		{
		}

	//-----------------------------------------------------------------------------------------//
	Dispatcher::Poll::~Poll()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Poll::Register( int fd, int events, IHandlerBase *handler )
		{
		disp_.Register( fd, events, handler );
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Poll::Update( int fd, int events, IHandlerBase *handler )
		{
		epoll_.Update( fd, events, g2::EpollData( handler ) );
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Poll::Unregister( int fd )
		{
		epoll_.Unregister( fd );
		}

	//-----------------------------------------------------------------------------------------//
	void Dispatcher::Poll::Stop()
		{
		disp_.Stop();
		}
	}
