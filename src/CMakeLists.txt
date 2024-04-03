set(BUILD_TARGET "DirectX_exe")

set(SOURCE_FILES
    main.cpp Application.h Application.cpp Dx12Wrapper.h Dx12Wrapper.cpp io.h io.cpp renderer.h renderer.cpp)

#実行ファイル生成
add_executable(${BUILD_TARGET})
target_sources(${BUILD_TARGET} PUBLIC ${SOURCE_FILES})

#DirectXTex.hのインクルードのための設定
target_include_directories(${BUILD_TARGET} PUBLIC /Libraries/DirectXTex-main/DirectXTex)
target_link_directories(${BUILD_TARGET} PUBLIC /Libraries/DirectXTex-main/DirectXTex/Bin/Desktop_2022_Win10/x64/Debug)
target_link_libraries(${BUILD_TARGET} DirectXTex)
