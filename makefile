s=$(shell uname -s | tr A-Z a-z)

con: con.$(s)
	cp $< $@.tmp
	chmod +x $@.tmp
	mv $@.tmp $@

con.darwin: con.c
	gcc-6 -O3 -s -DKXVER=3 -I$(HOME)/q/c -o $@ $< $(HOME)/q/c/m64/c.o 

con.linux: con.c
	gcc -O3 -s -DKXVER=3 -I$(HOME)/q/c -o $@ $< $(HOME)/q/c/l64/c.o -lpthread

