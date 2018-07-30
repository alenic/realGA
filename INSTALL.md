# Install
## Windows

```
> cmake ..
> MSBuild.exe .\realGen.sln /p:Configuration={Release|Debug}
```

For Win64 use the command

```
> cmake -G "Visual Studio 14 2015 Win64" ..
```

## Linux
```
> cmake -DCMAKE_BUILD_TYPE={Release|Debug} ..
> make
```