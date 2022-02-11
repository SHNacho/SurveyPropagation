#!/bin/bash

for f in "0.04" "0.02" "0.01" "0.005" "0.0025" "0.00125"; do
	for file in data/S-4.21/*; do
		echo "Ejecutando sobre ${file} con f = $f"
		bin/main_exe $file "0.02" 
	done
done
