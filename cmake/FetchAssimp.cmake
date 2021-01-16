include(FetchContent)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY "https://github.com/assimp/assimp"
  GIT_TAG        "master"
)
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build package with shared libraries." FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE INTERNAL "If the official samples are built as well (needs Glut).")
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "If the test suite for Assimp is built in addition to the library.")
set(ASSIMP_INSTALL OFF CACHE INTERNAL "Disable this if you want to use assimp as a submodule.")

FetchContent_MakeAvailable(assimp)

set_target_properties(assimp PROPERTIES FOLDER "assimp")
set_target_properties(assimp_cmd PROPERTIES FOLDER "assimp")
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs  PROPERTIES FOLDER "assimp")
set_target_properties(zlibstatic PROPERTIES FOLDER "zlib")
