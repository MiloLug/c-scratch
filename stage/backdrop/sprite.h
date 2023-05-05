namespace NS_backdrop {
    Backdrop backdrop({ 
        .costumeNumber = 1,
        .costumes = {
            {L"Default Costume", L"default.svg"},
        },
    });

    SpriteManager manageBackdrop(&backdrop);
}
