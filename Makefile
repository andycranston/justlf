justlf: justlf.c
	gcc -o justlf justlf.c

userinstall:
	cp -p justlf      $(HOME)/bin/justlf
	chmod u=rwx,go=rx $(HOME)/bin/justlf

rootinstall:
	cp -p justlf      /usr/local/bin/justlf
	chmod u=rwx,go=rx /usr/local/bin/justlf
