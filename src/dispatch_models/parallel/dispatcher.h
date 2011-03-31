#pragma once

#include "includes.h"
#include "g2includes.h"
#include "../../ipollrequest.h"
#include "../../ihandlerbase.h"

namespace g2io
	{
	class Dispatcher
		{
			G2_MARK_UNCOPYABLE( Dispatcher );

		public:
			Dispatcher( uint_t threadCount );
			~Dispatcher();

			void Register( int fd, int events, IHandlerBase *handler );
			void Dispatch();
			void Stop();

		private:
			//-----------------------------------------------------------------------------------------//
			class WorkerThread :public g2::Threading
				{
				public:
					void Register( int fd, int events, IHandlerBase *handler );
					
				private:
					virtual int Thread( void *args );
					
					g2::Epoll epoll_;
					
					static const uint_t EPOLL_EVENT_SIZE = 1024;
					friend class Poll;
				};
			typedef std::vector< WorkerThread* > threads_t;

			//-----------------------------------------------------------------------------------------//
			class Poll :public IPollRequest
				{
				public:
					Poll( Dispatcher &disp, g2::Epoll &ep );
					virtual ~Poll();
					virtual void Register( int fd, int events, IHandlerBase *handler );
					virtual void Update( int fd, int events, IHandlerBase *handler );
					virtual void Unregister( int fd );
					virtual void Stop();

				private:
					Dispatcher &disp_;
					g2::Epoll &epoll_;
				};			

			//-----------------------------------------------------------------------------------------//
			uint_t    threadCount_;
			threads_t threads_;
			bool      run_;
			uint_t    seq_;
		};
	}
