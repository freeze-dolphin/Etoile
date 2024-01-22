release_lin: clean
	mkdir build
	cd build && RELEASE_MODE=1 cmake -DCMAKE_BUILD_TYPE=Release ..
	cd build && make

release_win: clean
	mkdir build
	cd build && RELEASE_MODE=1 XC_WIN=1 cmake -DCMAKE_BUILD_TYPE=Release ..
	cd build && make

clean:
	rm -rf ./build/

.PHONY: release_lin release_win clean
