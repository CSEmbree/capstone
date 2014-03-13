#!/bin/bash

arecord -D plughw:1 --duration=$1 -f cd -vv ~/sounds/recTest.wav