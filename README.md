# Modern Computer Architecture

Contains code during the lab and the latex for the lab report. 

## ./

The root directory contains a Makefile that can compile simple test programs to be run on linux machines which means also the Microblaze. You do have to compile using the sb2 environment if your going Microblazin. 

## bytecodes/

Contains src but not the utilities to compile bytecode/data for the rvex VLIW co processor. 
The bytecodes are converted with bin2c to c code. 

## bin2c/

Small simple binary to c code converter.
Allows us to include the kernel bytecode directly into applications using the rvex. 

## documents/

Latex of the lab report. 

## rvex/

The rvex interface. Could have been compiled to a static library but we did not feel the need to. Including the c file is just as easy. 
Modifying it may break code that depends on it which is why there are other copies in x264/rvex/. 

## x264/

Video conversion software modded to use a kernel for the pixel_satd_8x4_something function. 
Also runtime measuring functionality was added because the Microblaze doesn't support profiling with gprof. 
Multiple precompiled binaries can be found within. 

## src/

Some (maybe outdated) test programs that might us the rvex interface or not. pixel.h supplies some x264 data types and macros. 
