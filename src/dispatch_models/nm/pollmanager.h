#pragma once

#include "includes.h"
#include "../../ipollrequest.h"

namespace g2io
	{
	class PollManager
		{
		public:
			Poll( size_t count );
			
			epoll_ptr_t Get();
			
		private:
			size_t seq_;
			epoll_vector_t epolls_;
		};
	}
