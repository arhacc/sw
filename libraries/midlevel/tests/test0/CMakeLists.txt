add_executable(test0 main.cpp)
target_include_directories(test0 PRIVATE ${XRT_API_INCLUDE_DIR} ${XPU_MID_LEVEL_INCLUDE_DIR})
target_link_libraries(test0 PRIVATE xpumidlevel-static xrtcore)
