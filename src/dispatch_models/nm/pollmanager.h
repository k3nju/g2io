#pragma once

#include "includes.h"
#include "poll.h"
#include "ipollrequest.h"

namespace g2io
	{
	typedef std::pair< int, poll_ptr_t > select_result_t;
	
	class PollManager
		{
		public:
			PollManager();
			
			void CreatePolls( size_t count, g2::IThreadPool *threadPool );
			void Register( int fd, int events, IHandlerBase *handler );
			select_result_t Select( struct epoll_event *events, size_t size );
			poll_ptr_t GetPoll();
			
		private:
			size_t seq_;
			size_t regIndex_;
			bool isAllPolling_;
			g2::SignalEvent pollWait_;
			
			typedef std::vector< poll_ptr_t > poll_vector_t;
			poll_vector_t polls_;
		};
	}
