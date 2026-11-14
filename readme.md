# Death Forward Game / Engine

This is a game / engine project--introspection into the "you only need C" statement, that people are starting tomake popular.

Any dependent tools i.e: `math`, `json_loading`, `allocators` are all non-vendored sources, because this is a learning project.

This whole project uses C11 standard, because that is the only one that I knew well at the time of starting this project; nothing in the new standard is really valuble in my opnion.

## Ideas

3d isometric rougelike, where death progresses the player along. If you don't die it will be harder.

### Build instructions

You must have visual studio 2026 installed on a win64 device.

```
pip install -r requirements.txt
python builld.py
```

Visual Studio can be installed [here](https://visualstudio.microsoft.com/)

Otherwise the script will resolve it's own dependencies.

## vendored sources

- [GLFW](https://www.glfw.org/download.html)
- [OPENGL](https://www.opengl.org/)
This may be replaced by FreeType2 Due to better control over variable fonts.
- [STB_Image/TTF](https://github.com/nothings/stb.git)
- [TINY_GLTF](https://github.com/syoyo/tinygltf.git)
