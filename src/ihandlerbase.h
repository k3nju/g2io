#pragma once

namespace g2io
	{
	class IPollRequest;
	
	//-----------------------------------------------------------------------------------------//
	class IHandlerBase
		{
		public:
			enum result
				{
				CONTINUE,
				HANDLED
				};
			
			virtual ~IHandlerBase(){};
			virtual enum result Handle( int events, IPollRequest &preq ) = 0;
		};
	}
