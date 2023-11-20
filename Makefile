SOURCE_FILES=$(wildcard src/*.cpp)

all:
	$(CXX) -O2 -shared -fPIC --no-gnu-unique $(SOURCE_FILES)  -o hyprfocus.so `pkg-config --cflags pixman-1 libdrm hyprland` -std=c++2b
	strip hyprfocus.so

clean:
	rm ./hyprfocus.so
