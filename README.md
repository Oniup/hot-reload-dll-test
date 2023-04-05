# Hot Reload DLL Test

this is a test where it loads a shared library at runtime. The application is 
able to detect whether the library has been recompiled, in which it will reload
the library again

# Build

Library uses cmake, compile without changing cmake file first. The while the
executable is running, change the dll's cpp file to print something else and 
recompile it. The app will be able to detect the change and update