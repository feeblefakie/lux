#/bin/sh -x

PROTOS="./lux/document/document_definition
        ./lux/dist/server-config
        ./lux/dist/update-protocol"

for file in $PROTOS
do
    dir=`dirname $file`
    protoc -I=. --cpp_out=. $file.proto
    mv $file.pb.cc $file.pb.cpp
done


