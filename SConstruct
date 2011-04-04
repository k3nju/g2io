#! /usr/bin/python
# -*- coding:utf-8 -*-

import os;

## definitions ##
HERE = os.getcwd();
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
	"-I./src/",
	];
LIBPATH = [
	HERE + "/g2/build/",
	];
LIBS = [ "g2", "pthread" ];
TARGETS = {
	"nm" : "./src/dispatch_models/nm/",
	"parallel" : "./src/dispatch_models/parallel/",
	"mock_nm" : "./test/nm/",
	"mock_parallel" : "./test/parallel/",
	};

## defs exported ##
Export( "CXXFLAGS" );
Export( "LIBPATH" );
Export( "LIBS" );

## utility functions ##
def build_model():
	StaticLibrary( "dispatcher",
				   Glob( "*.cpp" ),
				   CXXFLAGS = CXXFLAGS );
Export( "build_model" );

def build_mock( libpath, libs ):
	Program( "a.out",
			 Glob( "*.cpp" ),
			 CXXFLAGS = CXXFLAGS,
			 LIBPATH = libpath,
			 LIBS = libs );
Export( "build_mock" );

def run_sconscript( name, src_path ):
	SConscript( dirs = src_path,
				variant_dir = "./build/" + name );

## main ##
target = ARGUMENTS.get( "target", None );

if target in TARGETS:
	run_sconscript( target, TARGETS[target] );
else:
	for i in TARGETS:
		run_sconscript( i, TARGETS[i] );

