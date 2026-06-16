# W11_command_pattern_in_skip_blind_5224600078

Analisis Masalah
   a. Mengapa sistem skip reward cocok menggunakan Command Pattern?
     Karena command pattern merupakan design pattern yang mengubah sebuah permintaan atau aksi menjadi objek mandiri dimana sistem skip reward pada Balatro cocok menerapkan pattern ini karena pemisahan waktu antara kapan reward didapat dan kapan reward dieksekusi. Setiap reward dibungkus ke dalam objek Command yang mengimplementasikan interface yang sama. Objek ini dapat disimpan ke dalam antrean atau pool manajer, lalu dijalankan (deferred execution) kapan saja ketika game loop memicu event yang sesuai.
   b. Mengapa reward perlu disimpan sebagai object?
   Menyimpan reward sebagai objek memberikan beberapa keuntungan arsitektural seperti Enkasulapsi Data dan Logika dimana objek reward merupakan kombinasi antara data dan logika eksekusi reward. Lalu ada Kemudahan manajemen dimana sebagai objek reward dapat mudah dimasukan kedalam list/koleksi didalam class RewardCommandManager, dipindahkan, diperiksa kondisinya, dan dihapus dari memori setelah selesai digunakan. Selain itu ada Polimorfisme dimana Manager game tidak perlu tahu detail spesifik dari setiap reward dan hanya perlu memperlakukan semuanya sebagai objek ICommand generic dan memanggil fungsi .execute() secara seragam.
   c. Mengapa reward tidak langsung dieksekusi saat skip?
   Berdasarkan aturan mekanik game Balatro, skip reward memberikan keuntungan yang sifatnya tertunda atau bersyarat. Kita ambil contoh skip reward Free Joker, apabila Free Joker Reward dieksekusi pada setelah pemain menekan tombol skip maka efeknya akan hilang sia-sia karena pemain saat itu sedang berada di menu pilihan blind, bukan didalam menu Shop.
   d. Apa masalah yang muncul jika semua reward dibuat menggunakan if-else besar?
   Apabila penerapan skip reward menggunakan satu blok if-else besar maka kita akan menghadapi masalah teknis dimana penerapan skip reward melanggar single responsibility principle karena class if-else tersebut berada akan memikul banyak load karena harus mengetahui detail cara kerja semua jenis skip reward yang berbeda,

Analisis Responsibility dari setiap class

   
