import os, sys, time
from pathlib import Path
from Source.Dependencies.Panshilar import buildutils, metaprogdumpfile

FOLDER_STRUCTURE = buildutils.getFolderStructure(os.path.dirname(os.path.abspath(__file__)))
MAIN_FILE_C   = FOLDER_STRUCTURE.srcDir + 'zzzz_Unity.c'
MAIN_FILE_CXX = FOLDER_STRUCTURE.srcDir + 'zzzz_Unity.cpp'

DXC_PATH = os.path.join(
    FOLDER_STRUCTURE.depDir, 'dxc', 'bin',
    'linux-x64' if sys.platform == 'linux' else 'win-x64' if sys.platform == 'win32' else '',
    'dxc' if sys.platform == 'linux' else 'dxc.exe' if sys.platform == 'win32' else ''
).replace('\\', '/') if sys.platform == 'linux' or sys.platform == 'win32' else ''

SHADER_EXT_COMPUTE = '.cmpt.hlsl'
SHADER_EXT_TASK    = '.task.hlsl'
SHADER_EXT_MESH    = '.mesh.hlsl'
SHADER_EXT_VERT    = '.vert.hlsl'
SHADER_EXT_FRAG    = '.frag.hlsl'

def compileShader(
        inputPath:  Path,
        mode:       str, # 'spirv' or 'dxil'
        intermDir:  str,
        outputFile: str
    ) -> bool:
    # basically what we'll do is turn the input file into an intermediate file using dxc
    # - for vulkan, this is a spirv file
    # - for dx12, this is a dxil file
    #
    # then, we'll turn the intermediate file into a c11 file using file dumper

    inputFile = str(inputPath)
    profile = ''
    useSpvExt = False
    if False:
        profile = ''
    elif inputFile.endswith(SHADER_EXT_COMPUTE):
        profile = 'cs_6_0'
    elif inputFile.endswith(SHADER_EXT_TASK):
        profile = 'as_6_5'
        useSpvExt = True
    elif inputFile.endswith(SHADER_EXT_MESH):
        profile = 'ms_6_5'
        useSpvExt = True
    elif inputFile.endswith(SHADER_EXT_VERT):
        profile = 'vs_6_0'
    elif inputFile.endswith(SHADER_EXT_FRAG):
        profile = 'ps_6_0'

    if not profile:
        return True # not relevant

    currNsSinceUnixEpoch = time.time_ns()

    intermFile = intermDir + 'ShaderBytecodes' + inputPath.stem + '-tmpout-' + mode + '-' + str(currNsSinceUnixEpoch) + '.tmpShahdr'
    compileCommmand: list[str] = [DXC_PATH, inputFile, '-T', profile, '-E', 'main']

    if mode == 'spirv':
        compileCommmand += (['-spirv', '-fspv-target-env=vulkan1.3']) + \
                           (['-fspv-extension=SPV_EXT_mesh_shader'] if useSpvExt else [])

    elif mode == 'dxil':
        pass # nothing extra needed

    elif mode == 'mtl':
        buildutils.printErr("unimplemented mtl shader compiler backend")

    else:
        buildutils.printErr(f'Unknown shader compile mode: {mode}')
        return False

    compileCommmand += ['-Fo', intermFile]

    if not buildutils.runCommand(
        compileCommmand,
        f'{inputPath.name} {mode} Shader Compile'
    ):
        return False

    metaprogdumpfile.run(inputPath.name, intermFile, outputFile)
    os.remove(intermFile)
    return True

def recompileShaders() -> bool:
    success = True

    shaderDir = Path(FOLDER_STRUCTURE.srcDir + 'Shaders')

    for root, _, files in os.walk(shaderDir):
        for file in files:
            fullPath = Path(root) / file

            if not file.endswith('.hlsl'):
                continue

            # SPIR-V
            if not compileShader(
                fullPath,
                'spirv',
                FOLDER_STRUCTURE.tmpDir,
                str(fullPath).replace('.hlsl', '.spv.c')
            ):
                success = False

            # DXIL
            if not compileShader(
                fullPath,
                'dxil',
                FOLDER_STRUCTURE.tmpDir,
                str(fullPath).replace('.hlsl', '.dxil.c')
            ):
                success = False

    return success

if __name__ == '__main__':
    recompileShaders()
