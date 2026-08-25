.PHONY: all clean

all:
	mkdir -p build && \
	cd build && \
	cmake .. && \
	$(MAKE) && \
	mv -f ecf ..
	@echo "==> Build completed successfully."

clean:
	@rm -rf build ecf
	@echo "Cleaning..."
