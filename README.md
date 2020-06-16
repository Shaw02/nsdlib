# NES Sound Driver & Library (NSD.Lib)

This is a sound driver for the Nintendo Entertainment System (NES) and other compatible machines.
You can use it to play Background Music (BGM) and Sound Effects (SE) in your game projects and demos.

## Features

- This sound driver was developed with CPU load and code size in mind. The sequence data is optimized to take up as little ROM space as possible.
- The library functions can be called from both Assembly and C.
  - See cc65's [\_\_fastcall\_\_](https://github.com/cc65/wiki/wiki/Parameter-passing-and-calling-conventions#The_fastcall_calling_convention) calling convention.
- A MML Compiler, written in C++, converts MML files into sequence data the driver can use. It can output in assembly language as well as the [NSF music format](http://www.vgmpf.com/Wiki/index.php?title=NSF).
- This driver has various effects and commands to allow for ample musical expressiveness.

## Basic Syntax

- General MML commands are supported

  - [Tempo (t)](http://shaw.la.coocan.jp/nsdl/doc/mml_tempo.html)
  - [Notes (abcdefg)](http://shaw.la.coocan.jp/nsdl/doc/mml_note.html)
  - [Rest (r)](http://shaw.la.coocan.jp/nsdl/doc/mml_rest.html)
  - [Tie (^)](http://shaw.la.coocan.jp/nsdl/doc/mml_tie.html)
  - [Volume (v)](http://shaw.la.coocan.jp/nsdl/doc/mml_volume.html)
  - [Note Length (l)](http://shaw.la.coocan.jp/nsdl/doc/mml_length.html) (based on ticks, not video frames)
  - [Octave (o)](http://shaw.la.coocan.jp/nsdl/doc/mml_octave.html)
  - [Quantize (q)](http://shaw.la.coocan.jp/nsdl/doc/mml_gate.html)
  - [Loop (L)](http://shaw.la.coocan.jp/nsdl/doc/mml_loop.html)
  - [Repeat ( [,])](http://shaw.la.coocan.jp/nsdl/doc/mml_repeatA.html)
  - etc.

- [Volume Envelope](http://shaw.la.coocan.jp/nsdl/doc/mml_envelop.html)
- [Portamento](http://shaw.la.coocan.jp/nsdl/doc/mml_portamento.html)
- [Subroutines](http://shaw.la.coocan.jp/nsdl/doc/mml_sub.html)

For further documentation, explore the site all of these links lead to.

## Application Manual

- [Wiki Home](https://github.com/Shaw02/nsdlib/wiki)
  - [Developers Guide](https://github.com/Shaw02/nsdlib/wiki/DevelopersGuide)

## License

This project is licensed under the BSD 2-clause "Simplified" license - see the [LICENSE.md](LICENSE.md) file for details.  
Copyright &copy; 2012, S.W. All rights reserved.
