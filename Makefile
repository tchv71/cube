.PHONY: clean All

All:
	@echo "----------Building project:[ cube - Debug ]----------"
	@"$(MAKE)" -f  "cube.mk"
clean:
	@echo "----------Cleaning project:[ cube - Debug ]----------"
	@"$(MAKE)" -f  "cube.mk" clean
