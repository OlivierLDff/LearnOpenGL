cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)

project("LearnOpenGL" VERSION 1.0 LANGUAGES CXX)

include(cmake/FetchGlad.cmake)
include(cmake/FetchGlfw.cmake)
include(cmake/FetchStb.cmake)
include(cmake/FetchGlm.cmake)

add_library(learnopengl STATIC
  "lib/learnopengl/shader.hpp"
  "lib/learnopengl/shader.cpp"
  "lib/learnopengl/fileinfo.hpp"
  "lib/learnopengl/fileinfo.cpp"
  "lib/learnopengl/fpscounter.hpp"
  "lib/learnopengl/fpscounter.cpp"
  "lib/learnopengl/texture.hpp"
  "lib/learnopengl/texture.cpp"
  "lib/learnopengl/camera.hpp"
  "lib/learnopengl/camera.cpp"
  "lib/learnopengl/cameracontroller.hpp"
  "lib/learnopengl/cameracontroller.cpp"
  "lib/learnopengl/phongmaterial.hpp"
  "lib/learnopengl/phongmaterialcollection.hpp"
  "lib/learnopengl/diffusespecularmaterial.hpp"
  "lib/learnopengl/pointlight.hpp"
  "lib/learnopengl/directionlight.hpp"
  "lib/learnopengl/spotlight.hpp"
  "lib/learnopengl/window.hpp"
  "lib/learnopengl/window.cpp"
  "lib/learnopengl/stb_image.cpp"
)
target_compile_features(learnopengl PUBLIC cxx_std_20)

target_link_libraries(learnopengl PRIVATE
  glfw
  glad
  stb
  glm
)

target_include_directories(learnopengl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(CHAPTERS
  1.getting_started
  2.lighting
  3.model_loading
  4.advanced_opengl
  5.advanced_lighting
  6.pbr
  7.in_practice
)

set(1.getting_started
  1.1.hello_window
  1.2.hello_window_clear
  2.1.hello_triangle
  2.2.hello_triangle_indexed
  2.3.hello_triangle_exercise1
  2.4.hello_triangle_exercise2
  2.5.hello_triangle_exercise3
  3.1.shaders_uniform
  3.2.shaders_interpolation
  3.3.shaders_class
  3.4.shaders_exercise1
  3.5.shaders_exercise2
  3.6.shaders_exercise3
  4.1.textures
  4.2.textures_combined
  4.3.textures_exercise1
  4.4.textures_exercise2
  4.5.textures_exercise3
  4.6.textures_exercise4
  5.1.transformations
  5.2.transformations_exercise2
  6.1.coordinate_systems_plan
  6.1.coordinate_systems_cube
  6.2.coordinate_systems_depth
  6.3.coordinate_systems_multiple
  7.1.camera_circle
  7.2.camera_keyboard_dt
  7.3.camera_mouse_zoom
  7.4.camera_class
)

set(2.lighting
  1.colors
  2.1.basic_lighting_diffuse
  2.2.basic_lighting_specular
  2.3.basic_lighting_specular_repeat_box
  2.4.basic_lighting_specular_view_space
  2.5.basic_lighting_specular_gouraud
  2.6.basic_lighting_specular_texture
  2.7.basic_lighting_specular_light_from_view
  3.1.materials
  3.2.materials_exercise1
  4.1.lighting_maps_diffuse_map
  4.2.lighting_maps_specular_map
  4.3.lighting_maps_exercise3
  4.4.lighting_maps_exercise4
  5.1.light_casters_directional
  5.1.light_casters_directional_modelview
  5.2.light_casters_point
  5.3.light_casters_spot
  5.4.light_casters_spot_soft
  6.multiple_lights
)

set(3.model_loading
  1.model_loading
)

set(4.advanced_opengl
  1.1.depth_testing
  1.2.depth_testing_view
  2.stencil_testing
  3.1.blending_discard
  3.2.blending_sort
  5.1.framebuffers
  5.2.framebuffers_exercise1
  6.1.cubemaps_skybox
  6.2.cubemaps_environment_mapping
  8.advanced_glsl_ubo
  9.1.geometry_shader_houses
  9.2.geometry_shader_exploding
  9.3.geometry_shader_normals
  10.1.instancing_quads
  10.2.asteroids
  10.3.asteroids_instanced
  11.1.anti_aliasing_msaa
  11.2.anti_aliasing_offscreen
)

set(5.advanced_lighting
  1.advanced_lighting
  2.gamma_correction
  3.1.1.shadow_mapping_depth
  3.1.2.shadow_mapping_base
  3.1.3.shadow_mapping
  3.2.1.point_shadows
  3.2.2.point_shadows_soft
  4.normal_mapping
  5.1.parallax_mapping
  5.2.steep_parallax_mapping
  5.3.parallax_occlusion_mapping
  6.hdr
  7.bloom
  8.1.deferred_shading
  8.2.deferred_shading_volumes
  9.ssao
)

set(6.pbr
  1.1.lighting
  1.2.lighting_textured
  2.1.1.ibl_irradiance_conversion
  2.1.2.ibl_irradiance
  2.2.1.ibl_specular
  2.2.2.ibl_specular_textured
)

set(7.in_practice
  1.debugging
  2.text_rendering
  #3.2d_game
)

foreach(CHAPTER ${CHAPTERS})
  foreach(DEMO ${${CHAPTER}})
    file(GLOB SOURCE
      "src/${CHAPTER}/${DEMO}/*.h"
      "src/${CHAPTER}/${DEMO}/*.cpp"
      "src/${CHAPTER}/${DEMO}/*.vs"
      "src/${CHAPTER}/${DEMO}/*.fs"
      "src/${CHAPTER}/${DEMO}/*.gs"
      )

    if(SOURCE)
      set(NAME "${CHAPTER}_${DEMO}")
      add_executable(${NAME} ${SOURCE})
      target_link_libraries(${NAME} PRIVATE
        glfw
        glad
        stb
        glm
        learnopengl
      )
      target_compile_features(${NAME} PUBLIC cxx_std_17)
      set(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${CHAPTER}/${DEMO}")
      set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
      set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
      set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
      if(MSVC)
        # Make sure shader are in working dir
        set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_DIR}")
      endif()
      # copy shader files to build directory
      file(GLOB SHADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/${CHAPTER}/${DEMO}/*.vs"
        # "src/${CHAPTER}/${DEMO}/*.frag"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/${CHAPTER}/${DEMO}/*.fs"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/${CHAPTER}/${DEMO}/*.gs"
      )
      foreach(SHADER ${SHADERS})
        add_custom_command(
          TARGET ${NAME}
          POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>
          DEPENDS ${SHADER}
          COMMENT "Copy ${SHADER}"
        )
      endforeach(SHADER)
    endif()
  endforeach()
endforeach()
