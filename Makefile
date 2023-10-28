manual:
	-mkdir ./manual/pages
	pio run -t exec -e manual -a $(CODE)
	python3 ./manual/generate_manual.py