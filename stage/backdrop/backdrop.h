Backdrop backdrop({ 
    .costumeIndex = 1,
    .costumes = {
        {L"Default Costume", L"default.svg"},
    },
});

namespace NS_backdrop {
    SpriteManager manageBackdrop(&backdrop);
}
