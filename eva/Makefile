compile:
	cmake --build ../cmake-build-debug -j 6

test: compile
	cd ../cmake-build-debug && ninja test

help: compile
	../cmake-build-debug/Etoile -h --debug

simulate: compile
	../cmake-build-debug/Etoile -i /home/freeze-dolphin/Documents/workspace/idea/Etoile/eva/arcpkg/Arcthesia.AntitheEuphoriaHOLiC.arcpkg -o ./songs -b ./img/bg/1080 -p default -f --debug

simulate_multiple: compile
	../cmake-build-debug/Etoile -i ./arcpkg/ACC5.Standard.arcpkg -o ./songs -b ./img/bg/1080 -p default --force

simulate_multiple_valgrind: compile
	valgrind ../cmake-build-debug/Etoile -i ./arcpkg/ACC5.Standard.arcpkg -o ./songs -b ./img/bg/1080 -p default --force

all: compile

.PHONY: compile
