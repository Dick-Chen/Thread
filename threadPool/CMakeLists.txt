project(threadpool)

aux_source_directory(src SRC_SUB)
aux_source_directory(. SRC_CUR)

add_executable(threadpool ${SRC_SUB} ${SRC_CUR})

include_directories(include)

target_link_libraries(threadpool pthread)