import pathlib
import subprocess
import os
import sys

PROJECT_NAME = "app.exe"
SRC_DIR = "src"
INCLUDE_DIRS = ["include", "vendor/glfw/include", "vendor/glad/include"]
LIB_PATHS = ["vendor/glfw/lib-vc2026"]
DEBUG_BUILD = True
LIBS = [
  "glfw3.lib",
  "opengl32.lib",
  "user32.lib",
  "kernel32.lib", 
  "gdi32.lib",
  "shell32.lib"
] # .lib

RUNTIME_FLAG = "/MD"

def msvc():
  """Finds and runs the msvc setup script."""
  if "INCLUDE" in os.environ:
    return # Already setup.

  vsvars_path = r"C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvarsall.bat"

  if not os.path.exists(vsvars_path):
    print("Could not find vcvarsall.bat at: ", vsvars_path)
    return

  command = f'"{vsvars_path}" x64 && set'
  proc = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True, text=True)
  for line in proc.stdout:
    if '=' in line:
      key, value = line.strip().split('=', 1)
      os.environ[key] = value
  print("Environment variables initialized via vcvarsall.bat")
  
def build():
  msvc()
  root = pathlib.Path.cwd()
  src_dir = root / SRC_DIR

  sources = [str(f) for f in src_dir.rglob("*.c")]
  if not sources:
    print(f"Error: No .c files in {SRC_DIR}")
    return
  
  cmd = [
    "cl.exe",
    "/nologo",
    RUNTIME_FLAG,
    "/W4",
    "/std:c11",
    "/Fe:" + PROJECT_NAME,
    "/Zi", # PDB
  ]

  # COMPILE DEFS
  if DEBUG_BUILD:
    cmd.append("/DDebug")
    print("Build Mode: Debug")
  else:
    cmd.append("/DNDebug")
    print("Build mode: RELEASE")
  
  for inc in INCLUDE_DIRS:
    cmd.append(f"/I{inc}")

  sources = [str(f) for f in (root / "src").rglob("*.c")]
  #GLAD
  glad_file = root / "vendor/glad/src/glad.c"
  if glad_file.exists():
    sources.append(str(glad_file))
  cmd.extend(sources)

  cmd.append("/link")

  for lib_path in LIB_PATHS:
    cmd.append(f"/LIBPATH:{str(root / lib_path)}")
  
  cmd.extend(LIBS) # linker should automatically search for .lib
  
  print(f"Build with MSVC: {len(sources)} files...")

  try:
    result = subprocess.run(cmd, capture_output=True, text=True, check=False)

    if result.returncode == 0:
      print(f"Built successfully! {PROJECT_NAME}")
    else:
      print("Build failed!")
      print(result.stdout)
      print(result.stderr)

  except FileNotFoundError:
    print("Error: cl.exe not found. Run the from the developer console.")
    sys.exit(1)

if __name__ == "__main__":
  build()
