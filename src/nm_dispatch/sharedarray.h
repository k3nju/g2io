#pragma once

#include <cassert>
#include <stddef.h>
#include <stdint.h>

namespace g2io
	{
	template < class T >
	class SharedArray
		{
		public:
			typedef SharedArray< T > self_t;

			//-----------------------------------------------------------------------------------------//
			SharedArray( size_t size )
				:size_( size ),
				 data_( NULL )
				{
				if( size == 0 )
					{
					size_ = 1;
					}

				data_ = reinterpret_cast< data_t* >( new uint8_t[ sizeof( data_t ) + sizeof( T ) * size_ ] );
				data_->refCount = 0;
				}

			//-----------------------------------------------------------------------------------------//
			SharedArray( SharedArray &rhs )
				{
				size_ = rhs.size_;
				data_ = rhs.data_;
				__sync_add_and_fetch( &( data_->refCount ), 1 );
				}
			
			//-----------------------------------------------------------------------------------------//
			SharedArray& operator = ( SharedArray &rhs )
				{
				if( this != &rhs )
					{
					Reset();
					size_ = rhs.size_;
					data_ = rhs.data_;
					__sync_add_and_fetch( &( data_->refCount ), 1 );
					}

				return *this;
				}

			//-----------------------------------------------------------------------------------------//
			~SharedArray()
				{
				if( data_ != NULL )
					{
					Reset();
					}
				}

			//-----------------------------------------------------------------------------------------//
			T* GetRaw()
				{
				return data_->array;
				}

			//-----------------------------------------------------------------------------------------//
			size_t GetSize()
				{
				return size_;
				}
			
			//-----------------------------------------------------------------------------------------//
			T& operator[] ( size_t index )
				{
				return data_->array[ index ];
				}

			//-----------------------------------------------------------------------------------------//
			void Reset()
				{
				if( __sync_sub_and_fetch( &( data_->refCount ), 1 ) == 0 )
					{
					delete [] reinterpret_cast< uint8_t* >( data_ );
					size_ = 0;
					data_ = NULL;
					}
				}

//		private:
			typedef struct
				{
					uint16_t refCount;
					T array[1];
				} data_t;
			size_t size_;
			data_t *data_;
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__SharedArrayTest

#include <stdio.h>

struct A
	{
		~A()
			{
			puts( "~A" );
			}
	};

int GutsEntrySharedArrayTest(int,char**)
	{

	{
	g2io::SharedArray< A > array( 1 );

	{
	g2io::SharedArray< A > ref2( array );
	{
	g2io::SharedArray< A > ref3( 1 );
	puts( "copying" );
	ref3 = ref2;
	puts( "copied" );
	}
	}
	}

	puts( "end" );
	
	return 0;
	}

#endif
#endif

