# Makefile for ExpressionEvaluator by Hung Ngoc Phat (19120615)

ifeq ($(OS), Windows_NT)
	EXECUTABLE := MyCalc.exe 
else
	EXECUTABLE := MyCalc
endif

G++ := g++

# Subfolders
folders := InfixToPostfix PostfixEval StackFloat StackChar Utils FileRW

# Dependencies
CPP_DEPENDENCIES := $(foreach folder, $(folders), $(wildcard $(folder)/*.cpp))

# -g for debugging
$(EXECUTABLE): $(CPP_DEPENDENCIES) main.cpp
	$(G++) -g -std=c++14 $^ -o ../Release/$@ 

complex: $(CPP_DEPENDENCIES) main-adv.cpp
	$(G++) -g -std=c++14 $^ -o ../Release/c$(EXECUTABLE)
