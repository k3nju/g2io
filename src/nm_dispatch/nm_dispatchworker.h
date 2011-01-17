#pragma once

#include "controller.h"
#include "g2includes.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	class Dispatch :public g2::Threading
		{
		public:
			Dispatch()
				{
				}

			void Thread( void *argv )
				{
				try
					{
					Dispatch( (Controller*)argv );
					}
				catch( Exception &E )
					{
					E.Output();
					}
				}

			void Dispatch( Controller *c )
				{
				while( true )
					{
					g2::Epoll *epoll = c->GetEpoll
					}
				}
			
		private:
			
		};
	}
