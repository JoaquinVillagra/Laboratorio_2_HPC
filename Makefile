default:
	gcc wave.c -o wave -fopenmp
mac:
	gcc-7 wave.c -o wave -fopenmp