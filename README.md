#Smart Management Allocator

To be honest :
I was tired of always needing to free a few dozen pointers every time a malloc
fails.Let 's face it: It' s dumb to always repeat the same code over and over again.Say your
malloc fails and then you have to type this code over and over again and it keeps getting
longer every time :

```c void* ptr = malloc( sizeof( int ) * 10 );
if ( NULL == ptr ) {
        free( ptr1 );
        ptr1              = NULL;
        free( ptr2 ) ptr2 = NULL;
        for ( size_t i = 0; i < elements; i++ ) {
                free( ptr3[i] );
                ptr3[i] = NULL;
        }
        free( ptr3 );
        ptr3 = NULL;
        free( ptr4 );
        ptr4 = NULL;
        // This is getting annoying, right?
        return NULL;
}
```

This is where this library comes in.
You generate a `smalloc` context object and leave all the allocations and reallocations up to smalloc.
If you don't want to free everything manually, you can just tell smalloc to `smalloc_free_all(ctx)` and it will
automatically free all the pointers in the context you pass to it.

### Build

To build the project, you need to have CMake installed.
This will be installed as a library on your system.
To build and install the project, clone it and in the root directory of it run:

```bash
cmake -B cmake-build && cmake --build ./cmake-build --target smalloc -j 6
sudo cmake --install ./cmake-build # This installs the library on your system
```

### Usage

To use the library, refer to the `test/test.c` file.
You can include the library in your project by adding the following line to your CMakeLists.txt file:

```cmake
find_package(smalloc REQUIRED)

add_executable(YourBinary /path/to/your/file.c)

target_link_libraries(YourBinary PRIVATE smalloc::smalloc)
```

When using it, you can pass it flags for your allocations.
If a pointer does not need to be free'd early or you need to reallocate it later, you can just pass the
`SMALLOC_FLAG_AUTO` flag to the allocation function.
This will allocate the pointer to an internal stack and free it when you call free on it.

If you need to reallocate the pointer or want to free the pointer early, then you can pass the
`SMALLOC_FLAG_PERSIST` flag to the allocation function.
(NOTE: I may have not implemented the specific feature of freeing the allocation early just yet. :) ).
This will allocate the pointer to an internal list and give you the ability to reallocate the pointer or free it
early (soonTM).

You can also use the allocate_arr functionality to allocate an array of pointers.
Upon calling the regular `smalloc_free_all(ctx)`, it will automatically free all the pointers in the array.

### Valgrind

To see for yourself that this is (probably) a memory-safe allocator, you can run the following command:

```bash
cmake -B cmake-build && cmake --build ./cmake-build --target valgrind -j 6
```

The result will be saved in `cmake-build/valgrind.log`.
If you find any allocation errors let me know.

### TODO

- [ ] Add the ability to free a pointer early
- [ ] Add the ability to exclude a pointer from the auto free list (is that even useful? Why would you add it to smalloc
  in the first place if you don't want it free'd? For consistency? )

### License

It's GPLv3. Feel free to do whatever you want with this. All bets are off, no warranty, no liability, but it's both free
as in beer and free as in freedom. :)