set(BUILD_TARGET "hoge_exe")

#実行ファイル生成
add_executable(${BUILD_TARGET})
target_sources(${BUILD_TARGET} PUBLIC main.cpp)