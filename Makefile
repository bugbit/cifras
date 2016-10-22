.PHONY: clean All

All:
	@echo "----------Building project:[ cifras - Debug ]----------"
	@"$(MAKE)" -f  "cifras.mk"
clean:
	@echo "----------Cleaning project:[ cifras - Debug ]----------"
	@"$(MAKE)" -f  "cifras.mk" clean
