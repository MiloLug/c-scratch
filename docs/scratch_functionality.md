# C-Scratch

## Table of Contents

- [C-Scratch](#c-scratch)
  - [Table of Contents](#table-of-contents)
  - [Documentation - Scratch Blocks](#documentation---scratch-blocks)
  - [Scratch functions representation](#scratch-functions-representation)
    - [Motion](#motion)
    - [Looks](#looks)
    - [Sound](#sound)
    - [Events](#events)
    - [Control](#control)
    - [Sensing](#sensing)
    - [Operators](#operators)
    - [Variables](#variables)
      - [Variables declaration](#variables-declaration)
      - [Lists declaration](#lists-declaration)
      - [Variables manipulations](#variables-manipulations)
      - [Lists manipulations](#lists-manipulations)
    - [Custom blocks](#custom-blocks)

## Documentation - Scratch Blocks

All the docs here assume you have access to a `sprite` object.

Here is some function example, considering this:

```c++
Coroutine sprite_onFlagClicked(Context * ctx) {
    sprite.move(20);

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
  - general case - `sprite.switchCostumeTo(X)`
  - optimizations:
    - when you've just selected a name from the dropdown:

      `sprite.switchCostumeByName(L"selected costume name"_H)`

- [`next costume`]

    `sprite.nextCostume()`

- [`switch backdrop to (X)`]
  - the same as for sprite, but with the backdrop instead
    - `backdrop.switchCostumeTo(X)`
    - `backdrop.switchCostumeByName ...` etc.

- [`next backdrop`]

    `backdrop.nextCostume()`

- [`change size by (X)`]

    `...` NOT IMPLEMENTED

- [`set size to (X) %`]

    `...` NOT IMPLEMENTED

- [`change [X] effect by (Y)`]

    `...` NOT IMPLEMENTED

- [`set [X] effect by (Y)`]

    `...` NOT IMPLEMENTED

- [`clear graphic effect`]

    `...` NOT IMPLEMENTED

- [`show`]

    `sprite.show()`

- [`hide`]

    `sprite.hide()`

- [`go to [front] layer`]

    `SpriteManager::moveToFront(&sprite)`

- [`go to [back] layer`]

    `SpriteManager::moveToBack(&sprite)`

- [`go [forward] (Y) layers`]

    `SpriteManager::moveForward(&sprite)`

- [`go [backward] (Y) layers`]

    `SpriteManager::moveBackward(&sprite)`

- (`costume [number]`)

    `sprite.getCostumeIndex()`

- (`costume [name]`)

    `sprite.getCostumeName()`

- (`backdrop [number]`)

    `backdrop.getCostumeIndex()`

- (`backdrop [name]`)

    `backdrop.getCostumeName()`

- (`size`)

    `sprite.size`

### Sound

...

### Events

- [`when 🏳️ clicked`]

    action: `ACTION_START`

- [`when [KEY] key pressed`]

    action: `ACTION_KEYDOWN|SDL_SCANCODE_KEY` where KEY is SDL key name

- [`when this sprite clicked`]

    action: `ACTION_CLICK|sprite.actionId` where `sprite` is your sprite

- [`when [loudness|timer] > (X)`]

    action: `...` NOT IMPLEMENTED

- [`when I receive [X]`]

    action: `ACTION_MESSAGE|L"X"_A` where X is your action name

- [`broadcast (X)`]
  - general case - `Messages::broadcast(X)`
  - optimizations:
    - when you've just selected a name from the dropdown:

      `sprite.broadcast(L"selected action name"_A)`

- [`broadcast (X) and wait`]
  - general case - `cs_wait(Messages::broadcastWait(X))`
  - optimizations:
    - when you've just selected a name from the dropdown:

      `cs_wait(Messages::broadcastWait(L"selected action name"_A))`

### Control

- [`wait (X) seconds`]

    `waitFor(X)`

- [`repeat (X) ...some code`]

    `repeat (X) { ...some code; cs_yield; }`

- [`forever ...some code`]

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

    `sprite.stopOtherScripts()`

- [`when I start a clone`]

    `...` NOT IMPLEMENTED

- [since`create clone of (X)`]

    `...` NOT IMPLEMENTED

- [`delete this clone`]

    `...` NOT IMPLEMENTED

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
  - general case - `isKeyPressed(X)` where `X` is a name, variable or SDL scan-code
  - optimizations:
    - when you've just selected a key from the dropdown:

    `keyPressed[SDL_SCANCODE_KEY]`

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

- <`(STRING) contains (Y) ?`>

    `strContains(STRING, Y)`

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
Coroutine sprite_my_function() {
    sprite.move(20);

    stopThisScript();
}
```

You can add any arguments you need, but it's preferable to add sprite in any case.

Also it's better to add a template if you are going to use another arguments, since it's more efficient when mixing `Value`, `String`, numbers etc.:

```c++
template<typename T1, typename T2>
Coroutine sprite_my_function(T1 &&arg1, T2 &&arg2) {
    sprite.move(20);

    //....

    stopThisScript();
}
```

Later you can call it with `cs_wait(sprite_my_function(sprite, some value1, some value 2))`
