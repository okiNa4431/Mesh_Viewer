# HLSLファイルのコンパイル設定
set_source_files_properties(VertexShader.hlsl 
PROPERTIES 
VS_SHADER_TYPE Vertex 
VS_SHADER_ENTRYPOINT BasicVS
VS_SHADER_MODEL 5.0 )
set_source_files_properties(PixelShader.hlsl 
PROPERTIES 
VS_SHADER_TYPE Pixel
VS_SHADER_ENTRYPOINT BasicPS
VS_SHADER_MODEL 5.0 )