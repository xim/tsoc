.phony: $(MAKECMDGOALS) default_target

$(MAKECMDGOALS):
	./build $(MAKECMDGOALS)

default_target:
	./build all
