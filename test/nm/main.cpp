#include <stdio.h>

#include "tcpsocket.h"
#include "eventtype.h"
#include "coroutine.h"
#include "eventtype.h"
#include "../../src/ihandlerbase.h"
#include "../../src/ipollrequest.h"
#include "../../src/dispatch_models/nm/dispatcher.h"

const char RESPONSE[] =	
	"HTTP/1.1 200 OK\r\n"
	"Cache-Control: no-cache\r\n"
	"Content-Length: 1020\r\n"
	"Content-Type: text/html\r\n"
	"Last-Modified: Mon, 16 Mar 2009 20:35:26 GMT\r\n"
	"Accept-Ranges: bytes\r\n"
	"Server: Microsoft-IIS/7.5\r\n"
	"VTag: 279598141700000000\r\n"
	"X-Powered-By: ASP.NET\r\n"
	"Date: Thu, 31 Mar 2011 17:18:00 GMT\r\n"
	"Connection: keep-alive\r\n"
	"\r\n";

class Responder :public g2io::IHandlerBase
	{
	public:
		Responder( int sock )
			:sock_( sock ),
			 coro_( 0 )
			{
			}

		virtual ~Responder()
			{
			}

		virtual IHandlerBase::result Handle( int events, g2io::IPollRequest &poll )
			{
			CORO( coro_ )
				{
				
				{
				char buf[1024];
				sock_.Receive( buf, sizeof( buf ) );
				poll.Update( sock_.GetSocket(), g2::event_type::ONE_SHOT_WRITE, this );
				YIELD return CONTINUE;
				}

				{
				sock_.Send( RESPONSE, sizeof( RESPONSE ) );
				poll.Update( sock_.GetSocket(), g2::event_type::ONE_SHOT_READ, this );
				}
				
				}

			return HANDLED;
			}

	private:
		g2::TCPSocket sock_;
		int coro_;
	};

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

		virtual IHandlerBase::result Handle( int events, g2io::IPollRequest &poll )
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
				
				Responder *handler = new Responder( csock );
				poll.Register( csock, g2::event_type::ONE_SHOT_READ, handler );					
				}
				
			return CONTINUE;
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
	g2io::Dispatcher disp( 1, 3 );
	Acceptor a;
	disp.Register( a.GetSocket(),
				   g2::event_type::ONE_SHOT_ACCEPT,
				   &a );
	disp.Dispatch();
	
	return 0;
	}
