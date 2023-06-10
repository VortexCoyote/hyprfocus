SOURCE_FILES=$(wildcard src/*.cpp src/*.hpp)

all:
	$(CXX) -shared -fPIC --no-gnu-unique $(SOURCE_FILES)  -o hyprfocus.so -g `pkg-config --cflags pixman-1 libdrm hyprland` -std=c++2b
clean:
	rm ./hyprfocus.so
