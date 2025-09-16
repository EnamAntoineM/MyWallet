cmake_minimum_required(VERSION 3.21)
project(myWallet LANGUAGES CXX VERSION 1.0)

file(GLOB SOURCES "src_cpp/*.cpp")
add_executable(${PROJECT_NAME} "main.cpp" ${SOURCES})

set(saucer_serializer "Glaze")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include_cpp/")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/embedded/")
set_target_properties(${PROJECT_NAME} PROPERTIES 
    CXX_STANDARD 23 
    CXX_EXTENSIONS OFF 
    CXX_STANDARD_REQUIRED ON
)

include(FetchContent)
FetchContent_Declare(
    saucer 
    GIT_REPOSITORY "https://github.com/saucer/saucer" 
    GIT_TAG v6.0.1
)
FetchContent_MakeAvailable(saucer)

# -------------------- SQLite3 (Windows manual path) --------------------
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

if (!MSVC)
    set(MY_PERFORMANCE_FLAGS
        -O3 -march=native -mtune=native
        -flto -ffast-math -funroll-loops
        -fomit-frame-pointer -finline-functions
        -fdata-sections -ffunction-sections
        -Wall -Wextra
    )
    target_compile_options(${PROJECT_NAME} PRIVATE ${MY_PERFORMANCE_FLAGS})
endif()
target_include_directories(${PROJECT_NAME} PRIVATE "embedded")
target_link_libraries(${PROJECT_NAME} PRIVATE saucer::saucer SQLite::SQLite3)

# ===========================================================
# CPack packaging (for Windows .zip / .exe installers)
# ===========================================================
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "MyWallet")
set(CPACK_PACKAGE_VENDOR "Enam Antoine M. Kodjoh-Kpakpassou")
set(CPACK_PACKAGE_CONTACT "eakodjoh-kpakpassou@st.ug.edu.gh")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Wallet tracker built with Saucer (C++) and Svelte frontend")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MyWallet")

# Choose generators:
if(WIN32)
    set(CPACK_GENERATOR "ZIP")  # Or "NSIS" if you install NSIS for a real installer
else()
    set(CPACK_GENERATOR "TGZ")
endif()

# Install binary
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# Install frontend assets (adjust if your frontend build folder is different)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/frontend-dist/"
        DESTINATION share/mywallet)

# License
install(FILES "${CMAKE_SOURCE_DIR}/LICENSE"
        DESTINATION share/licenses/mywallet)

include(CPack)
