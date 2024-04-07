include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(square_solver_test testParabola.cpp testArrayView.cpp testSharedRingBuffer.cpp)
set_property(TARGET square_solver_test PROPERTY CXX_STANDARD 17)

target_link_libraries(square_solver_test
 PRIVATE GTest::GTest)

add_test(square_solver_tests square_solver_test)
