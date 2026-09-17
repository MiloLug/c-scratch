option(GCST_WARNINGS_AS_ERRORS "Promote significant warnings to errors" OFF)

# ---> GNU WARNINGS SETUP <--- #

set(GCST_WARN_GNU
    -Wall
    -Wextra
    -Wpedantic

    # correctness
    -Wcast-align                    
    -Wcast-qual                     
    -Wformat=2                      
    -Wnull-dereference
    -Wshadow                        
    -Wundef                         

    # C++ / polymorphism
    -Wnon-virtual-dtor              
    -Woverloaded-virtual            
    -Wzero-as-null-pointer-constant
    -Wextra-semi

    # control flow
    -Wimplicit-fallthrough
    -Wmisleading-indentation

    # noisy, kept as warnings only (see GCST_WARN_GNU_NOT_ERRORS)
    -Wconversion
    -Wsign-conversion
    -Wdouble-promotion
    -Wfloat-equal
    -Wold-style-cast
    -Wswitch-enum
)

set(GCST_WARN_GCC_ONLY
    -Wduplicated-cond      
    -Wduplicated-branches  
    -Wlogical-op           
    -Wuseless-cast
    -Wsuggest-override
)

set(GCST_WARN_CLANG_ONLY
    -Wcomma                         
    -Wloop-analysis
    -Winconsistent-missing-override
)

# -Werror Blacklist
set(GCST_WARN_GNU_NOT_ERRORS
    -Wno-error=unused-parameter
    -Wno-error=unused-variable
    -Wno-error=unused-but-set-variable
    -Wno-error=unused-but-set-parameter
    -Wno-error=unused-function
    -Wno-error=unused-local-typedefs
    -Wno-error=unused-label
    -Wno-error=missing-field-initializers
    -Wno-error=type-limits
    -Wno-error=sign-compare
    -Wno-error=conversion
    -Wno-error=sign-conversion
    -Wno-error=double-promotion
    -Wno-error=float-equal
    -Wno-error=old-style-cast
    -Wno-error=switch-enum
    -Wno-error=deprecated-declarations
    -Wno-error=comment
    -Wno-error=unknown-pragmas
)

# ---> MSVC WARNINGS SETUP <--- #

set(GCST_WARN_MSVC
    /W4
    /permissive-             
    /Zc:__cplusplus          
    /Zc:preprocessor         

    # off-by-default diagnostics raised to level 1, GNU counterparts in comments
    /w14242                         # -Wconversion
    /w14254
    /w14263                         # -Woverloaded-virtual
    /w14265                         # -Wnon-virtual-dtor
    /w14287                         # -Wsign-conversion
    /w14296                         # comparison always true/false
    /w14311                         # pointer truncation
    /w14545                         # expression with no effect
    /w14546
    /w14547
    /w14549
    /w14555
    /w14619                         # #pragma warning for a nonexistent number
    /w14640                         # thread-unsafe static initialization
    /w14826                         # sign-extending conversion
    /w14905                         # string literal cast
    /w14906
    /w14928                         # illegal copy-initialization
    /w14062                         # -Wswitch-enum
)

# /we whitelist (no BL in MSVC)
set(GCST_WARN_MSVC_ERRORS
    /we4715                         # not all control paths return a value
    /we4700                         # uninitialized local variable used
    /we4703                         # potentially uninitialized pointer used
    /we4473                         # not enough arguments for format string
    /we4474                         # too many arguments for format string
    /we4477                         # format string type mismatch
    /we4013                         # (C) function undefined, assumed extern int
    /we4020                         # too many actual parameters
    /we4047                         # differing levels of indirection
    /we4133                         # incompatible types
)

# ---> GCST WARNING UTILITIES <---

function(gcst_target_warnings target_name)
    if(MSVC)
        set(flags ${GCST_WARN_MSVC})
        if(GCST_WARNINGS_AS_ERRORS)
            list(APPEND flags ${GCST_WARN_MSVC_ERRORS})
        endif()
    else()
        set(flags ${GCST_WARN_GNU})
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            list(APPEND flags ${GCST_WARN_GCC_ONLY})
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            list(APPEND flags ${GCST_WARN_CLANG_ONLY})
        endif()

        if(GCST_WARNINGS_AS_ERRORS)
            list(APPEND flags -Werror ${GCST_WARN_GNU_NOT_ERRORS})
        endif()
    endif()

    target_compile_options("${target_name}" PRIVATE ${flags})
endfunction()

function(gcst_target_optimization target_name)
    if(MSVC)
        target_compile_options("${target_name}" PRIVATE
            $<$<CONFIG:Debug>:/Od;/Zi>
            $<$<CONFIG:Release>:/O2>
            $<$<CONFIG:RelWithDebInfo>:/O2;/Zi>
            $<$<CONFIG:MinSizeRel>:/O1>
        )
    else()
        target_compile_options("${target_name}" PRIVATE
            $<$<CONFIG:Debug>:-O0;-g>
            $<$<CONFIG:Release>:-O2>
            $<$<CONFIG:RelWithDebInfo>:-O2;-g>
            $<$<CONFIG:MinSizeRel>:-Os>
        )
    endif()
endfunction()
