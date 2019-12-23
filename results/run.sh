#! /bin/sh

cd ..
./waf --run=ddos-scenario-1755

echo start painting ...
cd results
sh paint.sh