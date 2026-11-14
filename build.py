import json
import os
import pathlib
import subprocess
import sys
import time
import tomllib

try:
  import colorama
  colorama.just_fix_windows_console()
except ImportError:
  print("colorama not installed. Run: pip install -r requirements.txt")
  sys.exit(1)

with open("build.toml", "rb") as f:
  cfg = tomllib.load(f)

PROJECT_NAME = cfg["project"]["name"]
SRC_DIR      = cfg["project"]["src_dir"]
BUILD_DIR    = cfg["project"]["build_dir"]
DEBUG_BUILD  = cfg["project"]["debug"]
INCLUDE_DIRS = cfg["paths"]["include_dirs"]
LIB_PATHS    = cfg["paths"]["lib_dirs"]
LIBS         = cfg["link"]["libs"]

VCVARS_PATH  = r"C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvarsall.bat"
RUNTIME_FLAG = "/MD"
GLAD_SOURCE  = "vendor/glad/src/glad.c"

RED    = "\033[91m"
YELLOW = "\033[93m"
GREEN  = "\033[92m"
RESET  = "\033[0m"


def fail(message):
  print(f"{RED}{message}{RESET}")
  sys.exit(1)


def color_output(text):
  for line in text.splitlines():
    low = line.lower()
    if "error" in low or "fatal" in low:
      print(f"{RED}{line}{RESET}")
    elif "warning" in low:
      print(f"{YELLOW}{line}{RESET}")
    else:
      print(line)


def msvc():
  if "INCLUDE" in os.environ:
    return

  if not os.path.exists(VCVARS_PATH):
    fail(f"Could not find vcvarsall.bat at: {VCVARS_PATH}")

  command = f'"{VCVARS_PATH}" x64 && set'
  proc = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True, text=True)
  for line in proc.stdout:
    if "=" in line:
      key, value = line.strip().split("=", 1)
      os.environ[key] = value
  proc.wait()
  print("Environment variables initialized via vcvarsall.bat")


def collect_sources(root):
  sources = [str(f) for f in (root / SRC_DIR).rglob("*.c")]
  if not sources:
    fail(f"Error: No .c files in {SRC_DIR}")

  glad_file = root / GLAD_SOURCE
  if glad_file.exists():
    sources.append(str(glad_file))
  return sources


def compile_flags():
  flags = [RUNTIME_FLAG, "/W4", "/std:c11"]
  if DEBUG_BUILD:
    flags += ["/DDebug", "/Od"]
  else:
    flags += ["/DNDebug", "/O2"]
  flags += [f"/I{inc}" for inc in INCLUDE_DIRS]
  return flags


def write_compile_commands(root, flags, sources):
  system_includes = []
  for path in os.environ.get("INCLUDE", "").split(";"):
    if path:
      system_includes += ["/imsvc", path]

  entries = [
    {
      "directory": str(root),
      "file": src,
      "arguments": ["cl.exe", *flags, *system_includes, "/c", src],
    }
    for src in sources
  ]

  (root / "compile_commands.json").write_text(json.dumps(entries, indent=2))


def build():
  msvc()
  root = pathlib.Path.cwd()
  build_dir = root / BUILD_DIR
  build_dir.mkdir(exist_ok=True)

  sources = collect_sources(root)
  flags = compile_flags()
  write_compile_commands(root, flags, sources)

  pdb_name = pathlib.Path(PROJECT_NAME).stem + ".pdb"

  cmd = [
    "cl.exe",
    "/nologo",
    "/MP",
    *flags,
    f"/Fe:{build_dir / PROJECT_NAME}",
    f"/Fo:{build_dir}\\",
    f"/Fd:{build_dir}\\",
    "/Zi",
    *sources,
    "/link",
    f"/PDB:{build_dir / pdb_name}",
    *[f"/LIBPATH:{root / lib_path}" for lib_path in LIB_PATHS],
    *LIBS,
  ]

  print(f"Build Mode: {'Debug' if DEBUG_BUILD else 'Release'}")
  print(f"Building with MSVC: {len(sources)} files...")

  start = time.perf_counter()
  try:
    result = subprocess.run(cmd, capture_output=True, text=True, check=False)
  except FileNotFoundError:
    fail("Error: cl.exe not found. Run this from a developer console.")
  elapsed = time.perf_counter() - start

  if result.returncode != 0:
    print(f"{RED}Build failed after {elapsed:.2f}s{RESET}")
    color_output(result.stdout)
    color_output(result.stderr)
    sys.exit(1)

  if result.stdout.strip():
    color_output(result.stdout)
  print(f"{GREEN}Built successfully! {PROJECT_NAME} in {elapsed:.2f}s{RESET}")


if __name__ == "__main__":
  build()
