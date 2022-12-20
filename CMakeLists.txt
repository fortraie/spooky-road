cmake_minimum_required(VERSION 3.24)
project(pswmanager_spooky_road)

set(CMAKE_CXX_STANDARD 17)

add_executable(pswmanager_spooky_road main.cpp classes/Session.cpp classes/Session.h support/encryption.cpp support/encryption.h support/iohandling.cpp support/iohandling.h classes/IOHandlingTag.h)
