#pragma once

#include "includes.h"

namespace g2io
	{
	class Dispatcher
		{
			G2_MARK_UNCOPYABLE( Dispatcher );

		public:
			Dispatcher( uint_t pollCount, uint_t threadCount );
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
			
			PollManager pollManager_;
			const uint_t threadCount_;
			thread_pool_t threadPool_;
		};
	}
