#!/usr/bin/env bash

cpplint --filter=-legal,-build,-runtime/int,-runtime/references,-runtime/explicit,-runtime/string src/*
