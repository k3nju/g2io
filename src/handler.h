#pragma once

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	class IHandlerBase
		{
		public:
			virtual ~IHandler() = 0 {};
			virtual void Handle( int events, g2::Epoll *epoll ) = 0;
		};
	}
