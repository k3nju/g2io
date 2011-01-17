#pragma once

#include <sys/epoll.h>

namespace g2io
	{
	struct IRegistrar
		{
			virtual void Register( int fd, int events, epoll_data_t *data ) = 0;
		};
	}
