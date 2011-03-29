#pragma once

#include "includes.h"
#include "../../ipollrequest.h"

namespace g2io
	{
	class PollManager;
	
	class Poll :public IPollRequest
		{
		public:
			Poll( PollManager *pollManager );
			virtual ~Poll();
			virtual void Register( int fd, int events, IHandlerBase *handler );
			virtual void Update( int fd, int events, IHandlerBase *handler );
			virtual void Unregister( int fd );
			virtual void Stop();
			inline int Select( epoll_event *events, size_t size ){ return epoll_.Select( events, size, -1 ); }

		private:
			PollManager *pollManager_;
			g2::Epoll epoll_;
		};
	}
