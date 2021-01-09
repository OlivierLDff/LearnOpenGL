include(FetchContent)

FetchContent_Declare(
  glad
  GIT_REPOSITORY "https://github.com/Dav1dde/glad"
  GIT_TAG        "master"
)
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_API "gl=" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")

FetchContent_MakeAvailable(glad)

set_target_properties(glad PROPERTIES FOLDER "GLAD")
set_target_properties(glad-generate-files PROPERTIES FOLDER "GLAD")
