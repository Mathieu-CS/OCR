all:
	+$(MAKE) -C image_preprocessing
	+$(MAKE) -C solver
	+$(MAKE) -C xor
	+$(MAKE) -C split

clean:
	+$(MAKE) -C image_preprocessing clean
	+$(MAKE) -C solver clean
	+$(MAKE) -C xor clean
	+$(MAKE) -C split clean
