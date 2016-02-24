LLVM_CONFIG=$$HOME/sources/llvm-3.7/_build/install/bin/llvm-config
IPYTHON3=ipython3
CXX=clang++-3.7

run:
	$(IPYTHON3) nbconvert Compil\ ou\ Face.ipynb --to slides --post serve

edit:
	$(IPYTHON3) notebook

clean:
	$(RM) a.s a.ll a.c  a.o  a.out  b.c  b.o  class_with_method.cpp  init_struct.c  mask.c  memcpy.c	raise.cpp

interp: interp.cpp
	$(CXX) `$(LLVM_CONFIG) --cxxflags --ldflags --libs` -rdynamic $< -o $@
