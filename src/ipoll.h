#pragma once

namespace g2io
	{
	class IHandlerBase;
	
	//-----------------------------------------------------------------------------------------//
	class IPoll
		{
		public:
			virtual ~IPoll(){};
			virtual void Register( int fd, int events, IHandlerBase *handler ) = 0;
			virtual void Update( int fd, int events, IHandlerBase *handler ) = 0;
			virtual void Unregister( int fd ) = 0;
			virtual void Stop() = 0;
		};
	}
