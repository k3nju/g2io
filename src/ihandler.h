#pragma once

namespace g2io
	{
	class IFDSet;
	
	//-----------------------------------------------------------------------------------------//
	class IHandlerBase
		{
		public:
			virtual ~IHandler() = 0 {};
			virtual void Handle( int events, IFDSet *fdSet ) = 0;
		};
	}
