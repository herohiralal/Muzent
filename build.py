import os, sys, time
from pathlib import Path
from Source.Dependencies.Panshilar import buildutils, metaprogdumpfile

SHADER_COMPILE_MODE_VK   = 'vk'
SHADER_COMPILE_MODE_DX12 = 'dx12'
SHADER_COMPILE_MODE_MTL  = 'mtl'

def compileShader(
        mode: str,
        inputFile: str,
        entryPts: list[str],
        intermDir: str,
        outputFile: str
    ) -> bool:
    # basically what we'll do is turn the input file into an intermediate file using slangc
    # - for vulkan, this is a spirv file
    # - for dx12, this is a dxil file
    #
    # then, we'll turn the intermediate file into a c11 file using file dumper

    currNsSinceUnixEpoch = time.time_ns()

    inputPath = Path(inputFile)
    intermFile = intermDir + 'ShaderBytecodes' + inputPath.stem + '-tmpout-' + mode + '-' + str(currNsSinceUnixEpoch) + '.tmpShahdr'
    compileCommmand: list[str] = ['slangc.exe' if sys.platform == 'win32' else 'slangc', inputFile]
    if mode == SHADER_COMPILE_MODE_VK:
        compileCommmand += ['-target', 'spirv', '-profile', 'spirv_1_4', '-emit-spirv-directly', '-fvk-use-entrypoint-name']
        for ep in entryPts:
            compileCommmand += ['-entry', ep]
    elif mode == SHADER_COMPILE_MODE_DX12:
        compileCommmand += ['-target', 'dxil'] # TODO
    elif mode == SHADER_COMPILE_MODE_MTL:
        compileCommmand += ['-target', 'metal'] # TODO
    else:
        buildutils.printErr(f'Unknown shader compile mode: {mode}')
        return False
    compileCommmand += ['-o', intermFile]

    if not buildutils.runCommand(
        compileCommmand,
        f'{inputPath.name} {mode} Shader Compile'
    ):
        return False

    metaprogdumpfile.run(inputPath.name, intermFile, outputFile)
    os.remove(intermFile)
    return True

FOLDER_STRUCTURE = buildutils.getFolderStructure(os.path.dirname(os.path.abspath(__file__)))
MAIN_FILE_C   = FOLDER_STRUCTURE.srcDir + 'zzzz_Unity.c'
MAIN_FILE_CXX = FOLDER_STRUCTURE.srcDir + 'zzzz_Unity.cpp'

TRIANGLE_SLANG_FILE     = FOLDER_STRUCTURE.srcDir + 'Shaders/triangle.slang'
TRIANGLE_SPIRV_FILE     = FOLDER_STRUCTURE.tmpDir + 'Shaders/triangle.spv'
TRIANGLE_SPIRV_SRC_FILE = FOLDER_STRUCTURE.srcDir + 'Shaders/triangle_spv.c'
TRIANGLE_DXIL_SRC_FILE  = FOLDER_STRUCTURE.srcDir + 'Shaders/triangle_dxil.c'

def recompileShaders() -> bool:
    success = True

    if not compileShader(SHADER_COMPILE_MODE_VK, TRIANGLE_SLANG_FILE, ['vertMain', 'fragMain'], FOLDER_STRUCTURE.tmpDir, TRIANGLE_SPIRV_SRC_FILE):
        success = False

    return success

if __name__ == '__main__':
    recompileShaders()
