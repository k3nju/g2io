#pragma once

#include "includes.h"
#include "ipollrequest.h"

namespace g2io
	{
	class PollManager;
	
	class Poll :public IPollRequest
		{
		public:
			Poll();
			virtual ~Poll();
			virtual void Register( int fd, int events, IHandlerBase *handler );
			virtual void Update( int fd, int events, IHandlerBase *handler );
			virtual void Unregister( int fd );
			virtual void Stop();
			int Select( epoll_event *events, size_t size );

			inline void SetPollManager( PollManager *pollManager ){ pollManager_ = pollManager; }
			inline void SetThreadPool( g2::IThreadPool *threadPool ){ threadPool_ = threadPool; }

			inline size_t GetCount() const { return count_; }
			
		private:
			void RegisterImpl( int fd, int events, IHandlerBase *handler );
			
			PollManager *pollManager_;
			g2::IThreadPool *threadPool_;
			g2::Epoll epoll_;
			size_t count_;
			bool isPolling_;

			friend class PollManager;
		};

	typedef boost::shared_ptr< Poll > poll_ptr_t;
	}
