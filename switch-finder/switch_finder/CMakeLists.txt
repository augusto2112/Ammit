add_library(SwitchFinder MODULE
    # List your source files here.
    SwitchFinder.cpp
        ../test/test.c)

# LLVM is (typically) built with no C++ RTTI. We need to match that;
# otherwise, we'll get linker errors about missing RTTI data.
set_target_properties(SwitchFinder PROPERTIES
    COMPILE_FLAGS "-fno-rtti -std=c++14"
)

# Get proper shared-library behavior (where symbols are not necessarily
# resolved when the shared library is linked) on OS X.
if(APPLE)
    set_target_properties(SwitchFinder PROPERTIES
        LINK_FLAGS "-undefined dynamic_lookup"
    )
endif(APPLE)
