#include "dispatchworker.h"
#include "controller.h"
#include "acceptor.h"


using namespace g2io;

const size_t I = 4;

int main()
	{
	Controller c;
	DispatchWorker d[I];
	Acceptor a;
	c.epoll.Register( a.GetFD(),
					  g2::event_type::ONE_SHOT_ACCEPT,
					  EpollData( &a ) );

	for( size_t i = 0; i < I; ++i )
		{
		d[i].SetArgv( &c );
		d[i].Create();
		}
	
	for( size_t i = 0; i < I; ++i )
		{
		d[i].Join();
		}
	
	
	return 0;
	}
