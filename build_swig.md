```
swig -c++ -python -I./include -o ./build/swig-python/realga_wrap.cxx ./swig/realga.i
```


```
c++ -O3 -fPIC -shared \
  ./build/swig-python/realga_wrap.cxx \
  -o ./build/swig-python/_realga.so \
  -I./include \
  $(python3-config --includes) \
  -L./build/libs -lrealga_shared \
  -Wl,-rpath,'$ORIGIN/../libs'
```