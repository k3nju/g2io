#pragma once

namespace g2
	{
	class Epoll;
	}

namespace g2io
	{
	class HandlerBase
		{
		public:
			virtual ~HandlerBase() = 0;
			virtual int GetFD() const = 0;
			virtual int Handle( int events , g2::Epoll *epoll ) = 0;
		};
	}
