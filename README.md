qcon replacement:

* readline-inspired interface (line editing and history)
* async interaction (get a prompt immediately after doing something slow: try `\\t 10000000 (1+)/ 1` and press enter)
* send-and-continue (type an expression and press ^O instead of enter)

##Usage

    ./con 1234

will connect to the KDB install on localhost port 1234.

Press ^C to disconnect.

#Building

The makefile assumes kdb/q is installed in `$HOME/q` and that you have
the [C bindings](http://kx.com/q/d/c.htm) installed in `$HOME/q/c`:

    q/c/k.h
    q/c/l64/c.o
    q/c/m64/c.o


