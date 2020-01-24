ren [Shader/]vert.spv Shader/old_vert.spv
ren [Shader/]frag.spv Shader/old_frag.spv

C:\VulkanSDK\1.1.130.0\Bin32\glslc.exe Shader/shader.vert -o Shader/vert.spv
C:\VulkanSDK\1.1.130.0\Bin32\glslc.exe Shader/shader.frag -o Shader/frag.spv

pause