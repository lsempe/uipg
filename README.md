UIPG - User Interface Programming for Games
====
<p>
Companion code for the book: User Interface Programming for Games
<p>
In the source code you will find three main sample projects:
<p>
sample.hud
sample.controls
sample.menu3d
<p>
Set any of these as the startup project then build and run with Visual Studio.
<p>
The code is organized into static libraries divided by common functionality:
<p>
<b>engine.core</b> - Holds the core functions and helper classes that may be used by any of the other libraries.</br>
<b>engine.math</b> - Holds the math library, vectors, matrices, inline functions, constant expressions, etc.</br>
<b>engine.render</b> - Holds the rendering device implementation, currently it is tightly bound to Direct3D.</br>
<b>engine.localization</b> - The localization framework as described in the book.</br>
<b>engine.ui</b> - The top level User Interface functionality that any UI system could need.</br>
<b>unittests</b> - No serious projects should neglect their unit tests, we use Google's test framework here.</br>
<p>
The example code uses a simple utility in C# called content.installer, the purpose of this utility is to run as a post-build step and copy the source content into the given sample's binary directory. 
<p>
The example code uses two third party libraries, DirectXTK and DirectXTex.
<p>
Currently all projects are provided using Visual Studio 2013 solution files. I can provide other versions if anyone needs them, just send me an email.
<p>
Nov 1, 2014 - The code has been pushed! I hope you find it useful, I will continue updating it I find bugs or receive bug reports.
<p>
Oct 23, 2014 - The book is finally published! I'm preparing to commit the code, I am just going over it one last time to make sure I don't forget anything. 
<p>
Thanks!
<p>
- LS
