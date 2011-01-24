#pragma once

namespace g2io
	{
	//-----------------------------------------------------------------------------------------//
	class Poll
		{
		public:
			virtual ~Poll() = 0 {};
			virtual void Register( int fd, int events );
		};
	}
