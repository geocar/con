#Building

The makefile assumes kdb/q is installed in `$HOME/q` and that you have
the [C bindings](http://kx.com/q/d/c.htm) installed in `$HOME/q/c`:

    q/c/k.h
    q/c/l64/c.o
    q/c/m64/c.o


#Usage

    ./con 1234

will connect to the KDB install on localhost port 1234.
