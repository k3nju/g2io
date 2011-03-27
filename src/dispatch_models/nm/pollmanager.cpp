#include "pollmanager.h"
#include "poll.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	Poll::Poll( size_t count )
		:epolls_()
		{
		for( size_t i = 0; i < count; ++i )
			{
			epolls_.push_back( epoll_ptr_t( new Poll( 1024 ) ) );
			}
		}

	//-----------------------------------------------------------------------------------------//
	epoll_ptr_t Poll::Get()
		{
		size_t i = __sync_fetch_and_add( &seq_, 1 ) & epolls_.size();
		return epolls_[i];
		}
	}
