run:
	ipython3 nbconvert Compil\ ou\ Face.ipynb --to slides --post serve

edit:
	ipython3 notebook

clean:
	$(RM) *.c *.o *.s *.ll a.out *.cpp
