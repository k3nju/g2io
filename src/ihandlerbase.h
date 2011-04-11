#pragma once

namespace g2io
	{
	class IPoll;
	
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
			virtual enum result Handle( int events, IPoll &poll ) = 0;
		};
	}
