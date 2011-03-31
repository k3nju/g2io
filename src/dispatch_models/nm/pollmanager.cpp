#include "pollmanager.h"
#include "poll.h"
#include <assert.h>

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	PollManager::PollManager()
		:seq_( 0 ),
		 regIndex_( 0 ),
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
		poll_vector_t::iterator head = polls_.begin();
		poll_vector_t::iterator foot = polls_.end();
		poll_vector_t::iterator iter = head;
		size_t size = (*head)->GetCount();
		
		for( ++head; head != foot; ++head )
			{
			size_t cur = (*head)->GetCount();
			if( cur < size )
				{
				iter = head;
				size = cur;
				}
			}

		(*iter)->RegisterImpl( fd, events, handler );
		}

	//-----------------------------------------------------------------------------------------//
	poll_ptr_t PollManager::GetPoll()
		{
		size_t i = __sync_fetch_and_add( &seq_, 1 ) % polls_.size();
		return polls_[i];
		}
	}
