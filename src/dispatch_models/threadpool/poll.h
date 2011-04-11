#pragma once

#include "includes.h"
#include "ipoll.h"

namespace g2
	{
	class ThreadPool;
	}

namespace g2io
	{
	class PollManager;
	
	class Poll :public IPoll
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
			inline void SetThreadPool( g2::ThreadPool *threadPool ){ threadPool_ = threadPool; }

			inline bool IsPolling() const { return isPolling_; }
			inline size_t GetCount() const { return count_; }
			
		private:
			void RegisterImpl( int fd, int events, IHandlerBase *handler );
			void StopPolling();
			
			PollManager *pollManager_;
			g2::ThreadPool *threadPool_;
			g2::Epoll epoll_;
			size_t count_;
			bool isPolling_;
			g2::EventFD stopEvent_;
			
			friend class PollManager;
		};

	typedef boost::shared_ptr< Poll > poll_ptr_t;
	}
