#pragma once

#include "includes.h"
#include "poll.h"
#include "../../ipollrequest.h"

namespace g2io
	{
	class PollManager
		{
		public:
			PollManager();
			
			void CreatePolls( size_t count, g2::IThreadPool *threadPool );
			void Register( int fd, int events, IHandlerBase *handler );
			poll_ptr_t GetPoll();
			
		private:
			size_t seq_;
			size_t regIndex_;
			g2::MutexLock regIndexLock_;
			
			typedef std::vector< poll_ptr_t > poll_vector_t;
			poll_vector_t polls_;
		};
	}
