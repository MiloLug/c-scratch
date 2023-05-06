# C-Scratch

## Table of Contents

1. [Motion](#motion)
2. [Control](#control)
3. [Sensing](#sensing)
4. [Operators](#operators)
5. [Variables](#variables)

## Documentation - Scratch Blocks

All the docs here assume you have `spriteA` and `spriteB` sprites. And your current sprite is `spriteA`.

Usually all your functions receive a sprite object pointer, named `sprite`.

### Such a function example

```c++
Coroutine sprite_onFlagClicked(Sprite * sprite) {
    sprite->move(20);

    stopThisScript();
}
```

## Scratch functions representation

> `&sprite` means that you have to pass a sprite pointer

### Motion

- [`move (X) steps`]

    `sprite->move(X)`

- [`turn right (X) degrees`]

    `sprite->turnRight(X)`

- [`turn left (X) degrees`]

    `sprite->turnLeft(X)`

- [`go to (random position)`]

    `sprite->goToRandomPosition()`

- [`go to (mouse-pointer)`]

    `sprite->goToPointer()`

- [`go to (sprite)`]

    `sprite->goToSprite(&sprite)`

- [`go to x: (X) y: (Y)`]

    `sprite->goXY(X, Y)`

- [`glide (T) secs to (random position)`]

    `cs_wait(sprite->glideToRandomPosition(T))` NOT IMPLEMENTED

- [`glide (T) secs to (mouse-pointer)`]

    `cs_wait(sprite->glideToPointer(T))` NOT IMPLEMENTED

- [`glide (T) secs to (sprite)`]

    `cs_wait(sprite->glideToSprite(&sprite))` NOT IMPLEMENTED

- [`glide (T) secs to x: (X) y: (Y)`]

    `cs_wait(sprite->glideXY(T, X, Y))` NOT IMPLEMENTED

- [`point in direction (X)`]

    `sprite->point(X)`

- [`point towards (mouse-pointer)`]

    `sprite->pointTowardsPointer()`

- [`point towards (sprite)`]

    `sprite->pointTowardsSprite(&sprite)`

- [`change x by (N)`]

    `sprite->changeX(N)`

- [`set x to (N)`]

    `sprite->setX(N)`

- [`change y by (N)`]

    `sprite->changeY(N)`

- [`set y to (N)`]

    `sprite->setY(N)`

- [`if on edge, bounce`]

    `sprite->edgeBounce()` NOT IMPLEMENTED

- [`set rotation style [left-right]`]

    `sprite->setRotationStyle(Sprite::ROT_LEFT_RIGHT)` NOT IMPLEMENTED

- [`set rotation style [left-right]`]

    `sprite->setRotationStyle(Sprite::ROT_NONE)` NOT IMPLEMENTED

- [`set rotation style [left-right]`]

    `sprite->setRotationStyle(Sprite::ROT_ALL_AROUND)` NOT IMPLEMENTED

- (`x position`)

    `sprite->x`

- (`y position`)

    `sprite->y`

- (`direction`)

    `sprite->getDirection()`

### Looks

- [`say (X)`]

    `...` NOT IMPLEMENTED

- [`think (X) for (Y) seconds`]

    `...` NOT IMPLEMENTED

- [`think (X)`]

    `...` NOT IMPLEMENTED

- [`switch costume to (X)`]

    `...` NOT IMPLEMENTED

- [`say (X) for (Y) seconds`]

    `...` NOT IMPLEMENTED

- [`say (X) for (Y) seconds`]

    `...` NOT IMPLEMENTED

- [`say (X) for (Y) seconds`]

    `...` NOT IMPLEMENTED

### Sound

...

### Events

...

### Control

- [`wait (X) seconds`]

    `...` NOT IMPLEMENTED

- [`repeat (X) ...some code`]

    `repeat (X) { ...some code; cs_yield; }`

- \[`forever ...some code`\]

    `forever { ...some code; cs_yield; }`

- [`wait until <expression>`]

    `waitUntil(expression)`

- [`repeat until <expression> ...some code`]

    `repeatUntil(expression) { ...some code; cs_yield; }`

- [`stop [all]`]

    `stopAll()`

- [`stop [this script]`]

    `stopThisScript()`

- [`stop [other scripts in sprite]`]

    `...` NOT IMPLEMENTED

> No clone functionality rn

### Sensing

- <`touching (mouse-pointer) ?`>

    `sprite->isTouchingPointer()`

- <`touching (edge) ?`>

    `sprite->isTouchingEdge()` NOT IMPLEMENTED

- <`touching (sprite) ?`>

    `sprite->isTouchingSprite(&sprite)` NOT IMPLEMENTED

- (`distance to (mouse-pointer)`)

    `sprite->getPointerDistance()`

- (`distance to (sprite)`)

    `sprite->getSpriteDistance(&sprite)`

- <`key (KEY) pressed?`>

    `keyPressed[SDL_SCANCODE_KEY]` where KEY is SDL key name

- <`mouse down?`>

    `mouseState.isButtonDown`

- (`mouse x`)

    `mouseState.x`

- (`mouse y`)

    `mouseState.y`

- (`timer`)

    `timer`

- [`reset time`]

    `resetTimer()`

### Operators

- `+ - * /`
- `< >`, `=` is `==`
- `and` as `&&`, `or` as `||`, `not <expr>` as `!expr`

- (`pick random (X) to (Y)`)

    `randInRange(X, Y)`

- (`join (X) (Y)`)

    `join(X, Y)`

- (`letter (X) of (Y)`)

    `letterOf(Y, X)`

- (`length of (X)`)

    `lengthOf(X)`

- <`(X) contains (Y) ?`>

    `...` NOT IMPLEMENTED

- <`(X) mod (Y)`>

    `fmod(X, Y)`

- <`round (X)`>

    `round(X)`

### Variables

#### Variables declaration

- `Value varName1;`
- `Value varName2 = L"text";`
- `Value varName3 = 23;`

#### Lists declaration

- `ValueArray arr1;`
- `ValueArray arr1({1, 2, L"text", varName1, 33});`

#### Variables manipulations

- [`set [varName] to (X)`]

    `varName = X`

- [`change [varName] by (X)`]

    `varName += X`

    or

    `varName -= X`

> No interface to show or hide rn

#### Lists manipulations

- [`add (X) to [listName]`]

    `listName.push(X)`

- [`delete (I) of [listName]`]

    `listName.remove(I)`

- [`delete all of [listName]`]

    `listName.clean()`

- [`insert (X) at (I) of [listName]`]

    `listName.insert(I, X)`

- [`replace item (I) of [listName] with (X)`]

    `listName.set(I, X)`

- [`item (I) of [listName]`]

    `listName.get(I)`

- [`item # of (X) in [listName]`]

    `listName.indexOf(X)`

- [`length of [listName]`]

    `listName.length`

- <`[listName] contains (X) ?`>

    `listName.contains(X)`

> No interface to show or hide rn

### Custom blocks

Just create a function like

```c++
Coroutine sprite_my_function(Sprite * sprite) {
    sprite->move(20);

    stopThisScript();
}
```

You can add any arguments you need, but it's preferable to add sprite in any case.

Also it's better to add a template if you are going to use another arguments, since it's more efficient when mixing `Value`, `String`, numbers etc.:

```c++
template<typename T1, typename T2>
Coroutine sprite_my_function(Sprite * sprite, T1 &&arg1, T2 &&arg2) {
    sprite->move(20);

    //....

    stopThisScript();
}
```

Later you can call it with `cs_wait(sprite_my_function(sprite, some value1, some value 2))`
