python3 ./minify/minify.py --file \
include/messages.h \
include/stat.h \
include/algorithms.h \
include/options.h \
include/chromosome.h \
include/fitnessfunction.h \
include/selection.h \
include/realga.h \
src/stat.cpp \
src/algorithms.cpp \
src/options.cpp \
src/chromosome.cpp \
src/selection_roulette.cpp \
src/selection_tournament.cpp \
src/realga.cpp \
> ./minify/realga_lib_minified.h

g++ -I./minify/realga_minified.h ./minify/test_realga_minified.cpp -o ./minify/test_realga_minified.bin
