#pragma once

#include "includes.h"
#include "g2includes.h"

namespace g2io
	{
	class Dispatcher
		{
			G2_MARK_UNCOPYABLE( Dispatcher );

		public:
			Dispatcher( uint_t threadCount );
			~Dispatcher();
			
			void Dispatch();

		private:
			//-----------------------------------------------------------------------------------------//
			class WorkerThread :public g2::Threading
				{
				public:

				private:
					virtual int Thread( void *argv );
					
					g2::Epoll epoll_;

					static const uint_t EPOLL_EVENT_SIZE = 1024;
				};
			
			typedef std::vector< WorkerThread* > thread_vector_t;
			thread_list_t threads_;
		};
	}
