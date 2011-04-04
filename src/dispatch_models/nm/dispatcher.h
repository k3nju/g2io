#pragma once

#include "includes.h"
#include "pollmanager.h"
#include "ihandlerbase.h"

namespace g2io
	{
	class Dispatcher
		{
			G2_MARK_UNCOPYABLE( Dispatcher );

		public:
			Dispatcher( size_t pollCount, size_t threadCount );
			~Dispatcher();

			void Register( int fd, int events, IHandlerBase *handler );
			void Dispatch();
			void Stop();

		private:
			class Worker :public g2::Threading
				{
				private:
					virtual int Thread( void *args );
				};

			typedef g2::ThreadPool< Worker > thread_pool_t;

			const size_t pollCount_;
			const size_t threadCount_;
			PollManager pollManager_;
			thread_pool_t threadPool_;
		};
	}
