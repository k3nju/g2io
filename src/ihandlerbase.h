#pragma once

namespace g2io
	{
	class IPoll;
	
	//-----------------------------------------------------------------------------------------//
	class IHandlerBase
		{
		public:
			virtual ~IHandlerBase(){};
			virtual void Handle( int events, IPoll &poll ) = 0;
		};
	}
