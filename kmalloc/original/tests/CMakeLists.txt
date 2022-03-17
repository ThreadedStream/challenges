add_executable(test main.c)
target_link_libraries(test kmalloc)
#target_compile_options(test PUBLIC -m32)