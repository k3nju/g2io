#pragma once

#include "g2includes.h"
#include "handlerbase.h"

using namespace g2;
using namespace g2io;

static const char RES[] = "HTTP/1.1 200 OK\r\n"
	"Cache-Control: no-cache\r\n"
	"Content-Length: 1020\r\n"
	"Content-Type: text/html\r\n"
	"Last-Modified: Mon, 16 Mar 2009 20:35:26 GMT\r\n"
	"Accept-Ranges: bytes\r\n"
	"Server: Microsoft-IIS/7.5\r\n"
	"VTag: 438357341900000000\r\n"
	"X-Powered-By: ASP.NET\r\n"
	"Date: Sun, 21 Nov 2010 08:33:14 GMT\r\n"
	"Connection: keep-alive\r\n\r\n";
size_t RES_SIZE = sizeof( RES ) - 1;

class Client :public g2io::HandlerBase
	{
	public:
		Client( int sock )
			{
			sock_.SetSocket( sock );
			sock_.EnableNonBlocking( true );
			}

		virtual ~Client()
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
			int next = 0;
			
			if( events & g2::event_type::ONE_SHOT_ACCEPT )
				{
				while( true )
					{
					rbuf_.EnsureCapacity( 1024 );
					ssize_t read = sock_.Receive( (char*)( rbuf_.GetEnd() ), rbuf_.GetWritableSize() );
					if( read == -1 && errno == EAGAIN )
						{
						next |= g2::event_type::ONE_SHOT_READ;
						break;
						}
					else if( read == 0 )
						{
						sock_.Close();
						return -1;
						}
					else
						{
						rbuf_.AddWriteCompletionSize( read );
						}
					}
				
				wbuf_.Write( RES, RES_SIZE );
				events |= event_type::ONE_SHOT_WRITE;
				}
			if( events & event_type::ONE_SHOT_WRITE )
				{
				while( wbuf_.GetReadableSize() > 0 )
					{
					ssize_t written = sock_.Send( (const char*)( wbuf_.GetBegin() ),
												  wbuf_.GetReadableSize() );
					if( written == -1 )
						{
						if( errno == EAGAIN && wbuf_.GetReadableSize() > 0 )
							{
							break;
							}
						else
							{
							sock_.Close();
							return -1;
							}
						}
					else
						{
						wbuf_.AddReadCompletionSize( written );
						if( wbuf_.GetReadableSize() == 0 )
							{
							sock_.Close();
							return -1;
							}
						}
					}
				}

			if( next != 0 )
				{
				e->Update( sock_.GetSocket(), next, EpollData( this ) );
				}
			
			return 0;
			}
		
	private:
		TCPSocket sock_;
		Buffer rbuf_;
		Buffer wbuf_;
	};
