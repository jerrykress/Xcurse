# Xcurse

### Custom terminal graphics library mimicking ncurses. still under construction...

This is a simple terminal user interface library that I'm developing for my other TUI applications. It uses xterm alternate screen buffer and uses wide characters to support UTF-8. Some nice features include: responsive sizing, styling, configurable refresh rate, input support.

[Updated Documentation](https://jerrykress.github.io/Xcurse/html/index.html)

## Project Structure

This project is structured mainly using a mixture of strategy pattern and template pattern. The intention is to avoid the need for any modification to the display class when implementing a new display component while also taking advantage of runtime polymorphism.

## Main Components

### Display Class

A singleton class that is responsible for all the I/O and setup process. It contains only one instance of the display, sets terminal locale, spawns helper threads and refreshs the screen at an interval that can be configured by the user. It also manages all the objects that show on the screen.

### BaseDisplayObject Class

This is an interface that the display recognises and any object class has to be implement it in order to appear on the display. It contains a very important `draw()` function which allows the object to modify a designated portion of the screen space however it sees fit. Some example classes derived from this class include windows and widgets.

### Layout Class

This works in a similar way that div does in html. It divides the available screen space into rows and columns and is the basis of the rendering process. A layout can hold any BaseDisplayObject as well as other layouts, which will be rendered recursively. The size of a layout can either be fixed or adaptive.

Any program will contain a single base layout called "root". It is the entry point of rendering process and cannot be deleted.

### Pixel

How each character is stored internally. It contains the raw characters and styling information.

### Position, Size

Important helper structs that are used to organise all objects.

### Stylable

A class that provides styling attributes for any member that inherits from it.

## Example Snippets

- Configure display
- Configure layouts
- Adding and getting objects
- Mapping keypress to actions

## Compilation

When used in other projects, compile the files used in your project and link to POSIX thread library.

```
# Add library files

file(GLOB src_files CONFIGURE_DEPENDS
     "Xcurse/src/*.cpp"
     "Xcurse/src/Common/*.cpp"
     "Xcurse/src/Display/*.cpp"
     "Xcurse/src/Utils/*.cpp"
     "Xcurse/src/Widgets/*.cpp"
     "Xcurse/src/Window/*.cpp"
     "Xcurse/src/Shapes/*.cpp"
     "src/Background/*.cpp"
     "src/Daemon/*.cpp"
     "src/Font/*.cpp"
     "src/Core/*.cpp"
)
add_executable(Clock src/Core/Clock.cpp ${src_files})

# Link to pthread library

TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)
```

## Example Projects

- Stock tracker: https://github.com/jerrykress/Stonks
- Countdown timer: https://github.com/jerrykress/ASCII-Clock
