add_executable(test2 main.cpp)
target_include_directories(test2 PRIVATE ${XRT_API_INCLUDE_DIR} ${XPU_MID_LEVEL_INCLUDE_DIR})
target_link_libraries(test2 PRIVATE xpumidlevel xrtcore)
