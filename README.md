# NES Sound Driver & Library (NSD.Lib)

## Summary

This is a sound driver for the Nintendo Entertainment System (NES) and/or compatible machine.
 This can playing the Back-Ground Music(BGM) and the Sound Effect(SE).
 This is developed as object library, and user application can control it.


## Characteristic

- The sound driver are optimized for code size and processing speed by asembly langage. Moreover, this music data is designed to get samll data and code size. So, this desine has preventive occupation of ROM area.
- C-Langage source code can coll this library functions.
    - The calling conversion of this library functions are \_\_fastcall\_\_ of `cc65'.
- MML Compiler can make the music data. Many C++ complier can build this MML Compiler.
- For the music expression, this sound driver has various music function.


## Licence

Copyright (c) 2012 S.W.
 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyrightnotice, this list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyrightnotice, this list of conditions and the following disclaimer in thedocumentation and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

