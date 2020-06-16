#!/bin/bash
killall brinespell >/dev/null 2>/dev/null
../out/brinespell --docroot "/dalaran/brinespell/out;/css,/img,/resources,/upload,/data,/dice" --http-address 0.0.0.0 --http-port 9090 --accesslog=- &
