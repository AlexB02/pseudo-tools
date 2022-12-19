CPP_COMPILER = clang++
FLAGS = -std=c++20

out:
	mkdir -p out/

psemulate: out
	$(CPP_COMPILER) $(FLAGS) ../src/main/emulator/ptpemulator.cpp -o out/psemulate

pscompile: out
	cd ../ && sbt assembly -error
	sh create_pscompile_script.sh

all: psemulate pscompile

clean:
	rm -rf out/
	
.PHONY: all psemulate pscompile clean out
