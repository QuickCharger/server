# server
服务器，用到libevent等库
编译后生成的所有有用的文件都放在bin文件夹下 可以直接运行

编译环境
win + cygwin
  mkdir sln && cd sln && cmake .. && cmake --build . --clean-first --config Release
linux
  mkdir sln && cd sln && cmake .. && cmake --build . --clean-first --config Release
