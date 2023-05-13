# C-Scratch

## Table of Contents

- [C-Scratch](#c-scratch)
  - [Table of Contents](#table-of-contents)
  - [Documentation - About Stage](#documentation---about-stage)
  - [Sprites](#sprites)
    - [Declaring a new sprite](#declaring-a-new-sprite)
  - [Scripts](#scripts)
    - [Coroutines](#coroutines)
    - [Actions](#actions)
    - [Calling other coroutines](#calling-other-coroutines)
    - [Messaging](#messaging)
    - [Literal suffixes](#literal-suffixes)

## Documentation - About Stage

## Sprites

### Declaring a new sprite

To declare a sprite, you have to specify all the basic sprite information.
And then just add it to the manager with `SpriteManager`'s constructor or by calling
`SpriteManager::add(&sprite)`:

> All the values correspond to Scratch's logic, so indexes are 1 based and the direction is 90deg based.

```cpp
Sprite sprite({
    .name = L"Sprite Name",
    .safeName = L"Sprite_Name",
    .x = 20,
    .y = 20,
    .direction = 90,
    .costumeIndex = 1,
    .size = 100,
    .visible = false,
    .layerOrder = 1,
    .costumes = {
        //  name            file name        pX, pY, width, height
        {L"Costume 1", L"backdrop1 (1).svg",  0,  0,  10,    20},
        {L"Costume 2", L"Castle 2.png",       0,  0,  10,    20},
    },
});

SpriteManager manageSprite(&sprite);
```

Where `pX` and `pY` are coordinates of the costume's pivot relative to it's _real_ center.

E.g. if `width` = 10 and `height` = 20, then coordinates of the pivot placed in top left corner will be `pX` = -5, `pY` = 10.

## Scripts

### Coroutines

Everything's made of coroutines. E.g. every part of the script that starts from a hat block (`define`, [`when 🏳️ clicked`] etc.) is a separate coroutine. In the end they are just some functions, running in parallel.

Well, not exactly in parallel, but if you don't know about async or coroutines, just accept this. You can read more in the [internals](internals.md).

### Actions

Actions are just some binding for the coroutines. For example, you can create a coroutine and say: `I want this coroutine to run when 🏳️ clicked`.

Also you have to say what sprite this coroutine is related to, as the scripts manager has to know it to handle such things as [`stop [other scripts in sprite]`].

And eventually, there are "subtypes" of actions. Like when you have an action like [`when [KEY] key pressed`]. It would be ridiculous to add separate actions for all possible keys. So there is one base action `ACTION_KEYDOWN`, and you can modify it by simply mixing an SDL key code:
`ACTION_KEYDOWN|SDL_SCANCODE_SPACE`.

There is an example of some bindings and coroutines:

```cpp
// Coroutines used for actions all need this context for some cases.
// Well, it was to hard to store both context and context-less coroutines in the bindings.
// So just add it and don't care unless you don't need it.
Coroutine testCoro(Context * ctx) {
    forever {
        wprintf(L"testCoro runs now\n");

        // Just add yield to your loops, so they don't block other coroutines
        cs_pass;
    }

    // Stop in the end of functions no matter what, so you don't have to care about whether you have or not any yields. Since a coroutine will be corrupted without cs_stop/yield/etc.
    cs_stop;
}

// This ScriptManager's constructor actually calls ScriptManager::bindScripts
// and it's convenient when you need to bind them somewhere before main() execution.
// But you can also use ScriptManager::bindScripts(...) directly
ScriptManager bindings({
    {ACTION_START, {  // specify an action
        {&sprite, {  // then sprite
            testCoro,  // and now some handlers for the action.
            testCoro,  // you can use the same coro more than once... And it will run several times simultaneously. 
        }},
    }},
});
```

### Calling other coroutines

It's very human.. Sorry.
So just do `cs_wait yourCoroutine(maybe some arguments)`.

Unfortunately, you can't just call it without this `cs_wait`, since all coroutines are not executed until they are waited.

### Messaging

Here messages are just subtypes of `ACTION_MESSAGE` action. So to register a message handler, you have to simply bind it along with others coroutines.

Here's an example:

```cpp
Coroutine onStart(Context * ctx) {
    Messages::broadcast(L"some message"_A);
    cs_wait Messages::broadcastWait(L"some message"_A);

    cs_stop;
}

Coroutine onSomeMessage(Context * ctx) {
    // Every message handler coroutine needs `Messages::Handler` definition
    // so that it can say "I'm finished" no matter what.
    // You may need this for things like waiting for all the handlers to stop.
    Messages::Handler handler(ctx);

    forever {
        wprintf(L"onSomeMessage runs now\n");
        cs_pass;
    }

    cs_stop;
}

ScriptManager bindings({
    // you can read about _A in "Literal suffixes"
    {ACTION_MESSAGE|L"some message"_A, {
        {&sprite, {
            onSomeMessage,
        }},
    }},
});
```

### Literal suffixes

- `_A`: it actually means "action". This string suffix allows to hash the sting (e.g. make it some unique number) and prepare this number, so it can be used as an action parameter (can be safely mixed to it).

- `_H`: returns almost the same hash as `_A`, but it's not prepared for safe action usage. In fact, this suffix will be used in any other situations that doesn't involve actions.

- `_S`: returns the literal (a string), wrapped in `String()` object

- `_K`: returns SDL scan code for the given name. 0 if there is no such a key
