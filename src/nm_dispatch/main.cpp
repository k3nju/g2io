#include <assert>
#include <boost/shared_array.hpp>

using namespace g2io;

const size_t THREAD_COUNT = 1;
const size_t EPOLL_COUNT  = 2;

int main()
	{
	Dispatcher d( THREAD_COUNT, EPOLL_COUNT );
	d.Run();
	
	return 0;
	}


//-----------------------------------------------------------------------------------------//
template < class Worker >
class Dispatcher
	{
	public:
		//-----------------------------------------------------------------------------------------//
		Dispatcher( size_t threadCount,
					size_t epollCount )
			:epollFds_(),
			 workers_(),
			 threadCount_( threadCount ),
			 epollCount_( epollCount )
			{
			assert( epollCount_ != 0 );
			assert( threadCount_ != 0 );

			workers_.reset( new Worker[ threadCount_ ] );
			epollFds_.reset( new int[ epollCount_ ] );
			}
		
	private:
		boost::shared_array< Worker* > workers_;
		boost::shared_array< int > epollFds_;
		size_t threadCount_;
		size_t epollCount_;
	};
