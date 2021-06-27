# Makefile for CSE437_HW02.cpp

CXX       := g++
CX_FLAGS  := -std=c++17
CXX_FLAGS := -lpthread
header_name := Timer.h
script_name := CSE437_HW02.cpp
out_file    := HW02_out

default: help force_look
	
operate: compile execute force_look
	
compile: 
	$(CXX) $(CX_FLAGS) $(header_name) $(script_name) $(CXX_FLAGS) -o $(out_file)

execute:
	@./$(out_file)
		
clean: force_look
	@rm $(out_file)
	
force_look : 
	@true

help:
	@echo " *** HELP *** "  
	@echo " <<< RECIPIES >>> " 
	@echo " -- help " 
	@echo " -- operate " 
	@echo " -- compile " 
	@echo " -- execute " 	
	@echo " -- clean " 
	
