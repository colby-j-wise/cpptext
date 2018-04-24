set(SOURCES main.cpp normalizer.cpp processor.cpp)
set(TEST_SOURCES test_main.cpp normalizer.cpp processor.cpp unit_test.cpp)
add_executable(cpptext ${SOURCES})
add_executable(unittest ${TEST_SOURCES})
# copy executable to bin directory
set_target_properties(cpptext PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../bin)
set_target_properties(unittest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../bin)
# link with libm for math functions
target_link_libraries(cpptext m)
target_link_libraries(unittest m)
# link with gnu scientific library and blas
target_link_libraries(cpptext GSL::gsl GSL::gslcblas)
target_link_libraries(unittest GSL::gsl GSL::gslcblas)
# link with OS specific thread library
target_link_libraries (cpptext ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (unittest ${CMAKE_THREAD_LIBS_INIT})
# link with Boost
target_link_libraries(cpptext ${Boost_LIBRARIES})
target_link_libraries(unittest ${Boost_LIBRARIES})
# for now use debug compile options with all warnings on
target_compile_options(cpptext PUBLIC ${MY_DEBUG_OPTIONS})
target_compile_options(unittest PUBLIC ${MY_DEBUG_OPTIONS})
message("${CMAKE_CXX_FLAGS}")
message("${MY_DEBUG_OPTIONS}")
