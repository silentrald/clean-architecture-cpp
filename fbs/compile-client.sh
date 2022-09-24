#!/bin/bash

flatc -o . \
  --filename-suffix '' \
  --ts \
    auth.fbs

