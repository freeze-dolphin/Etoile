release_lin: clean_lin
	mkdir build-lin64
	cd build-lin64 && cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_C_COMPILER=/usr/bin/cc \
		-DCMAKE_CXX_COMPILER=/usr/bin/c++ \
		..
	cd build-lin64 && make

release_win: clean_win
	mkdir build-win64
	cd build-win64 && cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_SYSTEM_NAME=Windows \
		-DCMAKE_C_COMPILER=/usr/bin/x86_64-w64-mingw32-gcc \
		-DCMAKE_CXX_COMPILER=/usr/bin/x86_64-w64-mingw32-g++ \
		..
	cd build-win64 && make

clean_lin:
	rm -rf ./build-lin64/

clean_win:
	rm -rf ./build-win64/

clean_all: clean_lin clean_win

all: release_lin release_win

.PHONY: release_lin release_win clean_lin clean_win clean_all all
