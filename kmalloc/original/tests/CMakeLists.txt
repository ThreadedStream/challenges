add_compile_options(-m32)
add_executable(test main.c)
target_link_libraries(test kmalloc)

