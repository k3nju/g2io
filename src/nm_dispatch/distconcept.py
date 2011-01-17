#! /usr/bin/python


def Test( socks, threads ):
	assert( threads > 1 );
	n = socks / threads;
	
	if n == 0:
		for i in xrange( 0, socks ):
			print i;
	else:
		for i in xrange( 0, threads - 1 ):
			print n;
			socks -= n;
		print socks;
	

if __name__ == "__main__":
	n = [
		( 1, 2 ),
		( 1, 3 ),
		( 4, 2 ),
		( 6, 2 ),
		( 7, 2 ),
		( 100, 3 ),
		];
	
	for i in n:
		print i;
		Test( i[0], i[1] );
