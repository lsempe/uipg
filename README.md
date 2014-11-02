UIPG - User Interface Programming for Games
====

Companion code for the book: User Interface Programming for Games

In the source code you will find three main sample projects:

sample.hud
sample.controls
sample.menu3d

Set any of these as the startup project then build and run with Visual Studio.

The code is organized into static libraries divided by common functionality:

engine.core - Holds the core functions and helper classes that may be used by any of the other libraries.
engine.math - Holds the math library, vectors, matrices, inline functions, constant expressions, etc.
engine.render - Holds the rendering device implementation, currently it is tightly bound to Direct3D.
engine.localization - The localization framework as described in the book.
engine.ui - The top level User Interface functionality that any UI system could need.
unittests - No serious projects should neglect their unit tests, we use Google's test framework here.

The example code uses a simple utility in C# called content.installer, the purpose of this utility is to run as a post-build step and copy the source content into the given sample's binary directory. 

The example code uses two third party libraries, DirectXTK and DirectXTex.

Currently all projects are provided using Visual Studio 2013 solution files. I can provide other versions if anyone needs them, just send me an email.

Nov 1, 2014 - The code has been pushed! I hope you find it useful, I will continue updating it I find bugs or receive bug reports.

Oct 23, 2014 - The book is finally published! I'm preparing to commit the code, I am just going over it one last time to make sure I don't forget anything. 

Thanks!

- LS
