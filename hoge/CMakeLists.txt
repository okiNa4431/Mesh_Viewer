set(BUILD_TARGET "hoge_exe")
set(CMAKE_SYSTEM_NAME Windows)

#実行ファイル生成
add_executable(${BUILD_TARGET})
target_sources(${BUILD_TARGET} PUBLIC main.cpp)

set_target_properties(${BUILD_TARGET} PROPERTIES LINK_FLAGS "/ENTRY:wWinMainCRTStartup")