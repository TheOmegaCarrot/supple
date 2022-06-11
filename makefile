PROJNAME=utils
STD=-std=c++17
OPT=-Og
WARN=-Wall -Wextra -pedantic -Wcast-align=strict -Wcast-qual -Wlogical-op -Wredundant-decls -Wdouble-promotion -Wformat=2 -Wnull-dereference -Wimplicit-fallthrough -Wuninitialized -Wstrict-overflow=4 -Wold-style-cast -Wconversion -Wduplicated-branches -Wduplicated-cond -Wtrampolines -Wfloat-equal -Wundef -Wshadow -Wshadow=local -Wfree-nonheap-object -Wsign-conversion -Wno-multichar -Wstrict-null-sentinel -Wctor-dtor-privacy -Wsign-promo -Wmisleading-indentation -Wzero-as-null-pointer-constant -Wuseless-cast -Wsuggest-override -Weffc++ -Wno-error=effc++ -Werror
CXX=g++
ADDFLAGS=-ggdb3
CXXFLAGS=$(STD) $(OPT) $(ADDFLAGS) $(WARN)
TSTARGS=
EXE=bin/$(PROJNAME)

.PHONY: default
default: exe test

.PHONY: exe
exe: bin obj $(EXE)

.PHONY: test
test: tst/bin/test
	@echo ; echo
	@printf "\033[1;32mTest Results:\033[1;0m\n\n"
	@./run_tests $(TSTARGS)

.PHONY: tags
tags:
	@printf "\033[1;32mGenerating ctags\033[1;0m\n"
	@ctags -R . >/dev/null 2>&1

.PHONY: doc
doc:
	@printf "\033[1;32mGenerating docs\033[1;0m\n"
	@doxygen >/dev/null 2>&1

.PHONY: docopen
docopen: doc
	@firefox -new-window doc/html/index.html >/dev/null 2>&1 & disown

.PHONY: all
all: tags exe assembly preprocess doc test

.PHONY: assembly
assembly: asm asm/main.s

.PHONY: preprocess
preprocess: pre pre/main.ii

asm:
	@mkdir -p asm

pre:
	@mkdir -p pre

bin:
	@mkdir -p bin

obj:
	@mkdir -p obj

tst/bin:
	@mkdir -p tst/bin

tst/obj:
	@mkdir -p tst/obj

.PHONY: clean
clean:
	@printf "\n\t\033[1;32mCLEAN\033[1;0m\n"
	@rm -f obj/* bin/* tst/obj/* tst/bin/*
	@rm -rf asm pre doc

$(EXE): obj/main.o
	@printf "\033[1;32mLinking\t\t $(EXE)\033[1;0m\n"
	@$(CXX) -o $@ $^ $(CXXFLAGS)
	@ln -sf $(EXE) ./run

tst/bin/test: tst/bin tst/obj tst/obj/test_lazy.o tst/obj/test_utils.o tst/obj/test_algorithm.o tst/obj/maintest.o obj/main.o
	@printf "\033[1;32mLinking\t\t tst/bin/test\033[1;0m\n"
	@rm -f tst/obj/main.o
	@$(CXX) -o $@ tst/obj/*.o $(CXXFLAGS)
	@ln -sf tst/bin/test ./run_tests

obj/main.o: src/main.cpp src/main.cpp
	@printf "\033[1;32mBuilding object\t $@\033[1;0m\n"
	@$(CXX) -c -o $@ $< -Iinc $(CXXFLAGS)
	@ln -sf ../../obj/main.o tst/obj/main.o

asm/main.s: src/main.cpp src/main.cpp
	@printf "\033[1;32mGenerating\t $@\033[1;0m\n"
	@$(CXX) -S -o $@ $< -Iinc $(CXXFLAGS)

pre/main.ii: src/main.cpp src/main.cpp
	@printf "\033[1;32mPreprocessing\t $@\033[1;0m\n"
	@$(CXX) -E -o $@ $< -Iinc $(CXXFLAGS)

tst/obj/test_lazy.o: tst/src/test_lazy.cpp 
	@printf "\033[1;32mBuilding object\t $@\033[1;0m\n"
	@$(CXX) -c -o $@ $< -Iinc -Itst/inc $(CXXFLAGS)

tst/obj/test_utils.o: tst/src/test_utils.cpp inc/utils/term_colors.h tst/inc/test_utils.h
	@printf "\033[1;32mBuilding object\t $@\033[1;0m\n"
	@$(CXX) -c -o $@ $< -Iinc -Itst/inc $(CXXFLAGS)

tst/obj/test_algorithm.o: tst/src/test_algorithm.cpp tst/inc/test_algorithm.h inc/utils/algorithm.hpp tst/inc/test_utils.h
	@printf "\033[1;32mBuilding object\t $@\033[1;0m\n"
	@$(CXX) -c -o $@ $< -Iinc -Itst/inc $(CXXFLAGS)

tst/obj/maintest.o: tst/src/maintest.cpp 
	@printf "\033[1;32mBuilding object\t $@\033[1;0m\n"
	@$(CXX) -c -o $@ $< -Iinc -Itst/inc $(CXXFLAGS)
