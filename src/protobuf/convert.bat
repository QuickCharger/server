set outpath=%cd%
cd ../../../protobuf
protoc.exe %outpath%\transmit.proto --cpp_out=%outpath% --proto_path=%outpath%
pause