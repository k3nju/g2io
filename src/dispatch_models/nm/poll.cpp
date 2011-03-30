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
		 count_( 0 )
		{
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
		__sync_fetch_and_add( &count_, 1 );
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
	}
