cmake_minimum_required(VERSION 3.21)
project(myWallet LANGUAGES CXX VERSION 1.0)

# ================= Sources =================
file(GLOB SOURCES "src_cpp/*.cpp")
# 👇 Add WIN32 here so MSVC knows it's a GUI app
add_executable(${PROJECT_NAME} WIN32 "main.cpp" ${SOURCES})

# ================= Windows Icon Resource =================
if (WIN32)
    # Resource script (.rc) in project root
    set(APP_ICON_RESOURCE "${CMAKE_SOURCE_DIR}/appIcon.rc")
    target_sources(${PROJECT_NAME} PRIVATE ${APP_ICON_RESOURCE})
endif()

# ================= Language / Features =================
set(saucer_serializer "Glaze")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include_cpp/"
)
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/embedded/"
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
    CXX_STANDARD 23 
    CXX_EXTENSIONS OFF 
    CXX_STANDARD_REQUIRED ON
)

# ================= Saucer =================
include(FetchContent)
FetchContent_Declare(
    saucer 
    GIT_REPOSITORY "https://github.com/saucer/saucer" 
    GIT_TAG v6.0.1
)
FetchContent_MakeAvailable(saucer)

# ================= SQLite =================
if (WIN32)
    set(SQLite3_INCLUDE_DIR "C:/sqlite3")
    set(SQLite3_LIBRARY "C:/sqlite3/sqlite3.lib")

    add_library(SQLite::SQLite3 UNKNOWN IMPORTED)
    set_target_properties(SQLite::SQLite3 PROPERTIES
        IMPORTED_LOCATION "${SQLite3_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${SQLite3_INCLUDE_DIR}"
    )
else()
    find_package(SQLite3 REQUIRED)
endif()

# ================= Performance Flags =================
if (NOT MSVC)
    set(MY_PERFORMANCE_FLAGS
        -O3 -march=native -mtune=native
        -flto -ffast-math -funroll-loops
        -fomit-frame-pointer -finline-functions
        -fdata-sections -ffunction-sections
        -Wall -Wextra
    )
    target_compile_options(${PROJECT_NAME} PRIVATE ${MY_PERFORMANCE_FLAGS})
endif()

# ================= Windows Console Hiding =================
if (WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "embedded")
target_link_libraries(${PROJECT_NAME} PRIVATE saucer::saucer SQLite::SQLite3)

# ================= Install Rules =================
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .
)

if (EXISTS "${CMAKE_SOURCE_DIR}/frontend-dist")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/frontend-dist/"
            DESTINATION share/mywallet)
endif()

if (EXISTS "${CMAKE_SOURCE_DIR}/LICENSE")
    install(FILES "${CMAKE_SOURCE_DIR}/LICENSE"
            DESTINATION share/licenses/mywallet)
endif()
# ================= CPack Packaging =================
set(CPACK_PACKAGE_NAME "MyWallet")
set(CPACK_PACKAGE_VENDOR "Enam Antoine M. Kodjoh-Kpakpassou")
set(CPACK_PACKAGE_CONTACT "eakodjoh-kpakpassou@st.ug.edu.gh")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Wallet tracker built with Saucer (C++) and Svelte frontend")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MyWallet")

# License shown during NSIS install
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

if (WIN32)
    # Generate both ZIP archive and NSIS installer
    set(CPACK_GENERATOR "ZIP;NSIS")

    # NSIS settings
    set(CPACK_NSIS_DISPLAY_NAME "MyWallet")
    set(CPACK_NSIS_HELP_LINK "https://github.com/saucer/saucer")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/saucer/saucer")
    set(CPACK_NSIS_CONTACT "${CPACK_PACKAGE_CONTACT}")
    set(CPACK_NSIS_MODIFY_PATH ON)

    # Installer icon
    set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/src/lib/assets/icon.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/src/lib/assets/icon.ico")

    # Start Menu shortcut (points to installed exe in $INSTDIR)
    set(CPACK_NSIS_MENU_LINKS "myWallet.exe" "MyWallet")

    # Desktop shortcut (optional)
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut \\\"$DESKTOP\\\\MyWallet.lnk\\\" \\\"$INSTDIR\\\\myWallet.exe\\\"")
    set(CPACK_NSIS_DELETE_ICONS_EXTRA
        "Delete \\\"$DESKTOP\\\\MyWallet.lnk\\\"")

elseif(APPLE)
    # macOS: DMG disk image
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "MyWallet")
    # Optional background image for DMG
    # set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/src/lib/assets/dmg-background.png")

else()
    # Linux / Arch: simple tarball
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)