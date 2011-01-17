#pragma once

#include "g2includes.h"
#include "handlerbase.h"
#include "client.h"

using namespace g2io;

class Acceptor :public HandlerBase
	{
	public:
		Acceptor()
			{
			sock_.EnableReuseAddr( true );
			sock_.Bind( "localhost", 56001 );
			sock_.Listen( 0xffff );
			sock_.EnableNonBlocking( true );
			}

		virtual ~Acceptor()
			{
			try
				{
				sock_.Close();
				}
			catch( ... )
				{
				}
			}

		virtual int GetFD() const
			{
			return sock_.GetSocket();
			}

		virtual int Handle( int events, g2::Epoll *e )
			{
			if( events & g2::event_type::ONE_SHOT_ACCEPT )
				{
				while( true )
					{
					int csock = sock_.Accept();
					if( csock == -1 && errno == EAGAIN )
						{
						e->Update( sock_.GetSocket(),
								   g2::event_type::ONE_SHOT_ACCEPT,
								   g2::EpollData( this ) );
						break;
						}
					
					Client *c = new Client( csock );
					e->Register( csock, g2::event_type::ONE_SHOT_READ, EpollData( c ) );
					}
				}

			return 0;
			}

	private:
		TCPSocket sock_;
	};
