#pragma once

#include "includes.h"
#include "g2includes.h"

namespace g2io
	{
	class Dispatcher
		{
			G2_MARK_UNCOPYABLE( Dispatcher );

		public:
			Dispatcher( uint_t threadCount, uint_t epollCount );
			~Dispatcher();
			
			void Dispatch();
		};
	}
