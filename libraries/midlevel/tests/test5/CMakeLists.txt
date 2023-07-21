add_executable(test5 main.cpp)
target_include_directories(test5 PRIVATE ${XRT_API_INCLUDE_DIR} ${XPU_MID_LEVEL_INCLUDE_DIR})
target_link_libraries(test5 PRIVATE xpumidlevel-static xrtcore)
