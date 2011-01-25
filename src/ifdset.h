#pragma once

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	class IFDSet
		{
		public:
			virtual ~IFDSet() = 0 {};
			virtual void Register( int fd, int events );
		};
	}
