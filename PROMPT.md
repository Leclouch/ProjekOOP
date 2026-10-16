Project ini adalah game turn based C++17 untuk tugas OOP (Encapsulation, Abstraction, Inheritance, Polymorphism). Folder core/ berisi seluruh logika game dan sudah selesai serta teruji lewat test_console.cpp. Tugasmu: membuat sprite, efek suara, dan UI grafis memakai raylib dan raygui.

## Aturan utama
1. Jangan ubah file apa pun di core/. Baca semua file di core/ terlebih dahulu. UI hanya boleh memakai BattleManager.h serta getter const di Hero.h, Enemy.h, dan Character.h.
2. Semua kode UI di folder ui/ (App.h, App.cpp). main.cpp di root hanya membuat App lalu menjalankannya.
3. CMakeLists.txt mengunduh raylib dan raygui otomatis lewat FetchContent. Target: Windows dengan MinGW. CMake menyalin folder assets/ ke folder hasil build. Tulis perintah build lengkap di README.md.
4. BattleManager tidak bisa disalin (copy constructor dan assignment di delete). Simpan di App sebagai pointer: buat dengan new saat game dimulai, delete saat Main Lagi atau saat program ditutup.
5. Kode UI harus sederhana dan mudah dibaca mahasiswa. Beri komentar singkat di bagian penting.
6. Kerjakan berurutan dari Langkah 1 sampai 4. Pastikan build berhasil di akhir setiap langkah sebelum lanjut.

## Langkah 1: Sprite
Gunakan MCP pixel-art untuk menggambar sprite. Jika MCP tidak tersedia, buat lewat skrip Python dengan Pillow dan simpan skripnya di tools/.

Gaya: pixel art bergaya anime JRPG chibi. Satu palet maksimal 16 warna dipakai bersama oleh semua sprite. Outline gelap, latar transparan. Sudut pandang ala JRPG battle klasik: hero dilihat tiga perempat dari depan dan menghadap kanan (ke arah musuh), musuh dilihat tiga perempat dari depan dan menghadap kiri (ke arah hero), sehingga wajah tetap terlihat. Hero dan musuh biasa berukuran 16x16, bos 32x32. Semua karakter dalam pose siaga bertarung yang diam, kaki menapak di baris piksel paling bawah agar semua berdiri di garis tanah yang sama. Senjata tidak boleh terpotong tepi kanvas. Siluet ketiga hero harus mudah dibedakan satu sama lain walau tanpa warna.

Simpan di assets/sprites/:
- warrior.png: [NAMA WARRIOR]. Seorang gadis muda bertubuh kecil dan ramping, sekitar 154 cm, dengan kulit cerah dan wajah halus berekspresi serius dan tegas. Rambutnya pirang keemasan, bagian belakangnya dikepang lalu digulung menjadi sanggul dan diikat pita biru, dengan poni di dahi, dua helai samping membingkai wajah, dan sejumput rambut yang berdiri melengkung di puncak kepala. Matanya hijau zamrud. Ia mengenakan gaun panjang biru kerajaan berlengan panjang dengan rok mengembang hampir sampai mata kaki dan lapisan kain putih di tepi rok. Di atasnya ia memakai zirah perak berlis emas: pelindung dada yang menutup dada hingga perut, pelindung bahu kecil, sarung tangan zirah sampai lengan bawah, lempengan zirah bertingkat di pinggang yang melebar menutupi bagian atas rok, dan sepatu zirah yang menutup tulang kering hingga kaki. Ia memegang pedang panjang dengan gagang dan pelindung tangan emas, pegangan biru, dan bilah yang memancarkan cahaya keemasan. Pose: kuda kuda lebar dan berat, badan sedikit condong ke depan, senjata utama dipegang dua tangan siap menebas atau disandarkan di bahu.
- mage.png: [NAMA MAGE]. Seorang gadis remaja bertubuh kecil, pendek, dan ramping dengan kulit cerah dan wajah bulat yang ekspresif. Rambutnya cokelat sangat gelap hingga hampir hitam, dipotong bob pendek sebatas leher dengan poni tidak rata di dahi dan dua helai samping yang lebih panjang membingkai pipi. Matanya berwarna merah crimson yang tampak menyala. Ia memakai topi penyihir hitam berukuran sangat besar dengan ujung runcing yang terkulai ke belakang, pinggiran lebar, dan pita merah oranye bergesper emas kecil di dasarnya. Mata kirinya tertutup penutup mata hitam bermotif emas kecil. Ia mengenakan gaun merah pendek sebatas paha dengan lis emas, dilapisi jubah hitam berkerah tinggi yang berdiri dengan pinggiran emas dan menjuntai di punggung, choker hitam di leher, sabuk cokelat di pinggang, dan sarung tangan gelap. Kakinya asimetris: satu kaki memakai stoking hitam sepanjang paha, kaki lainnya dililit perban putih, keduanya dengan sepatu bot cokelat bertali setinggi betis. Ia memegang tongkat sihir kayu yang tinggi, setinggi tubuhnya atau lebih, dengan ujung atas melengkung yang memegang batu permata merah.. Pose: berdiri tegak, tongkat atau media sihir diangkat ke depan ke arah musuh, tangan lain terbuka, ujung jubah atau rambut sedikit berkibar. Beri satu atau dua piksel cahaya terang di ujung tongkat.
- archer.png: [NAMA ARCHER]. Seorang pemuda pendek dan ramping yang tampak seperti remaja laki laki mungil, dengan wajah halus, bulat, dan androgini serta ekspresi santai yang sering tersenyum jahil. Rambutnya hitam kebiruan gelap, pendek dengan poni menutupi dahi, dan dua kepangan tipis di sisi kepala dekat telinga yang ujungnya bergradasi ke warna toska dan tampak sedikit bercahaya. Matanya toska kehijauan dengan gradasi warna yang cerah. Ia memakai topi beret hijau tua yang agak miring dengan bunga putih kecil disematkan di sisinya. Pakaiannya terdiri dari kemeja putih lengan panjang yang agak menggembung, rompi hijau tua beraksen emas, kerah hitam dengan detail emas dan bros di tengah leher, jubah pendek hijau berlis emas dengan pola geometris yang menutup bahu dan punggung, permata hijau toska berbingkai emas di bagian depan dekat dada, celana pendek hijau tua, stoking putih panjang dengan pola belah ketupat samar, dan sepatu cokelat rendah. Ia membawa lira hijau kecil di tangannya.. Pose: setengah jongkok dan ringan, busur dibentangkan ke kanan dengan anak panah terpasang, siku tangan penarik terlihat ke belakang.
- slime.png: Mochi Slime, gumpalan hijau mint bulat, satu kelopak sakura di atas kepala, mata titik. Pose: sedikit gepeng seperti sedang memantul, bagian bawah rata dengan tanah.
- goblin.png: Kogoblin, oni kecil berkulit ungu, tanduk pendek, membawa dua belati. Pose: membungkuk dan merunduk siap menerjang, satu belati di tiap tangan.
- dragon.png (32x32): Kurogane, naga hitam bergaya timur, sisik besi, mata merah. Bos terakhir. Pose: tubuh melingkar memenuhi kanvas, kepala besar di sisi kiri atas menghadap hero, mulut sedikit terbuka dengan beberapa piksel api atau asap.

Buat juga assets/sprites/preview.png berisi keenam sprite berjajar dengan skala 8x agar mudah dicek.

## Langkah 2: Efek suara
Buat skrip tools/make_sfx.py yang hanya memakai modul bawaan Python (wave, math, random, struct) untuk membangkitkan efek suara retro 8 bit dari gelombang square, triangle, dan noise. Format: mono, 44100 Hz, 16 bit, durasi 0.1 sampai 1 detik. Jika Python tidak tersedia, buat skrip setara dengan Node.

Simpan di assets/sound/:
- click.wav: klik tombol singkat
- attack.wav: pukulan pendek
- skill.wav: ledakan kuat
- heal.wav: nada naik berkilau
- enemy_down.wav: nada turun
- win.wav: jingle kemenangan pendek
- lose.wav: jingle kekalahan pendek

## Langkah 3: UI
Gaya pixel: render seluruh game ke RenderTexture 320x180, lalu skala ke jendela 1280x720 dengan kelipatan bulat dan TEXTURE_FILTER_POINT supaya piksel tetap tajam. Font dari assets/fonts/PressStart2P.ttf; jika tidak ada, pakai font bawaan raylib.

Tiga layar:
1. Pilih Hero: tiga kartu berisi sprite, nama, peran, HP, serangan, mana, nama skill dan biaya mananya. Klik kartu untuk memilih.
2. Battle: hero di kiri, musuh di kanan, sprite diperbesar dengan skala bulat. Setiap karakter punya nama dan HP bar, hero juga punya mana bar. Tampilkan "Musuh X/3". Di bawah ada tombol Attack, Skill (tulis nama skill dan biaya mana), dan Heal (tulis biaya mana). Tombol Skill dan Heal tampak redup saat mana kurang, tapi tetap bisa diklik karena core akan menulis pesan ke log. Panel log menampilkan 6 baris terakhir dari getLog().
3. Hasil: tulisan MENANG atau KALAH dan tombol Main Lagi yang kembali ke Pilih Hero.

Efek visual: sprite yang terkena damage berkedip merah selama beberapa frame. Tanpa animasi lain.

Suara: pakai InitAudioDevice, LoadSound, PlaySound. Tentukan suara mana yang diputar dengan membandingkan nilai sebelum dan sesudah aksi lewat getter (HP hero, HP musuh, mana hero, getEnemyIndex, isGameOver, isPlayerWon), bukan dengan mengubah core:
- setiap klik tombol: click
- tombol Attack: attack
- tombol Skill dan mana berkurang: skill
- tombol Heal dan mana berkurang: heal
- getEnemyIndex bertambah: enemy_down
- game baru selesai: win atau lose sesuai isPlayerWon

Semua aset opsional saat program berjalan: jika file sprite tidak ditemukan, gambar kotak berwarna sebagai gantinya; jika file suara tidak ditemukan, lewati. Program tidak boleh crash karena aset hilang.

## Langkah 4: Cek akhir
Build dari nol mengikuti README. Pastikan ketiga hero bisa dipilih, game bisa dimenangkan dan bisa kalah, Main Lagi berjalan berulang kali tanpa crash, dan suara terdengar. Laporkan secara ringkas file apa saja yang dibuat dan perintah untuk menjalankan game.