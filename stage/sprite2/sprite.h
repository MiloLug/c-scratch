namespace NS_sprite2 {
    Sprite sprite({
        .name = L"Sprite 2",
        .safeName = L"sprite2",
        .x = -79,
        .y = -27,
        .direction = 90,
        .costumeIndex = 1,
        .size = 100,
        .visible = false,
        .layerOrder = 1,
        .costumes = {
            {L"Costume 1", L"costume1.png", 0, 0, 95, 100},
        },
    });

    SpriteManager manageSprite(&sprite);
}
