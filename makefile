# sudo apt install gcc-ia16-elf binutils-ia16-elf
# sudo apt install gcc-ia16-elf g++-ia16-elf libnewlib-ia16-elf


default:
	gcc ent.c -o ent -lm
	gcc -m32 -O0 -o dry32 dry.c
	gcc -O0 -o dry64 dry.c
	ia16-elf-gcc -c -O0 dry.c -o dry16
	objcopy --dump-section .text=dry16.bin dry16
	objcopy --dump-section .text=dry32.bin dry32
	objcopy --dump-section .text=dry64.bin dry64
	echo "Entropy of text only section of 16-bit GCC compiled code (https://github.com/tkchia/gcc-ia16) - 16-bit x86" >> log.txt
	./ent dry16.bin 89 >> log.txt
	echo "Entropy of text only section of 32-bit GCC compiled code - 32-bit x86" >> log.txt
	./ent dry32.bin FF >> log.txt
	echo "Entropy of text only section of 64-bit gcc compiled code - 64-bit x86" >> log.txt
	./ent dry64.bin 48 >> log.txt


clean:
	rm ent
	rm dry16*
	rm dry32*
	rm dry64*
	rm log.txt

