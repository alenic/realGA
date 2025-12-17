
mkdir -p build/swig-python/realga

swig -c++ -python -relativeimport \
  -I./include \
  -outdir ./build/swig-python/realga \
  -o ./build/swig-python/realga_wrap.cxx \
  ./swig/realga.i

mv ./build/swig-python/realga/realga.py ./build/swig-python/realga/__init__.py


c++ -O3 -fPIC -shared \
  ./build/swig-python/realga_wrap.cxx \
  -o ./build/swig-python/realga/_realga.so \
  -I./include \
  $(python3-config --includes) \
  -L./build/libs -lrealga_shared \
  -Wl,-rpath,'$ORIGIN'

cp ./build/libs/librealga_shared.so ./build/swig-python/realga/
cp ./swig/setup.py ./build/swig-python/


cd build/swig-python
pip install .