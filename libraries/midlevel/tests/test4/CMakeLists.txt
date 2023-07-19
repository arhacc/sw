add_executable(test4 main.cpp)
target_include_directories(test4 PRIVATE ${XRT_API_INCLUDE_DIR} ${XPU_MID_LEVEL_INCLUDE_DIR})
target_link_libraries(test4 PRIVATE xpumidlevel xrtcore)
