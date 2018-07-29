import os

version = "0_14"

env=Environment()
env.Append(CXXFLAGS = "-std=c++11")

libraries = ["libHoney.a"]

env.AppendUnique(FRAMEWORKS = Split("OpenGL SDL2 SDL2_image SDL2_mixer SDL2_ttf"))

OneRedStar = env.Program(target = "OneRedStar",
  source = Glob("./Source/*.cpp"),
  LIBS = libraries,
  LIBPATH="Honey/%s/Library" % version,
  CPPPATH="Honey/%s/Includes" % version)

Default()

def cleanObjects(target, source, env):
  os.system("rm ./Source/*.o")

cleanup = Command("cleanObjects", [], cleanObjects)
Depends(cleanup, BUILD_TARGETS)
BUILD_TARGETS.append("cleanObjects")