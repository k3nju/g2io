#include "pollmanager.h"
#include "poll.h"
#include <assert.h>

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	PollManager::PollManager()
		:seq_( 0 ),
		 regIndex_( 0 ),
		 regIndexLock_(),
		 polls_()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void PollManager::CreatePolls( size_t count, g2::IThreadPool *threadPool )
		{
		assert( count > 0 );
		
		for( size_t i = 0; i < count; ++i )
			{
			poll_ptr_t p( new Poll() );
			p->SetPollManager( this );
			p->SetThreadPool( threadPool );
			polls_.push_back( p );
			}

		regIndex_ = count - 1;
		}

	//-----------------------------------------------------------------------------------------//
	void PollManager::Register( int fd, int events, IHandlerBase *handler )
		{
		g2::CriticalScope<> locked( regIndexLock_ );
		
		size_t tmp = polls_[regIndex_]->GetCount();
		size_t size = polls_.size();
		
		for( size_t i = 0; i < size; ++i )
			{
			if( tmp < polls_[i]->GetCount() )
				{
				regIndex_ = i;
				break;
				}
			}

		polls_[regIndex_]->epoll_.Register( fd, events, g2::EpollData( handler ) );
		}

	//-----------------------------------------------------------------------------------------//
	poll_ptr_t PollManager::GetPoll()
		{
		size_t i = __sync_fetch_and_add( &seq_, 1 ) % polls_.size();
		return polls_[i];
		}
	}
