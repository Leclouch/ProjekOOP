#pragma once
#include <string>
#include "raylib.h"
#include "core/BattleManager.h"

// App mengurus jendela, aset, input, dan gambar.
// Semua logika pertarungan tetap di BattleManager (folder core/).
class App {
public:
    App();
    ~App();

    // App memegang pointer BattleManager, jadi tidak boleh disalin
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    void run();

private:
    enum class Screen { SelectHero, Battle, Result };
    enum class Action { Attack, Skill, Heal };

    // Data kartu hero di layar Pilih Hero
    struct HeroInfo {
        HeroType type;
        std::string name, role, skillName;
        int hp, attack, mana, skillCost;
    };

    // Ukuran layar virtual: game digambar di sini lalu diperbesar ke jendela
    static constexpr int VIRTUAL_W = 320;
    static constexpr int VIRTUAL_H = 180;
    static constexpr int FLASH_FRAMES = 16;   // lama kedip merah saat kena damage
    static constexpr int RESULT_DELAY = 60;   // jeda (frame) sebelum layar Hasil

    Screen screen;
    BattleManager* battle;     // dibuat dengan new saat game dimulai
    int heroIndex;             // 0 Warrior, 1 Mage, 2 Archer
    HeroInfo heroes[3];

    int heroFlash;             // sisa frame kedip merah hero
    int enemyFlash;            // sisa frame kedip merah musuh
    int resultTimer;           // hitung mundur menuju layar Hasil

    // Aset (semuanya opsional, boleh tidak ada)
    RenderTexture2D target;
    Font font;
    bool customFont;
    int fontSize;
    float fontSpacing;
    Texture2D heroTex[3];
    Texture2D enemyTex[3];
    Sound sndClick, sndAttack, sndSkill, sndHeal, sndEnemyDown, sndWin, sndLose;

    // Persiapan dan pembersihan
    void loadAssets();
    void unloadAssets();
    void setupGuiStyle();
    void loadHeroInfo();
    void startGame(int index);
    void endGame();

    // Tiap layar menggambar dirinya sendiri sekaligus membaca klik
    void selectScreen();
    void battleScreen();
    void resultScreen();
    void doAction(Action action);

    // Pembantu gambar dan suara
    void drawText(const std::string& text, float x, float y, Color color, int scale = 1);
    void drawTextCentered(const std::string& text, float centerX, float y, Color color, int scale = 1);
    float textWidth(const std::string& text, int scale = 1);
    void drawSprite(const Texture2D& tex, float centerX, float bottomY, int scale,
                    int fallbackSize, Color fallbackColor, int flash);
    void drawBar(float x, float y, float w, int value, int maxValue, Color color);
    bool button(Rectangle rect, const std::string& text, bool dim = false);
    void play(const Sound& sound);
};
