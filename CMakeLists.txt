cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(PLATFORM "pc" CACHE STRING "Platform to build for")
set_property(CACHE PLATFORM PROPERTY STRINGS pc vita switch)

IF(PLATFORM STREQUAL "pc")
ELSEIF(PLATFORM STREQUAL "vita")
    IF(DEFINED ENV{VITASDK})
        include("$ENV{VITASDK}/share/vita.toolchain.cmake" REQUIRED)
        include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
        set(VITA_APP_NAME "Ironhead")
        set(VITA_TITLEID  "IRHD00001")
        set(VITA_VERSION  "01.00")
        set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")
    ELSE()
        message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
    ENDIF()
ELSEIF(PLATFORM STREQUAL "switch")
    IF(DEFINED ENV{DEVKITPRO})
        include("cmake/switch.toolchain.cmake" REQUIRED)
        include("cmake/switch.tools.cmake" REQUIRED)
    ELSE()
        message(FATAL_ERROR "Please define DEVKITPRO to point to your SDK path!")
    ENDIF()

ELSE()
    message(FATAL_ERROR "Wrong platform")
ENDIF()

project(ironhead CXX)

set (ironhead_VERSION_MAJOR 2)
set (ironhead_VERSION_MINOR 6)
set (ironhead_VERSION_RELEASE 4)
set (ironhead_APP_ID org.nxengine.ironhead)

include(CheckCXXCompilerFlag)
include(GNUInstallDirs)


find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${PNG_PNG_INCLUDE_DIR})
include_directories("${ironhead_SOURCE_DIR}/deps")

file(GLOB APP_SOURCES "src/[a-zA-Z]*.cpp")
file(GLOB CM_SOURCES "src/common/[a-zA-Z]*.cpp")
file(GLOB UT_SOURCES "src/Utils/[a-zA-Z]*.cpp")
file(GLOB INI_SOURCES "src/Utils/Ini/[a-zA-Z]*.cpp")
file(GLOB EV_SOURCES "src/Event/[a-zA-Z]*.cpp")
file(GLOB GM_SOURCES "src/Game/[a-zA-Z]*.cpp")
file(GLOB ST_SOURCES "src/State/[a-zA-Z]*.cpp")
file(GLOB GR_SOURCES "src/Graphics/[a-zA-Z]*.cpp")
file(GLOB SN_SOURCES "src/Sound/[a-zA-Z]*.cpp")
file(GLOB PL_SOURCES "src/platform/[a-zA-Z]*.cpp")

include_directories(${ironhead_SOURCE_DIR})

set(SOURCES
    ${APP_SOURCES}
    ${CM_SOURCES}
    ${UT_SOURCES}
    ${INI_SOURCES}
    ${EV_SOURCES}
    ${GM_SOURCES}
    ${ST_SOURCES}
    ${GR_SOURCES}
    ${SN_SOURCES}
    ${PL_SOURCES}
)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("debug mode")
    add_definitions(-DTRACE_SCRIPT)
    add_definitions(-DDEBUG)
ENDIF()

add_definitions("-Wall")

add_executable(ironhead ${SOURCES})

IF(PLATFORM STREQUAL "pc")
set_property(TARGET ironhead PROPERTY OUTPUT_NAME ironhead)
    add_definitions("-std=c++17")
    target_link_libraries(ironhead ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${PNG_LIBRARY} ${JPEG_LIBRARY})

ELSEIF(PLATFORM STREQUAL "vita")
    add_definitions("-std=gnu++17")
    add_definitions("-D__VITA__")
    target_link_libraries(ironhead ${SDL2_MIXER_LIBRARY} ${SDL2_IMAGE_LIBRARY} SDL2::SDL2-static ${PNG_LIBRARY} ${JPEG_LIBRARY}
      webp
      png
      jpeg
      pthread
      m
      z
      FLAC
      vorbisfile
      vorbis
      ogg
      mikmod
      mpg123
      opusfile
      opus
    )

  vita_create_self(${PROJECT_NAME}.self ironhead UNSAFE)
  vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
    VERSION ${VITA_VERSION}
    NAME ${VITA_APP_NAME}
    FILE platform/vita/sce_sys sce_sys
    FILE data data
  )
  add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/release/NXEngine-Evo-${ironhead_VERSION_MAJOR}.${ironhead_VERSION_MINOR}.${ironhead_VERSION_RELEASE}.vpk
    COMMAND cp ironhead.vpk ${CMAKE_SOURCE_DIR}/release/NXEngine-Evo-${ironhead_VERSION_MAJOR}.${ironhead_VERSION_MINOR}.${ironhead_VERSION_RELEASE}.vpk
    DEPENDS ironhead.vpk
    COMMENT "Moving vpk to release"
  )
  add_custom_target(ironheadbinvpk_ ALL DEPENDS ${CMAKE_SOURCE_DIR}/release/NXEngine-Evo-${ironhead_VERSION_MAJOR}.${ironhead_VERSION_MINOR}.${ironhead_VERSION_RELEASE}.vpk)

ELSEIF(PLATFORM STREQUAL "switch")
    add_definitions("-std=gnu++11")
    add_definitions("-D__SWITCH__")
    target_link_libraries(nx SDL2_mixer SDL2_image SDL2 png jpeg webp
      m
      z
      FLAC
      vorbisidec
      ogg
      mikmod
      mpg123
      modplug
      EGL
      glapi
      drm_nouveau
      -lnx
      opusfile
      opus
    )

    add_nro_target(nx "NXEngine-evo" "NXEngine team" "${ironhead_VERSION_MAJOR}.${ironhead_VERSION_MINOR}.${ironhead_VERSION_RELEASE}" "${CMAKE_SOURCE_DIR}/platform/switch/icon.jpg" "${CMAKE_SOURCE_DIR}/release")
    add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/release/NXEngine-Evo-${ironhead_VERSION_MAJOR}.${ironhead_VERSION_MINOR}.${ironhead_VERSION_RELEASE}.nro
      COMMAND cp nx.nro ${CMAKE_SOURCE_DIR}/release/NXEngine-Evo-${ironhead_VERSION_MAJOR}.${ironhead_VERSION_MINOR}.${ironhead_VERSION_RELEASE}.nro
      DEPENDS nx.nro
      COMMENT "Moving nro to release"
    )
    add_custom_target(nxbinnro_ ALL DEPENDS ${CMAKE_SOURCE_DIR}/release/NXEngine-Evo-${ironhead_VERSION_MAJOR}.${ironhead_VERSION_MINOR}.${ironhead_VERSION_RELEASE}.nro)
ENDIF()

IF(PLATFORM STREQUAL "pc")
    install(TARGETS ironhead RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(DIRECTORY data DESTINATION ${CMAKE_INSTALL_DATADIR}/ironhead)

    # Install XDG metadata on Desktop Linux like platforms
    IF(UNIX AND NOT APPLE AND NOT ANDROID)
        install(FILES platform/xdg/${ironhead_APP_ID}.desktop     DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
        install(FILES platform/xdg/${ironhead_APP_ID}.png         DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps)
        install(FILES platform/xdg/${ironhead_APP_ID}.appdata.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
    ENDIF()
ENDIF()


