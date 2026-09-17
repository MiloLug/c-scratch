include("${CMAKE_CURRENT_LIST_DIR}/warnings.cmake")

function(gcst_message message)
    message("-- | (GCST) | ${message}")
endfunction()

function(gcst_normalize var)
    if(NOT ${var} OR ${var} MATCHES "-NOTFOUND$")
        set(${var} "" PARENT_SCOPE)
    else()
        string(REPLACE ";" " " tmp "${${var}}")
        set(${var} "${tmp}" PARENT_SCOPE)
    endif()
endfunction()


function(gcst_export_prepare target_name)
    gcst_message("Exporting ${target_name}")

    foreach(obj IN LISTS ARGN)
        gcst_message("-- Adding source-object '${obj}' to target '${target_name}'")
        target_sources(${target_name} PRIVATE $<TARGET_OBJECTS:${obj}>)
    endforeach()

    string(FIND "${target_name}" "_" POS)
    if(POS EQUAL -1)
        set(MODULE "${target_name}")
        set(PREFIX "${target_name}")
    else()
        string(SUBSTRING "${target_name}" 0 ${POS} PREFIX)
        math(EXPR RIGHT_START "${POS} + 1")
        string(SUBSTRING "${target_name}" ${RIGHT_START} -1 MODULE)
    endif()

    add_library("${PREFIX}::${MODULE}" ALIAS "${target_name}")
    target_include_directories("${target_name}" PUBLIC
        $<BUILD_INTERFACE:${GCST_INCLUDE_DIRS}>
        $<INSTALL_INTERFACE:include>
    )
    set_target_properties("${target_name}" PROPERTIES EXPORT_NAME "${MODULE}")
    gcst_target_warnings("${target_name}")
    gcst_target_optimization("${target_name}")
    if (MINGW)
        target_link_libraries("${target_name}" INTERFACE
            $<$<COMPILE_FEATURES:cxx_std_23>:stdc++exp>
        )
    endif()

    gcst_message("Exported (with prefix = '${PREFIX}', module = '${MODULE}')")
endfunction()


function(gcst_binary_prepare target_name)
    target_include_directories("${target_name}" PUBLIC "${GCST_INCLUDE_DIRS}")
    gcst_target_warnings("${target_name}")
    gcst_target_optimization("${target_name}")
    if (MINGW)
        target_link_libraries("${target_name}" PUBLIC
            $<$<COMPILE_FEATURES:cxx_std_23>:stdc++exp>
        )
    endif()

    get_target_property(libs ${target_name} LINK_LIBRARIES)
    get_target_property(opts ${target_name} COMPILE_OPTIONS)
    gcst_normalize(libs)
    gcst_normalize(opts)

    gcst_message("Preparing target '${target_name}'")
    gcst_message("-- flags: ${opts}")
    gcst_message("-- libs: ${libs}")
endfunction()