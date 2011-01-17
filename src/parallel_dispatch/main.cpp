#include "dispatchworker.h"
#include "handlerbase.h"
#include "g2includes.h"

using namespace g2io;
using namespace g2;

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
unsigned int I = 0;

struct Client : HandlerBase
	{
		TCPSocket sock_;
		Buffer rbuf_;
		Buffer wbuf_;
		
		Client( int csock )
			:sock_()
			{
			sock_.SetSocket( csock );
			sock_.EnableNonBlocking( true );
			}

		~Client()
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

		virtual int Handle( int e, Epoll *epoll )
			{
			try
				{
				if( e & event_type::ONE_SHOT_READ )
					{
					while( true )
						{
						rbuf_.EnsureCapacity( 1024 );
						ssize_t read = sock_.Receive( (char*)( rbuf_.GetEnd() ), rbuf_.GetWritableSize() );
						if( read == -1 && errno == EAGAIN )
							{
							epoll->Update( sock_.GetSocket(),
										   event_type::ONE_SHOT_READ,
										   EpollData( this ) );
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
					e |= event_type::ONE_SHOT_WRITE;
					epoll->Update( sock_.GetSocket(),
								   e,
								   EpollData( this ) );
					}
				
				if( e & event_type::ONE_SHOT_WRITE )
					{
					while( wbuf_.GetReadableSize() > 0 )
						{
						ssize_t written = sock_.Send( (const char*)( wbuf_.GetBegin() ),
													  wbuf_.GetReadableSize() );
						if( written == -1 )
							{
							if( errno == EAGAIN && wbuf_.GetReadableSize() > 0 )
								{
								epoll->Update( sock_.GetSocket(),
											   event_type::ONE_SHOT_WRITE,
											   EpollData( this ) );
								break;
								}
							else
								{
								sock_.Close();
								return -1;
								}
							
							break;
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
				}
			catch( Exception &E )
				{
				E.Output();
				}

			return 0;
			}
	};


struct Acceptor : HandlerBase
	{
		TCPSocket sock_;
		IRegistrar *reg_;
		
		Acceptor( IRegistrar &r )
			:sock_(),
			 reg_( &r )
			{
			sock_.EnableReuseAddr( true );
			sock_.Bind( "0.0.0.0", 56000 );
			sock_.Listen( 0xfff );
			sock_.EnableNonBlocking( true );
			}

		~Acceptor()
			{
			}
		
		virtual int GetFD() const
			{
			return sock_.GetSocket();
			}
		
		virtual int Handle( int e, Epoll *epoll )
			{
			try
				{
				if( e & event_type::ONE_SHOT_ACCEPT )
					{
					while( true )
						{
						int csock = sock_.Accept();
						if( csock == -1 && errno == EAGAIN )
							{							
							epoll->Update( sock_.GetSocket(),
										   event_type::ONE_SHOT_ACCEPT,
										   EpollData( this ) );
							break;
							}

						Client *c = new Client( csock );
						reg_->Register( csock, event_type::ONE_SHOT_READ, EpollData( c ) );
						}
					}
				}
			catch( Exception &E )
				{
				E.Output();

				return -1;
				}
			
			return 0;
			}
	};

int main()
	{
	try
		{
		IODispatch< DispatchWorker, 4 > disp;
		Acceptor *a = new Acceptor( disp );
		disp.Register( a->GetFD(), event_type::ONE_SHOT_ACCEPT, EpollData( a ) );
		disp.Dispatch();
		}
	catch( Exception &E )
		{
		E.Output();
		}
	
	return 0;
	}
