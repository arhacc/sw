add_executable(test1 main.cpp)
target_include_directories(test1 PRIVATE ${XRT_API_INCLUDE_DIR} ${XPU_MID_LEVEL_INCLUDE_DIR})
target_link_libraries(test1 PRIVATE xpumidlevel-static xrtcore)
