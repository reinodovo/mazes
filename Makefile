pdf:
	pio run -t exec -e native -a $(SEED)
	defusal manual.md -n Mazes -o manual.pdf -v $(SEED)