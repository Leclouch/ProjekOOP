# Game Simple: Turn Based RPG

Game pertarungan bergiliran C++17 untuk tugas OOP (Encapsulation, Abstraction,
Inheritance, Polymorphism). Pilih satu hero, lalu kalahkan tiga musuh berurutan:
Slime, Goblin, dan bos Dragon.

Logika game ada di `core/`, tampilan grafis memakai [raylib](https://www.raylib.com/)
dan [raygui](https://github.com/raysan5/raygui) di `ui/`.

## Kebutuhan

- Windows dengan MinGW (g++ yang mendukung C++17), sudah ada di PATH
- CMake 3.20 atau lebih baru
- Koneksi internet saat build pertama (raylib dan raygui diunduh otomatis)

## Build dan jalankan

Jalankan dari folder project (folder yang berisi `CMakeLists.txt`):

```bat
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 4
build\game.exe
```

Folder `assets/` otomatis disalin ke `build/assets/` setiap selesai build.
`game.exe` dilink statis, jadi bisa dijalankan tanpa DLL MinGW.

Untuk build ulang dari nol, hapus folder `build/` lalu ulangi perintah di atas.

## Cara main

1. **Pilih Hero**: klik salah satu dari tiga kartu hero.
2. **Battle**: klik `Attack`, tombol skill, atau `Heal`. Tombol skill dan Heal
   tampak redup saat mana kurang (tetap bisa diklik, pesan muncul di log).
   Setiap giliran mana bertambah 5.
3. **Hasil**: tampil MENANG atau KALAH. Klik `Main Lagi` untuk kembali memilih hero.

## Struktur folder

```
core/              logika game (tidak bergantung pada raylib)
ui/App.h, App.cpp  seluruh kode tampilan, input, dan suara
main.cpp           membuat App lalu menjalankannya
assets/sprites/    sprite pixel art (preview.png berisi semuanya skala 8x)
assets/sound/      efek suara 8 bit
assets/fonts/      font Press Start 2P (lisensi SIL OFL, lihat OFL.txt)
tools/             skrip Python pembuat sprite dan suara
test_console.cpp   versi console untuk menguji core
```

## Membuat ulang aset (opsional)

Aset sudah tersedia di `assets/`. Jika ingin mengubah lalu membuatnya ulang:

```bat
pip install pillow
python tools/make_sprites.py
python tools/make_sfx.py
```

`make_sprites.py` berisi grid piksel tiap sprite dan memakai satu palet 16 warna.
`make_sfx.py` hanya memakai modul bawaan Python (wave, math, random, struct).

Semua aset bersifat opsional: jika sprite hilang, game menggambar kotak berwarna;
jika suara hilang, suara dilewati; jika font hilang, dipakai font bawaan raylib.
