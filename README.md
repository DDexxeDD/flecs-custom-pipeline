# Flecs Custom Pipline

A simple example of a custom pipeline in flecs.  I wanted to play with custom pipelines and the examples included with flecs didn't really cover enough for me.  Hopefully this little demo can help you too :)

## Building

You will need [meson](https://mesonbuild.com/Getting-meson.html) to build this project.

**Build**


```
meson setup build
meson compile -C build
```

The executable "`pipeline`" will be created in the `build` directory.


## License

MIT License

Since most of this is just pulled out of the flecs code I figured this should just have the flecs license.  You can do whatever you want with whatever I contributed.
