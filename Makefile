all:
	+$(MAKE) -C image_preprocessing
	+$(MAKE) -C solver
	+$(MAKE) -C xor
