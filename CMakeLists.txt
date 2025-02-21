cmake_minimum_required(VERSION 3.12)

project(wmpq)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_executable(wmpq
    main.cxx
    queue_manager.cxx
    ytdlp_handler.cxx
   mpv_player.cxx
)

target_link_libraries(wmpq PRIVATE spdlog fmt curl mpv)
