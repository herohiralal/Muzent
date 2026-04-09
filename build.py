import os, sys, time
from pathlib import Path
from Source.Dependencies.Panshilar import buildutils, metaprogdumpfile

SHADER_EXT_COMPUTE = '.cmpt.slang'
SHADER_EXT_TASK    = '.task.slang'
SHADER_EXT_MESH    = '.mesh.slang'
SHADER_EXT_VERT    = '.vert.slang'
SHADER_EXT_FRAG    = '.frag.slang'

def compileShader(
        inputPath:  Path,
        mode:       str, # 'spirv' or 'dxil'
        intermDir:  str,
        outputFile: str
    ) -> bool:
    # basically what we'll do is turn the input file into an intermediate file using slangc
    # - for vulkan, this is a spirv file
    # - for dx12, this is a dxil file
    #
    # then, we'll turn the intermediate file into a c11 file using file dumper

    inputFile = str(inputPath)
    dxilProfile = ''
    if False:
        dxilProfile = ''
    elif inputFile.endswith(SHADER_EXT_COMPUTE):
        dxilProfile = 'cs_6_0'
    elif inputFile.endswith(SHADER_EXT_TASK):
        dxilProfile = 'as_6_0'
    elif inputFile.endswith(SHADER_EXT_MESH):
        dxilProfile = 'ms_6_0'
    elif inputFile.endswith(SHADER_EXT_VERT):
        dxilProfile = 'vs_6_0'
    elif inputFile.endswith(SHADER_EXT_FRAG):
        dxilProfile = 'ps_6_0'
    if not dxilProfile:
        return True # not relevant

    currNsSinceUnixEpoch = time.time_ns()

    intermFile = intermDir + 'ShaderBytecodes' + inputPath.stem + '-tmpout-' + mode + '-' + str(currNsSinceUnixEpoch) + '.tmpShahdr'
    compileCommmand: list[str] = ['slangc.exe' if sys.platform == 'win32' else 'slangc', inputFile]

    if mode == 'spirv':
        compileCommmand += ['-target', 'spirv', '-profile', 'spirv_1_4']
    elif mode == 'dxil':
        compileCommmand += ['-target', 'dxil', '-profile', dxilProfile]
    elif mode == 'mtl':
        compileCommmand += ['-target', 'metal'] # TODO
    else:
        buildutils.printErr(f'Unknown shader compile mode: {mode}')
        return False

    compileCommmand += ['-entry', 'main', '-o', intermFile]

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

def recompileShaders() -> bool:
    success = True

    shaderDir = Path(FOLDER_STRUCTURE.srcDir + 'Shaders')

    for root, _, files in os.walk(shaderDir):
        for file in files:
            fullPath = Path(root) / file

            if not file.endswith('.slang'):
                continue

            # SPIR-V
            if not compileShader(
                fullPath,
                'spirv',
                FOLDER_STRUCTURE.tmpDir,
                str(fullPath).replace('.slang', '.spv.c')
            ):
                success = False

            # DXIL
            if not compileShader(
                fullPath,
                'dxil',
                FOLDER_STRUCTURE.tmpDir,
                str(fullPath).replace('.slang', '.dxil.c')
            ):
                success = False

    return success

if __name__ == '__main__':
    recompileShaders()
