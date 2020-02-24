#!/bin/bash
killall brinespell >/dev/null 2>/dev/null
MALLOC_CHECK_=0 ../out/brinespell --docroot "/dalaran/brinespell/out;/css,/img,/resources,/upload,/template" --http-address 0.0.0.0 --http-port 9090 &>../out/log.txt &
