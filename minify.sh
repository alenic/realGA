cat include/messages.h > ./minify/realga_lib_minified.h
python3 ./minify/minify.py --file \
include/stat.h \
include/options.h \
include/chromosome.h \
include/fitnessfunction.h \
include/mutation.h \
include/crossover.h \
include/selection.h \
include/realga.h \
src/chromosome.cpp \
src/crossover_fixed.cpp \
src/crossover_linear.cpp \
src/crossover_uniform.cpp \
src/mutation.cpp \
src/mutation_gaussian.cpp \
src/mutation_uniform.cpp \
src/options.cpp \
src/realga.cpp \
src/selection_roulette.cpp \
src/selection_tournament.cpp \
src/stat.cpp >> ./minify/realga_lib_minified.h

g++ -I./minify/realga_minified.h ./minify/test_realga_minified.cpp -o ./minify/test_realga_minified.bin
