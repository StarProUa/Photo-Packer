cmake_minimum_required(VERSION 3.5)

project(Photo-Packer LANGUAGES C)

add_executable(Photo-Packer main.c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

include(GNUInstallDirs)
install(TARGETS Photo-Packer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
