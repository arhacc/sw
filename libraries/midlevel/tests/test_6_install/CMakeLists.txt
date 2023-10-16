add_executable(test_6_install main.cpp)
target_include_directories(test_6_install PRIVATE ${XRT_API_INCLUDE_DIR} ${XPU_MID_LEVEL_INCLUDE_DIR})
target_link_libraries(test_6_install PRIVATE xpumidlevel-static xrtcore)
