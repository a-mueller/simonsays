add_executable(simonsays
        main.cpp
        songs.hpp
        keypad_map.hpp
        io.hpp
        config.hpp
        io.cpp
        game.hpp
        game.cpp
)

target_link_libraries(simonsays
        pico_stdlib
        hardware_i2c
        pico_neotrellis
        pico_tone
)

pico_enable_stdio_usb(simonsays 1)
pico_enable_stdio_uart(simonsays 1)
pico_add_extra_outputs(simonsays)

