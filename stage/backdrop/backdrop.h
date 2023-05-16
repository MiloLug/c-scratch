Backdrop backdrop({ 
    .costumeIndex = 1,
    .costumes = {
        {L"Default Costume", L"default.svg", 0, 0, 100, 100},
    },
});

namespace NS_backdrop {
    SpriteManager manageBackdrop(&backdrop);
}
