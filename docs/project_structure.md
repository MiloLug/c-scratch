# C-Scratch

## Table of Contents

1. [Stage (scratch project code itself)](#stage)
2. [Runtime](#runtime)

## Documentation - Project Structure

### `/stage`

> `<safe_safe_sprite_name>` - this means already processed and safe sprite name you can use for directories, variable names etc.
>
> `<Sprite Name>` - this means well.. Actual sprite name, as in scratch. It will be used also to select this sprites in operations like <`touching (...) ?`>

- `/backdrop` - there are some backdrop things. It's similar to sprites, but differs a bit.
  - `/sprite.h` - backdrop's description

    ```cpp
        namespace NS_backdrop {
            Backdrop backdrop({ 
                .costumeNumber = 1,
                .costumes = {
                    //  name            file name
                    {L"Costume 1", L"backdrop1 (1).svg"},
                    {L"Costume 2", L"Castle 2.png"},
                },
            });

            SpriteManager manageBackdrop(&backdrop);
        }
    ```

  - `/scripts.h` - the backdrop's scripts

    ```cpp
        namespace NS_backdrop {
            class Scripts {
            public:
                static Coroutine someFunction() {
                    backdrop.someMethod();

                    stopThisScript();
                }
            };

            ScriptManager bindScripts({
                {ACTION_START, {
                    {&backdrop, {
                        Scripts::someFunction,
                    }},
                }},
            });
        }
    ```

- `/<safe_sprite_name>` - some sprite's directory

  - `/sprite.h` - the sprite's description

    ```cpp
        namespace NS_<safe_sprite_name> {
            Sprite sprite({
                .name = L"<Sprite Name>",
                .safeName = L"<safe_sprite_name>",
                .x = 100,
                .y = 0,
                .width = 95,
                .height = 100,
                .direction = 135,
                .costumeNumber = 1,
                .size = 100,
                .visible = true,
                .layerOrder = 1,
                .costumes = {
                    {L"Costume 1", L"costume1.png"},
                },
            });

            SpriteManager manageSprite(&sprite);
        }
    ```

  - `/scripts.h` - this sprite's scripts

    ```cpp
        namespace NS_<safe_sprite_name> {
            class Scripts {
            public:
                static Coroutine someFunction() {
                    sprite.move(10);

                    stopThisScript();
                }
            };

            ScriptManager bindScripts({
                {ACTION_START, {
                    {&sprite, {
                        Scripts::someFunction,
                    }},
                }},
            });
        }
    ```

- `/scripts.h` - includes all the scripts and some headers for them, so they can be initialized.
- `/sprites.h` - includes all the sprites for initialization.

### `/runtime`

This is basically all the tools for running code from the `/stage`.
It includes pen implementation in the `/pen` directory and small math library in the `/math` directory. Also `Value`, `ValueArray` implementations etc. You can read more in the [internals](internals.md) section.
