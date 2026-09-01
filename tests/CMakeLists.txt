find_package(PkgConfig REQUIRED)
pkg_check_modules(CRITERION REQUIRED criterion)

add_executable(unit_tests
    test_helpers.c
        test_lda.c
        test_sequences.c
        test_adc.c
        test_sbc.c
        test_cmp.c
)

target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CRITERION_INCLUDE_DIRS}
)

target_link_libraries(unit_tests PRIVATE
    emu_core
    ${CRITERION_LIBRARIES}
    raylib
)

target_compile_options(unit_tests PRIVATE ${CRITERION_CFLAGS_OTHER})

add_test(NAME unit_tests COMMAND unit_tests )