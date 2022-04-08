cmake_minimum_required(VERSION 3.0.0)
project(cifras VERSION 0.1.0)

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    #set(CMAKE_C_FLAGS "-s STANDALONE_WASM" )
endif()

add_executable(cifras CIFRAS.C)
    
if (EMSCRIPTEN)
set_target_properties(cifras PROPERTIES LINK_FLAGS "-s EXPORTED_FUNCTIONS=_resuelve,_generaEnunciado -s EXPORTED_RUNTIME_METHODS=ccall --shell-file ../shell_cifras.html")
    # set_target_properties(
    #     ssolar
    #     PROPERTIES
    #     LINK_FLAGS
    #     "-s TOTAL_MEMORY=33554432 -s USE_GLFW=3 -s USE_WEBGL2=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"jpg\"]' --preload-file ../assets@assets"
    # )
else(EMSCRIPTEN)
    # target_link_libraries(ssolar glfw GL GLEW SDL2 SDL2main SDL2_image)    
endif(EMSCRIPTEN)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
