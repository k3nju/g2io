#include "dispatchworker.h"
#include "config.h"
#include "handlerbase.h"
#include "epolldata.h"

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	int DispatchWorker::Thread( void* )
		{
		try
			{
			while( true )
				{
				epoll_event actives[EPOLL_EVENT_SIZE];
				int activesCount = epoll_.Select( actives, EPOLL_EVENT_SIZE, -1 );
				
				for( int i = 0; i < activesCount; ++i )
					{
					int events = actives[i].events;
					HandlerBase *handler = (HandlerBase*)( actives[i].data.ptr );
					if( handler->Handle( events, &epoll_ ) == -1 )
						{
						delete handler;
						}
					}
				}
			}
		catch( Exception &E )
			{
			E.Output();
			}

		return 0;
		}
	}
