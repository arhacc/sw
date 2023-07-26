add_executable(test-debug0 main.cpp)
target_include_directories(test-debug0 PRIVATE ${XRT_API_INCLUDE_DIR} ${XPU_MID_LEVEL_INCLUDE_DIR})
target_link_libraries(test-debug0 PRIVATE xpumidlevel xrtcore)
