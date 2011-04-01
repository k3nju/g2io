#! /usr/bin/python
# -*- coding:utf-8 -*-

import os;

## definitions ##

CXXFLAGS = [
	"-fpermissive",
	"-g",
	"-Wpointer-arith",
	"-W",
	"-Wall",
	"-Wno-variadic-macros",
	"-pedantic",
	"-O2",
	"-I./g2/src/",
	];

LIBPATH = [
	"./g2/build/",
	];

LIBS = [ "g2", "pthread" ];

HERE = os.getcwd();
MODEL_SRC_PATH = HERE + "/src/dispatch_models/";
MODEL_TEST_PATH = HERE + "/test/";
ENV = Environment( CXXFLAGS = CXXFLAGS );
list_cpp = lambda d: [ d + i for i in os.listdir( d ) if i.endswith( ".cpp" ) ];
TARGETS = {
	"nm" : MODEL_SRC_PATH + "nm/",
	"parallel" : MODEL_SRC_PATH + "parallel/",
	"test_nm" : MODEL_TEST_PATH + "nm/",
	"test_parallel" : MODEL_TEST_PATH + "parallel/",
	};

## utility functions ##

def build_model( d ):
	targets = list_cpp( d );
	ENV.StaticLibrary( "dispatcher",
					   targets );
Export( "build_model" );

def build_mock( d ):
	targets = list_cpp( d );
	ENV.Program( targets,
				 LIBPATH = LIBPATH );
Export( "build_mock" );

def run_sconscript( name, path ):
	SConscript( exports=[ "path" ],
				dirs = path,
				variant_dir="./build/" + name );

## main ##

target = ARGUMENTS.get( "target", None );
if target in TARGETS:
	run_sconscript( target, TARGETS[target] );
else:
	for i in TARGETS:
		run_sconscript( i, TARGETS[i] );

