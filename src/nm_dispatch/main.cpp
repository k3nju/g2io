#include <stdio.h>

//-----------------------------------------------------------------------------------------//
class EpollManager
	{
	public:
		EpollManager( size_t size )
			:seq_( 0 ),
			 size_( size ),
			 epollEntries_( NULL )
			{
			assert( size_ != 0 );
			epollEntries_ = new g2::Epoll[ size_ ];
			for( size_t i = 0; i < size_; ++i )
				{
				epollEntries_[i].isUsed = false;
				}
			}
		
		~EpollManager()
			{
			delete [] epolls_;
			}
		
		int Select( epoll_event *events, size_t size, g2::Epoll &*epoll )
			{
			for( size_t i = 0; i < size_; ++i )
				{
				size_t i = __sync_fetch_and_add( &seq_, 1 ) % size_;
				EpollEntry *e = epollEntries_ + i;
				if( __sync_bool_compare_and_swap( &( e->isUsed ), false,true ) )
					{
					epoll = &( e->epoll );
					int count = e->epoll.Select( events, size, -1 );
					__sync_bool_compare_and_swap( &( e->isUsed ), true, false );
					
					return count;
					}
				}

			return -1;
			}
		
	private:
		struct EpollEntry
			{
				g2::Epoll epoll;
				bool isUsed;
			};
		
		size_t size_;
		size_t seq_;
		EpollEntry *epollEntries_;
	};

//-----------------------------------------------------------------------------------------//
class DispatchWorker :public g2::Threading
	{
	public:
		
	private:
		virtual int Thread( void *argv )
			{
			Component *comp = (Component*)argv;
			EpollManager *epollMngr = NULL;
			
			while( true )
				{
				g2::Epoll *epoll = NULL;
				epoll_event_array_t events( new epoll_event[1024] );
				int count = epollMngr->Select( events.get(), 1024, epoll );
				if( count > 0 )
					{
					Distribute( events, count, epoll );
					}
				else
					{
					Task task;
					
					}
				}
			
			return 0;
			}
	};

int main()
	{

	return 0;
	}
