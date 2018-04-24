set(SOURCES main.cpp normalizer.cpp processor.cpp)
add_executable(cpptext ${SOURCES})
# copy executable to bin directory
set_target_properties(cpptext PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../bin)
# link with libm for math functions
target_link_libraries(cpptext m)
# link with gnu scientific library and blas
target_link_libraries(cpptext GSL::gsl GSL::gslcblas)
# link with OS specific thread library
target_link_libraries (cpptext ${CMAKE_THREAD_LIBS_INIT})
# link with Boost
target_link_libraries(cpptext ${Boost_LIBRARIES})
# for now use debug compile options with all warnings on
target_compile_options(cpptext PUBLIC ${MY_DEBUG_OPTIONS})
message("${CMAKE_CXX_FLAGS}")
message("${MY_DEBUG_OPTIONS}")
