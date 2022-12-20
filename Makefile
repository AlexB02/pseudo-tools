CPP_COMPILER = clang++
FLAGS = -std=c++20
BUILD_DIR = build/out

out:
	mkdir -p $(BUILD_DIR)

psemulate: out
	$(CPP_COMPILER) $(FLAGS) src/main/emulator/ptpemulator.cpp -o $(BUILD_DIR)/psemulate

psemulate-gdb: out
	$(CPP_COMPILER) $(FLAGS) src/main/emulator/ptpemulator.cpp -o $(BUILD_DIR)/psemulate -g

pscompile: out
	sbt assembly -error
	sh build/create_pscompile_script.sh

all: psemulate pscompile

clean:
	rm -rf $(BUILD_DIR)
	
.PHONY: all psemulate pscompile clean out
