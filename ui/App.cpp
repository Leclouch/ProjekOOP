#include "App.h"

#include <algorithm>
#include <vector>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

namespace {

// Warna tema (diambil dari palet sprite)
const Color COL_BG      = {32, 28, 48, 255};
const Color COL_PANEL   = {58, 56, 74, 255};
const Color COL_HOVER   = {80, 78, 104, 255};
const Color COL_OUTLINE = {26, 16, 32, 255};
const Color COL_TEXT    = {244, 244, 236, 255};
const Color COL_DIM     = {150, 152, 172, 255};
const Color COL_GOLD    = {240, 192, 64, 255};
const Color COL_RED     = {200, 40, 56, 255};
const Color COL_HP      = {56, 140, 72, 255};
const Color COL_MANA    = {40, 80, 184, 255};
const Color COL_SKY     = {72, 96, 140, 255};
const Color COL_GROUND  = {40, 104, 64, 255};

// Lokasi file aset, relatif terhadap folder game.exe
std::string assetPath(const std::string& file) {
    return std::string(GetApplicationDirectory()) + "assets/" + file;
}

// Muat gambar jika filenya ada. Jika tidak ada, id tekstur = 0.
Texture2D loadTextureSafe(const std::string& file) {
    Texture2D tex = {};
    std::string path = assetPath(file);
    if (FileExists(path.c_str())) {
        tex = LoadTexture(path.c_str());
        SetTextureFilter(tex, TEXTURE_FILTER_POINT);
    }
    return tex;
}

// Muat suara jika filenya ada. Jika tidak ada, frameCount = 0.
Sound loadSoundSafe(const std::string& file) {
    Sound snd = {};
    std::string path = assetPath(file);
    if (FileExists(path.c_str())) {
        snd = LoadSound(path.c_str());
    }
    return snd;
}

}  // namespace

// ===========================================================================
// Siklus hidup App
// ===========================================================================
App::App()
    : screen(Screen::SelectHero), battle(nullptr), heroIndex(0),
      heroFlash(0), enemyFlash(0), resultTimer(0) {
    InitWindow(VIRTUAL_W * 4, VIRTUAL_H * 4, "Game Simple - Turn Based RPG");
    InitAudioDevice();
    SetTargetFPS(60);

    // Semua digambar ke tekstur kecil 320x180, lalu diperbesar tanpa blur
    target = LoadRenderTexture(VIRTUAL_W, VIRTUAL_H);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    loadAssets();
    setupGuiStyle();
    loadHeroInfo();
}

App::~App() {
    delete battle;          // aman walau battle bernilai nullptr
    battle = nullptr;
    unloadAssets();
    UnloadRenderTexture(target);
    CloseAudioDevice();
    CloseWindow();
}

void App::loadAssets() {
    // Font pixel; jika tidak ada pakai font bawaan raylib
    std::string fontPath = assetPath("fonts/PressStart2P.ttf");
    customFont = FileExists(fontPath.c_str());
    if (customFont) {
        font = LoadFontEx(fontPath.c_str(), 8, nullptr, 0);
        SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);
        fontSize = 8;
        fontSpacing = 0;
    } else {
        font = GetFontDefault();
        fontSize = 10;
        fontSpacing = 1;
    }

    heroTex[0] = loadTextureSafe("sprites/warrior.png");
    heroTex[1] = loadTextureSafe("sprites/mage.png");
    heroTex[2] = loadTextureSafe("sprites/archer.png");
    enemyTex[0] = loadTextureSafe("sprites/slime.png");
    enemyTex[1] = loadTextureSafe("sprites/goblin.png");
    enemyTex[2] = loadTextureSafe("sprites/dragon.png");

    sndClick     = loadSoundSafe("sound/click.wav");
    sndAttack    = loadSoundSafe("sound/attack.wav");
    sndSkill     = loadSoundSafe("sound/skill.wav");
    sndHeal      = loadSoundSafe("sound/heal.wav");
    sndEnemyDown = loadSoundSafe("sound/enemy_down.wav");
    sndWin       = loadSoundSafe("sound/win.wav");
    sndLose      = loadSoundSafe("sound/lose.wav");
}

void App::unloadAssets() {
    if (customFont) UnloadFont(font);
    for (int i = 0; i < 3; i++) {
        if (heroTex[i].id != 0) UnloadTexture(heroTex[i]);
        if (enemyTex[i].id != 0) UnloadTexture(enemyTex[i]);
    }
    Sound* sounds[] = {&sndClick, &sndAttack, &sndSkill, &sndHeal, &sndEnemyDown, &sndWin, &sndLose};
    for (Sound* s : sounds) {
        if (s->frameCount > 0) UnloadSound(*s);
    }
}

void App::setupGuiStyle() {
    GuiSetFont(font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
    GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 1);

    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(COL_OUTLINE));
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(COL_PANEL));
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(COL_TEXT));
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt(COL_GOLD));
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(COL_HOVER));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(COL_GOLD));
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, ColorToInt(COL_GOLD));
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, ColorToInt(COL_GOLD));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(COL_OUTLINE));
    GuiSetStyle(DEFAULT, BORDER_COLOR_DISABLED, ColorToInt(COL_OUTLINE));
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(COL_BG));
    GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, ColorToInt(COL_DIM));
}

// Baca statistik tiap hero lewat BattleManager sementara (langsung dihapus lagi)
void App::loadHeroInfo() {
    const HeroType types[3] = {HeroType::Warrior, HeroType::Mage, HeroType::Archer};
    for (int i = 0; i < 3; i++) {
        BattleManager preview(types[i]);
        const Hero& h = preview.getHero();
        heroes[i] = {types[i], h.getName(), h.getRole(), h.getSkillName(),
                     h.getMaxHp(), h.getAttackPower(), h.getMaxMana(), h.getSkillCost()};
    }
}

void App::startGame(int index) {
    delete battle;                              // jaga-jaga jika masih ada game lama
    battle = new BattleManager(heroes[index].type);
    heroIndex = index;
    heroFlash = enemyFlash = resultTimer = 0;
    screen = Screen::Battle;
}

void App::endGame() {
    delete battle;
    battle = nullptr;
    screen = Screen::SelectHero;
}

// ===========================================================================
// Loop utama
// ===========================================================================
void App::run() {
    while (!WindowShouldClose()) {
        // Skala bulat terbesar yang muat di jendela (1280x720 -> 4x)
        int scale = std::min(GetScreenWidth() / VIRTUAL_W, GetScreenHeight() / VIRTUAL_H);
        if (scale < 1) scale = 1;
        int offsetX = (GetScreenWidth() - VIRTUAL_W * scale) / 2;
        int offsetY = (GetScreenHeight() - VIRTUAL_H * scale) / 2;

        // Ubah posisi mouse ke koordinat layar virtual 320x180
        SetMouseOffset(-offsetX, -offsetY);
        SetMouseScale(1.0f / scale, 1.0f / scale);

        // 1. Gambar game ke layar virtual
        BeginTextureMode(target);
        ClearBackground(COL_BG);
        switch (screen) {
            case Screen::SelectHero: selectScreen(); break;
            case Screen::Battle:     battleScreen(); break;
            case Screen::Result:     resultScreen(); break;
        }
        EndTextureMode();

        // 2. Perbesar layar virtual ke jendela (tinggi negatif karena tekstur terbalik)
        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle src = {0, 0, static_cast<float>(VIRTUAL_W), -static_cast<float>(VIRTUAL_H)};
        Rectangle dst = {static_cast<float>(offsetX), static_cast<float>(offsetY),
                         static_cast<float>(VIRTUAL_W * scale), static_cast<float>(VIRTUAL_H * scale)};
        DrawTexturePro(target.texture, src, dst, {0, 0}, 0.0f, WHITE);
        EndDrawing();
    }
}

// ===========================================================================
// Layar 1: Pilih Hero
// ===========================================================================
void App::selectScreen() {
    drawTextCentered("PILIH HERO", VIRTUAL_W / 2.0f, 6, COL_GOLD);

    Vector2 mouse = GetMousePosition();
    for (int i = 0; i < 3; i++) {
        const HeroInfo& info = heroes[i];
        Rectangle card = {8.0f + i * 104.0f, 20, 96, 156};
        bool hover = CheckCollisionPointRec(mouse, card);

        DrawRectangleRec(card, hover ? COL_HOVER : COL_PANEL);
        DrawRectangleLinesEx(card, 1, hover ? COL_GOLD : COL_OUTLINE);

        float cx = card.x + card.width / 2;
        float left = card.x + 8;
        drawSprite(heroTex[i], cx, card.y + 68, 2, 32, COL_GOLD, 0);

        drawTextCentered(info.name, cx, card.y + 72, COL_TEXT);
        drawTextCentered("(" + info.role + ")", cx, card.y + 82, COL_DIM);
        drawText("HP   " + std::to_string(info.hp), left, card.y + 94, COL_TEXT);
        drawText("ATK  " + std::to_string(info.attack), left, card.y + 104, COL_TEXT);
        drawText("MANA " + std::to_string(info.mana), left, card.y + 114, COL_TEXT);

        // Nama skill dipecah per kata jika terlalu lebar untuk kartu
        float y = card.y + 126;
        std::string line, word;
        std::string words = info.skillName + " ";
        for (char ch : words) {
            if (ch != ' ') { word += ch; continue; }
            std::string test = line.empty() ? word : line + " " + word;
            if (!line.empty() && textWidth(test) > card.width - 8) {
                drawTextCentered(line, cx, y, COL_GOLD);
                y += 9;
                line = word;
            } else {
                line = test;
            }
            word.clear();
        }
        drawTextCentered(line, cx, y, COL_GOLD);
        drawTextCentered(std::to_string(info.skillCost) + " MP", cx, y + 9, COL_MANA);

        if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            play(sndClick);
            startGame(i);
            return;
        }
    }
}

// ===========================================================================
// Layar 2: Battle
// ===========================================================================
void App::battleScreen() {
    const Hero& hero = battle->getHero();
    const Enemy& enemy = battle->getCurrentEnemy();
    int enemyIndex = battle->getEnemyIndex();
    const float groundY = 102;

    // Latar: langit dan tanah
    DrawRectangle(0, 0, VIRTUAL_W, static_cast<int>(groundY), COL_SKY);
    DrawRectangle(0, static_cast<int>(groundY), VIRTUAL_W, VIRTUAL_H - static_cast<int>(groundY), COL_GROUND);
    DrawLine(0, static_cast<int>(groundY), VIRTUAL_W, static_cast<int>(groundY), COL_OUTLINE);

    drawTextCentered("Musuh " + std::to_string(enemyIndex + 1) + "/" +
                     std::to_string(battle->getEnemyCount()), VIRTUAL_W / 2.0f, 4, COL_TEXT);

    // Hero di kiri, musuh di kanan, sprite diperbesar 2x
    drawSprite(heroTex[heroIndex], 40, groundY, 2, 32, COL_GOLD, heroFlash);
    int enemySize = (enemyIndex == 2) ? 48 : 32;     // bos lebih besar
    drawSprite(enemyTex[enemyIndex], 268, groundY, 2, enemySize, COL_RED, enemyFlash);

    // Nama dan bar
    drawText(hero.getName(), 76, 52, COL_TEXT);
    drawBar(76, 62, 66, hero.getHp(), hero.getMaxHp(), COL_HP);
    drawBar(76, 73, 66, hero.getMana(), hero.getMaxMana(), COL_MANA);
    drawText(enemy.getName(), 148, 52, COL_TEXT);
    drawBar(148, 62, 66, enemy.getHp(), enemy.getMaxHp(), COL_HP);

    if (heroFlash > 0) heroFlash--;
    if (enemyFlash > 0) enemyFlash--;

    // Tombol aksi. Skill dan Heal redup saat mana kurang, tapi tetap bisa diklik
    bool over = battle->isGameOver();
    if (over) GuiDisable();
    std::string skillText = hero.getSkillName() + " " + std::to_string(hero.getSkillCost()) + "MP";
    std::string healText = "Heal " + std::to_string(Hero::HEAL_COST) + "MP";
    if (button({4, 105, 64, 14}, "Attack")) doAction(Action::Attack);
    if (button({72, 105, 160, 14}, skillText, !hero.canUseSkill())) doAction(Action::Skill);
    if (button({236, 105, 80, 14}, healText, hero.getMana() < Hero::HEAL_COST)) doAction(Action::Heal);
    GuiEnable();

    // Panel log: 6 baris terakhir dari getLog()
    Rectangle panel = {2, 122, 316, 56};
    DrawRectangleRec(panel, COL_BG);
    DrawRectangleLinesEx(panel, 1, COL_OUTLINE);
    const std::vector<std::string>& log = battle->getLog();
    int first = std::max(0, static_cast<int>(log.size()) - 6);
    for (int i = first; i < static_cast<int>(log.size()); i++) {
        bool newest = (i == static_cast<int>(log.size()) - 1);
        drawText(log[i], 4, 125 + (i - first) * 9.0f, newest ? COL_GOLD : COL_TEXT);
    }

    // Setelah game selesai, tunggu sebentar lalu pindah ke layar Hasil
    if (over && --resultTimer <= 0) {
        screen = Screen::Result;
    }
}

// Jalankan aksi lalu tentukan suara dan efek dengan membandingkan
// nilai sebelum dan sesudah aksi (core tidak diubah sama sekali)
void App::doAction(Action action) {
    const Hero& hero = battle->getHero();
    int heroHpBefore = hero.getHp();
    int manaBefore = hero.getMana();
    int enemyHpBefore = battle->getCurrentEnemy().getHp();
    int enemyIndexBefore = battle->getEnemyIndex();
    bool overBefore = battle->isGameOver();

    switch (action) {
        case Action::Attack: battle->playerAttack(); break;
        case Action::Skill:  battle->playerSkill();  break;
        case Action::Heal:   battle->playerHeal();   break;
    }

    bool manaUsed = hero.getMana() < manaBefore;
    if (action == Action::Attack) play(sndAttack);
    if (action == Action::Skill && manaUsed) play(sndSkill);
    if (action == Action::Heal && manaUsed) play(sndHeal);

    if (battle->getEnemyIndex() > enemyIndexBefore) {
        play(sndEnemyDown);                               // musuh berganti
    } else if (battle->getCurrentEnemy().getHp() < enemyHpBefore) {
        enemyFlash = FLASH_FRAMES;                        // musuh kena damage
    }
    if (hero.getHp() < heroHpBefore) {
        heroFlash = FLASH_FRAMES;                         // hero kena damage
    }

    if (!overBefore && battle->isGameOver()) {
        play(battle->isPlayerWon() ? sndWin : sndLose);
        resultTimer = RESULT_DELAY;
    }
}

// ===========================================================================
// Layar 3: Hasil
// ===========================================================================
void App::resultScreen() {
    bool won = battle->isPlayerWon();
    drawTextCentered(won ? "MENANG" : "KALAH", VIRTUAL_W / 2.0f, 40, won ? COL_GOLD : COL_RED, 3);

    int defeated = won ? battle->getEnemyCount() : battle->getEnemyIndex();
    drawTextCentered(battle->getHero().getName() + " mengalahkan " + std::to_string(defeated) +
                     "/" + std::to_string(battle->getEnemyCount()) + " musuh",
                     VIRTUAL_W / 2.0f, 80, COL_TEXT);

    if (button({110, 110, 100, 18}, "Main Lagi")) {
        endGame();
    }
}

// ===========================================================================
// Pembantu
// ===========================================================================
void App::drawText(const std::string& text, float x, float y, Color color, int scale) {
    DrawTextEx(font, text.c_str(), {static_cast<float>(static_cast<int>(x)), static_cast<float>(static_cast<int>(y))},
               static_cast<float>(fontSize * scale), fontSpacing * scale, color);
}

void App::drawTextCentered(const std::string& text, float centerX, float y, Color color, int scale) {
    drawText(text, centerX - textWidth(text, scale) / 2, y, color, scale);
}

float App::textWidth(const std::string& text, int scale) {
    return MeasureTextEx(font, text.c_str(), static_cast<float>(fontSize * scale), fontSpacing * scale).x;
}

// Gambar sprite dengan kaki di bottomY. Jika gambar tidak ada, pakai kotak warna.
// flash > 0 membuat sprite berkedip merah.
void App::drawSprite(const Texture2D& tex, float centerX, float bottomY, int scale,
                     int fallbackSize, Color fallbackColor, int flash) {
    Color tint = (flash > 0 && (flash / 2) % 2 == 0) ? Color{255, 60, 60, 255} : WHITE;

    if (tex.id == 0) {
        float size = static_cast<float>(fallbackSize * scale);
        Rectangle box = {centerX - size / 2, bottomY - size, size, size};
        DrawRectangleRec(box, flash > 0 ? tint : fallbackColor);
        DrawRectangleLinesEx(box, 1, COL_OUTLINE);
        return;
    }
    float w = static_cast<float>(tex.width * scale);
    float h = static_cast<float>(tex.height * scale);
    Vector2 pos = {static_cast<float>(static_cast<int>(centerX - w / 2)), bottomY - h};
    DrawTextureEx(tex, pos, 0.0f, static_cast<float>(scale), tint);
}

// Bar HP atau mana dengan angka di tengahnya
void App::drawBar(float x, float y, float w, int value, int maxValue, Color color) {
    const float h = 10;
    float fill = (maxValue > 0) ? w * value / maxValue : 0;
    DrawRectangleRec({x, y, w, h}, COL_OUTLINE);
    DrawRectangleRec({x + 1, y + 1, std::max(0.0f, fill - 2), h - 2}, color);
    std::string text = std::to_string(value) + "/" + std::to_string(maxValue);
    drawTextCentered(text, x + w / 2, y + 1, COL_TEXT);
}

// Tombol raygui. dim = tampil redup tapi tetap bisa diklik.
bool App::button(Rectangle rect, const std::string& text, bool dim) {
    if (dim) GuiSetAlpha(0.45f);
    bool clicked = GuiButton(rect, text.c_str());
    GuiSetAlpha(1.0f);
    if (clicked) play(sndClick);
    return clicked;
}

// Putar suara hanya jika filenya berhasil dimuat
void App::play(const Sound& sound) {
    if (sound.frameCount > 0) PlaySound(sound);
}
