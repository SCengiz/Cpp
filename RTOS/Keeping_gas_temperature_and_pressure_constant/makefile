# Makefile for CSE437_HW01.cpp

CXX       := g++
CX_FLAGS  := -std=c++17
CXX_FLAGS := -lpthread
script_name := CSE437_HW01.cpp
out_file    := HW01_out

default: help force_look
	
operate: 
	$(CXX) $(CX_FLAGS) $(script_name) $(CXX_FLAGS) -o $(out_file)
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
	@echo " -- clean " 
	
