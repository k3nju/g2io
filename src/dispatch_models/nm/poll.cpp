#include "poll.h"
#include "pollmanager.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	Poll::Poll( PollManager *pollManager )
		:pollManager_( pollManager ),
		 epoll_()
		{
		}
	
	//-----------------------------------------------------------------------------------------//
	Poll::~Poll()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void Poll::Register( int fd, int events, IHandlerBase *handler )
		{
		pollManager_->Register( fd, events, handler );
		}

	//-----------------------------------------------------------------------------------------//
	void Poll::Update( int fd, int events, IHandlerBase *handler )
		{
		epoll_->Update( fd, events, g2::EpollData( handler ) );
		}

	//-----------------------------------------------------------------------------------------//
	void Poll::Unregister( int fd )
		{
		epoll_->Unregister( fd );
		}

	//-----------------------------------------------------------------------------------------//
	void Poll::Stop()
		{
		}
	}
