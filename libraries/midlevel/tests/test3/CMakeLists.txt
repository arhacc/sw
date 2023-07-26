add_executable(test3 main.cpp)
target_include_directories(test3 PRIVATE ${XRT_API_INCLUDE_DIR} ${XPU_MID_LEVEL_INCLUDE_DIR})
target_link_libraries(test3 PRIVATE xpumidlevel-static xrtcore)
