# Copyright (C) 2014 Maria Jose Galvez Trigo GNU GPLV3

cmake_minimum_required(VERSION 2.8)
project(tetheringenabler)

find_package(qibuild)

option(TETHERINGENABLER_IS_REMOTE
  "module is compiled as a remote module (ON or OFF)"
  OFF)

set(_srcs
  main.cpp
  tetheringenabler.h
  tetheringenabler.cpp
)

if(TETHERINGENABLER_IS_REMOTE)
  add_definitions(" -DTETHERINGENABLER_IS_REMOTE ")
  qi_create_bin(tetheringenabler ${_srcs})
  qi_use_lib(tetheringenabler ALCOMMON)
else()
  qi_create_lib(tetheringenabler SHARED ${_srcs} SUBFOLDER naoqi)
  qi_use_lib(tetheringenabler ALCOMMON BOOST)
endif()





