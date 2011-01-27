#include <stdio.h>

#include "tcpsocket.h"
#include "eventtype.h"
#include "../../src/ihandlerbase.h"
#include "../../src/ipoll.h"
#include "../../src/dispatch_models/parallel/dispatcher.h"

class Acceptor :public g2io::IHandlerBase
	{
	public:
		Acceptor()
			:sock_()
			{
			sock_.EnableReuseAddr( true );
			sock_.Bind( "localhost", 56000 );
			sock_.EnableNonBlocking( true );
			sock_.Listen( 0xffff );
			}
		
		virtual ~Acceptor()
			{
			}

		virtual void Handle( int events, g2io::IPoll &poll )
			{
			while( true )
				{
				int csock = sock_.Accept();
				if( csock == -1 && errno == EAGAIN )
					{
					poll.Update( sock_.GetSocket(),
								 g2::event_type::ONE_SHOT_ACCEPT,
								 this );
					break;
					}

				printf( "%d\n", csock );
				}
			}

		int GetSocket()
			{
			return sock_.GetSocket();
			}

	private:
		g2::TCPSocket sock_;
	};


int main()
	{
	g2io::Dispatcher disp( 1 );
	Acceptor a;
	disp.Register( a.GetSocket(),
				   g2::event_type::ONE_SHOT_ACCEPT,
				   &a );

	disp.Dispatch();
	
	return 0;
	}
