waf:
	waf

document:
	waf --docs

configure:
	waf configure

clang:
	LD_LIBRARY_PATH="/usr/lib:/usr/local/lib" CXXFLAGS="-stdlib=libc++" LINKFLAGS="-stdlib=libc++ -ldl" CXX=clang++ waf configure

clean:
	waf clean

distclean:
	waf distclean

.PHONY: waf document configure clang clean distclean
