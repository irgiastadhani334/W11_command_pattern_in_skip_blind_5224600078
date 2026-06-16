# W11_command_pattern_in_skip_blind_5224600078

# Analisis Masalah
   ## a. Mengapa sistem skip reward cocok menggunakan Command Pattern?
     Karena command pattern merupakan design pattern yang mengubah sebuah permintaan atau aksi menjadi objek mandiri dimana sistem skip reward pada Balatro cocok menerapkan pattern ini karena pemisahan waktu antara kapan reward didapat dan kapan reward dieksekusi. Setiap reward dibungkus ke dalam objek Command yang mengimplementasikan interface yang sama. Objek ini dapat disimpan ke dalam antrean atau pool manajer, lalu dijalankan (deferred execution) kapan saja ketika game loop memicu event yang sesuai.
   ## b. Mengapa reward perlu disimpan sebagai object?
    Menyimpan reward sebagai objek memberikan beberapa keuntungan arsitektural seperti Enkasulapsi Data dan Logika dimana objek reward merupakan kombinasi antara data dan logika eksekusi reward. Lalu ada Kemudahan manajemen dimana sebagai objek reward dapat mudah dimasukan kedalam list/koleksi didalam class RewardCommandManager, dipindahkan, diperiksa kondisinya, dan dihapus dari memori setelah selesai digunakan. Selain itu ada Polimorfisme dimana Manager game tidak perlu tahu detail spesifik dari setiap reward dan hanya perlu memperlakukan semuanya sebagai objek ICommand generic dan memanggil fungsi .execute() secara seragam.
   ## c. Mengapa reward tidak langsung dieksekusi saat skip?
    Berdasarkan aturan mekanik game Balatro, skip reward memberikan keuntungan yang sifatnya tertunda atau bersyarat. Kita ambil contoh skip reward Free Joker, apabila Free Joker Reward dieksekusi pada setelah pemain menekan tombol skip maka efeknya akan hilang sia-sia karena pemain saat itu sedang berada di menu pilihan blind, bukan didalam menu Shop.
   ## d. Apa masalah yang muncul jika semua reward dibuat menggunakan if-else besar?
    Apabila penerapan skip reward menggunakan satu blok if-else besar maka kita akan menghadapi masalah teknis dimana penerapan skip reward melanggar single responsibility principle karena class if-else tersebut berada akan memikul banyak load karena harus mengetahui detail cara kerja semua jenis skip reward yang berbeda,

# Penjelasan Command Pattern
Command Pattern adalah behavioral design pattern yang mengubah sebuah permintaan, aksi, atau perintah menjadi sebuah objek mandiri (executable object). Dengan mengubah perintah menjadi objek, kita dapat memisahkan (decouple) antara objek yang meminta operasi (invoker/manager) dengan objek yang tahu bagaimana cara melakukan operasi tersebut (receiver/concrete command).

Pada game Balatro, ketika pemain menekan tombol Skip Blind, game tidak langsung mengeksekusi efek reward saat itu juga karena kondisi permainan belum tepat (misal: efek Shop tidak bisa berjalan di layar Select Blind). Di sinilah Command Pattern menyelesaikan masalah melalui konsep Deferred Execution (eksekusi yang tertunda), di mana objek reward disimpan terlebih dahulu dan baru dipicu fungsinya ketika event yang tepat terjadi di masa depan.

# Analisis Responsibility dari setiap class
| Responsibility | Class | Peran dalam sistem |
| --- | --- | --- |
|Membuat reward|RewardFactory|Bertindak sebagai pembuat objek command. Kelas lain tidak perlu tahu bagaimana cara membuat objek FreeJokerCommand atau DoubleMoneyCommand, cukup meminta ke Factory menggunakan TagType.|
|Menyimpan reward|RewardCommandManager|Berfungsi sebagai wadah container untuk semua reward command yang sedang aktif dan belum terpicu di dalam std::vector|
|Mengecek trigger|RewardCommandManager|Memantau siklus game loop (event pemicu). Ketika sebuah aksi terjadi (seperti EnterShop atau EnterBlind), kelas ini mencocokkannya dengan trigger yang dimiliki setiap command|
|Mengeksekusi reward|ICommand / ConcreteCommand|Menjalankan logika atau efek mekanik game yang spesifik (misal: memotong harga joker atau menggandakan uang) tepat ketika fungsi .execute() dipanggil|
|Menghapus reward|RewardCommandManager|Melakukan manajemen memori (cleanup) secara otomatis untuk menghapus dan membuang objek command dari antrean setelah efeknya selesai dieksekusi|
|Menetapkan kontrak standar (behavior) untuk semua reward|ICommand|Memastikan bahwa RewardCommandManager dapat berinteraksi dengan seluruh reward yang ada di game tanpa harus mengetahui detail isi dari reward tersebut|
|Enkapsulasi logika mekanik reward yang spesifik dan mengetahui kapan event trigger-nya sendiri|FreeJokerCommand, DoubleMoneyCommand, MegaBlindBuffCommand|Memisahkan setiap reward ke dalam kelasnya masing-masing membuat kode memenuhi Single Responsibility Principle (SRP). Jika efek DoubleMoneyCommand ingin diubah fungsinya di kemudian hari, kita hanya perlu mengedit kelas tersebut tanpa berisiko merusak logika reward lainnya.|

# Class Diagram
   ----+---------------------------------------+
       |               ICommand                | <----------------+
       +---------------------------------------+                  |
       | + ~ICommand()                         |                  |
       | + execute() : void = 0                |                  |
       | + getTrigger() : TriggerEvent = 0     |                  |
       | + isFinished() : bool = 0             |                  |
       +---------------------------------------+                  |
                           ^                                      |
                           | (Inheritance / Realization)          |
         +-----------------+-----------------+                    |
         |                                   |                    |
+----------------------+           +----------------------+       |
|   FreeJokerCommand   |           |  DoubleMoneyCommand  |       |
+----------------------+           +----------------------+       |
| - finished : bool    |           | - finished : bool    |       |
+----------------------+           +----------------------+       |
| + execute()          |           | + execute()          |       |
| + getTrigger()       |           | + getTrigger()       |       |
| + isFinished()       |           | + isFinished()       |       |
+----------------------+           +----------------------+       |
                                                                  |
                                                                  |
+---------------------------------------+                         |
|         RewardCommandManager          |                         |
+---------------------------------------+                         |
| - activeRewards : vector<ICommand*>   | ------------------------+ 
+---------------------------------------+   (Aggregation / Stores)
| + addReward(reward: ICommand)         |
| + handleEvent(event: TriggerEvent)    |
| - cleanupRewards()                    |
+---------------------------------------+
                               ^
                               | (Uses to populate manager)
+---------------------------------------+
|             RewardFactory             |
+---------------------------------------+
| + createReward(tag: TagType)          | ----> Menciptakan ConcreteCommand
+---------------------------------------+

# Flow Sistem
1. Player Skip Blind
   Pemain menekan tombol "Skip Blind" pada antarmuka pemilihan blind game Balatro.Babak blind tersebut dilewati, pemain merelakan reward normal, dan sistem bersiap memberikan special reward berupa Tag
2. Reward Created (Proses Instansiasi)
   Game mendeteksi jenis Tag yang didapatkan dari aksi skip tersebut. Sistem memanggil RewardFactory::createReward(TagType). Factory bertindak sebagai pembuat objek dan langsung menginstansiasi concrete command yang sesuai. Objek command kini hidup di memori sebagai entitas mandiri yang membawa tipe trigger-nya masing-masing.
3. Reward Stored (Proses Penyimpanan)
   Objek command yang baru saja dibuat oleh Factory diserahkan kepada pengelola pusat. addReward() milik RewardCommandManager dipanggil dengan mengirimkan pointer dari objek reward tersebut. Reward kini tersimpan dengan aman di dalam koleksi activeRewards (antrean internal manajer). Eksekusinya ditunda (deferred execution) sampai waktu yang belum ditentukan.
4. Event Happens (Pemicu Gameplay)
   Pemain terus melanjutkan permainan hingga sebuah peristiwa/kondisi di dalam game terpenuhi. Pemain memutuskan untuk melangkah masuk ke menu toko (EnterShop) atau memulai babak blind berikutnya (EnterBlind). Sistem Event Handler utama game menangkap kejadian tersebut dan langsung meneruskannya ke manajer dengan memanggil fungsi manager.handleEvent(TriggerEvent).
5. Reward Executed (Proses Eksekusi)
   RewardCommandManager melakukan iterasi (perulangan) untuk memeriksa seluruh daftar reward yang sedang ia simpan. Manajer mencocokkan TriggerEvent yang baru saja terjadi dengan nilai .getTrigger() dari setiap objek command. Jika cocok, fungsi .execute() pada concrete command tersebut dipanggil. Logika di dalam FreeJokerCommand berjalan, memodifikasi harga kartu Joker di toko menjadi $0. Status internal command tersebut kini berubah menjadi selesai (finished = true).
6. Reward Removed (Proses Pembersihan Memori)
   Setelah semua perintah yang memenuhi syarat selesai dieksekusi, manajer menjalankan fungsi pembersihan internal (cleanupRewards()). Manajer memeriksa properti .isFinished() dari setiap objek. Objek command yang bernilai true akan dihapus dari std::vector secara permanen. Memori dibersihkan secara otomatis menggunakan Smart Pointer untuk mencegah kebocoran memori (memory leak), dan siklus hidup (lifecycle) dari skip reward tersebut resmi berakhir.
# Pseudocode
Ada pada Repository
# Kesimpulan
Berdasarkan analisis, perancangan struktur kelas, dan simulasi alur yang telah dilakukan, penerapan Command Pattern pada sistem Skip Blind Reward dalam Balatro memberikan kesimpulan arsitektur yang krusial:
1. Keberhasilan Memisahkan Waktu Eksekusi (Deferred Execution): Command Pattern terbukti menjadi solusi paling efektif untuk menangani mekanik game yang membutuhkan penundaan aksi. Sistem berhasil memisahkan momen saat pemain mendapatkan reward dengan momen ketika reward tersebut benar-benar aktif di masa depan.
2. Kepatuhan terhadap Prinsip SOLID:
   - Single Responsibility Principle (SRP): Setiap efek reward diisolasi ke dalam kelas perintah konkritnya masing-masing (FreeJokerCommand, DoubleMoneyCommand, MegaBlindBuffCommand), sehingga pengelolaan logikanya menjadi sangat fokus dan modular.
   - pen/Closed Principle (OCP): Dengan memanfaatkan kombinasi ICommand dan RewardFactory, tim pengembang dapat menambahkan puluhan jenis reward baru di masa mendatang tanpa perlu mengubah atau membongkar kode manajemen inti yang sudah stabil.
3. Manajemen Memori yang Efisien: Melalui kelas RewardCommandManager, siklus hidup setiap reward terkontrol dengan baik. Objek reward disimpan hanya saat dibutuhkan dan segera dihapus dari memori setelah selesai dieksekusi, menjaga performa game tetap optimal dan bebas dari kebocoran memori.

# Pertanyaan Analisis
- Apa keuntungan reward disimpan sebagai object?
  Reward menjadi memiliki data (status) dan perilaku (logika eksekusi) sendiri yang terenkapsulasi, serta dapat dikelola secara dinamis di dalam koleksi (array/vector).
- Mengapa sistem ini lebih scalable dibanding if-else?
  Karena penambahan reward baru dilakukan dengan membuat kelas baru, bukan dengan memperpanjang baris kode bercabang di dalam satu fungsi besar yang rapi dan rawan bug.
- Apa hubungan Event System dan Command Pattern?
  Event System bertindak sebagai pemicu (trigger) yang mengabarkan bahwa suatu kondisi gameplay terjadi (misal: masuk toko), sementara Command Pattern menangani apa tindakan spesifik yang harus dieksekusi menanggapi kabar tersebut.
- Apa yang dimaksud deferred execution?
  Penundaan eksekusi sebuah blok kode/perintah sampai kondisi atau waktu yang tepat di masa depan terpenuhi, alih-alih langsung dijalankan saat objek dibuat.
- Bagaimana cara menambahkan reward baru tanpa mengubah banyak kode existing?
  Buat kelas concrete command baru yang menurunkan ICommand , lalu daftarkan tipe pembuatannya ke dalam RewardFactory. Kode pada kelas RewardCommandManager sama sekali tidak perlu diubah.

# Lampiran Prompt AI
|||
|---|---|
|Analisis design pattern|Kita tahu bahwa didalam Balatro pemain bisa melakukan skip blind dimana:
- Blind tidak dimainkan
- Reward Normal tidak diperoleh
- Namun pemain mendapat reward khusus berupa tag/skip reward

Reward tersebut beberapa tidak langsung aktif, melainkan baru jalan jika terjadi event tertentu misal :
- saat masuk shop berikutnya
- saat blind berikutnya dimulai
- saat membeli kartu
- dan sebagainya

Pada soal kali ini kita menggunakan 3 skip reward yaitu:
- Free Joker Reward : Joker berikutnya gratis, Trigger pada EnterShop
- Double Money Reward: Uang menjadi 2x, Trigger pada EnterShop
- Mega Blind Buff Reward: Bonus multiplier pada blind
berikutnya, Trigger pada EnterBlind

Untuk pengerjaan kali ini kita mulai dari soal analisa masalah terlebih dahulu|
|Membuat pseudocode|Kita disini langsung menuju ke Rancangan Struktur class dimana berisi:
Interface:
class ICommand

Concrete Command:
minimal:
FreeJokerCommand
DoubleMoneyCommand
MegaBlindBuffCommand

Manager:
RewardCommandManager
Responsibility:
- menyimpan command
- mengecek trigger
- mengeksekusi command
- menghapus command yang selesai digunakan

Factory:
RewardFactory
Responsibility:
- membuat reward berdasarkan tag|
|Membuat class diagram|Dari struktur dan pseudocode class yang kita buat kita akan menuju kedalam pembuatan diagram untuk menjelaskan hubungan asosiasi setiap class |
|Mencari ide
architecture|Bagaimanakah penyusunan file untuk setiap class yang telah kita buat agar kita mencegah circular dependency dan memudahkan mencari fungsi?|
