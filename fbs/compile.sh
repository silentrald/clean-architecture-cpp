#!/bin/bash

flatc -o ../src/fbs \
  --filename-suffix '' \
  --filename-ext hpp \
  --cpp \
    auth.fbs

