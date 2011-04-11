#include "poll.h"
#include <assert.h>
#include "pollmanager.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	Poll::Poll()
		:pollManager_( NULL ),
		 threadPool_( NULL ),
		 epoll_(),
		 count_( 0 ),
		 isPolling_( false ),
		 stopEvent_( 0 )
		{
		epoll_.Register( stopEvent_.GetFD(), g2::event_type::READ, g2::EpollData( NULL ) );
		}
	
	//-----------------------------------------------------------------------------------------//
	Poll::~Poll()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void Poll::Register( int fd, int events, IHandlerBase *handler )
		{
		assert( pollManager_ != NULL );
		pollManager_->Register( fd, events, handler );
		}

	//-----------------------------------------------------------------------------------------//
	void Poll::Update( int fd, int events, IHandlerBase *handler )
		{
		assert( pollManager_ != NULL );
		epoll_.Update( fd, events, g2::EpollData( handler ) );
		}

	//-----------------------------------------------------------------------------------------//
	void Poll::Unregister( int fd )
		{
		assert( pollManager_ != NULL );
		epoll_.Unregister( fd );
		__sync_fetch_and_sub( &count_, 1 );
		}

	//-----------------------------------------------------------------------------------------//
	void Poll::Stop()
		{
		assert( threadPool_ != NULL );
		threadPool_->Stop();
		}

	//-----------------------------------------------------------------------------------------//
	int Poll::Select( epoll_event *events, size_t size )
		{
		int ret = 0;

		if( __sync_bool_compare_and_swap( &isPolling_, false, true ) )
			{
			try
				{
				ret = epoll_.Select( events, size, -1 );
				}
			catch( ... )
				{
				__sync_bool_compare_and_swap( &isPolling_, true, false );
				throw;
				}
			
			__sync_bool_compare_and_swap( &isPolling_, true, false );
			}
		
		return ret;
		}

	//-----------------------------------------------------------------------------------------//
	void Poll::RegisterImpl( int fd, int events, IHandlerBase *handler )
		{
		epoll_.Register( fd, events, g2::EpollData( handler ) );
		__sync_fetch_and_add( &count_, 1 );
		}

	//-----------------------------------------------------------------------------------------//
	void Poll::StopPolling()
		{
		stopEvent_.Write( 0 );
		}
	}
