#include <iostream>
#include <string>
using namespace std;

/* ============================================================
   Fungsi bantu: int ? string (karena tanpa to_string)
   ============================================================ */
string intToString(int n) {
    string hasil = "";
    bool neg = false;
    if (n == 0) return "0";
    if (n < 0) { neg = true; n = -n; }
    while (n > 0) {
        hasil = char((n % 10) + '0') + hasil;
        n /= 10;
    }
    if (neg) hasil = "-" + hasil;
    return hasil;
}

/* ============================================================
   TREE – Struktur kursi tiap studio
   ============================================================ */
struct Kursi {
    int nomor;
    bool terisi;
    Kursi* left;
    Kursi* right;
};

Kursi* tambahKursi(Kursi* root, int nomor) {
    if (root == NULL) {
        root = new Kursi;
        root->nomor = nomor;
        root->terisi = false;
        root->left = NULL;
        root->right = NULL;
    } else if (nomor < root->nomor)
        root->left = tambahKursi(root->left, nomor);
    else if (nomor > root->nomor)
        root->right = tambahKursi(root->right, nomor);
    return root;
}

Kursi* cariKursi(Kursi* root, int nomor) {
    if (root == NULL || root->nomor == nomor) return root;
    if (nomor < root->nomor) return cariKursi(root->left, nomor);
    return cariKursi(root->right, nomor);
}

void tampilKursi(Kursi* root, int idMapping[]) {
    if (root == NULL) return;
    tampilKursi(root->left, idMapping);
    if (idMapping[root->nomor - 1] == -1)
        cout << "Kursi " << root->nomor << " (Kosong)\n";
    else
        cout << "Kursi " << root->nomor << " (Terisi oleh ID "
             << idMapping[root->nomor - 1] << ")\n";
    tampilKursi(root->right, idMapping);
}

/* ============================================================
   ARRAY – Film & Studio
   ============================================================ */
struct Studio {
    string namaStudio;
    string jadwal;
    Kursi* kursiRoot;
    int idPemesanKursi[20];
};

struct Film {
    string judul;
    int jumlahStudio;
    Studio* daftarStudio;
};

/* ============================================================
   LINKED LIST – Data user
   ============================================================ */
struct User {
    int id;
    string nama;
    string film;
    string studio;
    string jadwal;
    int jumlahTiket;
    int kursi[10];
    string status;
    User* next;
};
User* headUser = NULL;

void tambahUser(string nama, int id) {
    User* baru = new User;
    baru->id = id;
    baru->nama = nama;
    baru->film = "";
    baru->studio = "";
    baru->jadwal = "";
    baru->jumlahTiket = 0;
    baru->status = "Menunggu";
    baru->next = NULL;

    if (headUser == NULL) headUser = baru;
    else {
        User* temp = headUser;
        while (temp->next != NULL) temp = temp->next;
        temp->next = baru;
    }
}

User* cariUserByID(int id) {
    User* u = headUser;
    while (u != NULL) {
        if (u->id == id) return u;
        u = u->next;
    }
    return NULL;
}

/* ============================================================
   QUEUE – Antrean berbasis ID
   ============================================================ */
struct Queue {
    int data[100];
    int front, rear, count;
    Queue() { front = 0; rear = -1; count = 0; }

//menambahkan ID user ke belakang antrean
    void enqueue(int id) {
        if (count == 100) { cout << "Antrean penuh!\n"; return; }
        rear++; //belakang
		data[rear] = id; count++;
    }

//mengambil & menghapus ID paling depan
    int dequeue() {
        if (count == 0) return -1;
        int n = data[front]; 
		front++; //maju
		count--; //kurang
		return n;
    }

    bool kosong() { return count == 0; }

//menampilkan isi antrean dari depan ke belakang
    void tampil() {
        cout << "Antrean ID: ";
        if (count == 0) cout << "(Kosong)";
        for (int i = front; i <= rear; i++)
            cout << "[" << data[i] << "] ";
        cout << endl;
    }
//memasukkan ID ke bagian DEPAN antrean
    void enqueueFront(int id) {
    if (count == 100) {
        cout << "Antrean penuh!\n";
        return;
    }

    // Geser semua data ke kanan
    for (int i = rear; i >= front; i--) {
        data[i + 1] = data[i];
    }

    data[front] = id;
    rear++;
    count++;
}

} antrean;

/* ============================================================
   STACK – Riwayat transaksi (dengan fitur Undo)
   ============================================================ */
   struct Bioskop; 

struct Stack {
    string data[200];
    int idUser[200];
    string tipe[200]; 
    int top;
    Stack() { top = -1; }

    // Tambah data ke stack
    void push(string s, int id, string jenis) {
        top++;
        data[top] = s;
        idUser[top] = id;
        tipe[top] = jenis;
    }
    
    void undo(struct Bioskop &b);


    void tampil() {
        if (top == -1) { 
            cout << "Belum ada transaksi.\n"; 
            return; 
        }
        for (int i = top; i >= 0; i--)
            cout << "- " << data[i] << endl;
    }
};
Stack histori;



/* ============================================================
   BIOSKOP – Data utama
   ============================================================ */
struct Bioskop {
    Film daftarFilm[3];
    int jumlahFilm;
    int jumlahKursi;

    Bioskop() {
        jumlahFilm = 3;
        jumlahKursi = 10;
    }

    void init() {
        daftarFilm[0].judul = "Avatar";
        daftarFilm[0].jumlahStudio = 2;
        daftarFilm[0].daftarStudio = new Studio[2];
        daftarFilm[0].daftarStudio[0].namaStudio = "Studio 1";
        daftarFilm[0].daftarStudio[0].jadwal = "10:00 WIB";
        daftarFilm[0].daftarStudio[1].namaStudio = "Studio 2";
        daftarFilm[0].daftarStudio[1].jadwal = "15:00 WIB";

        daftarFilm[1].judul = "Joker";
        daftarFilm[1].jumlahStudio = 1;
        daftarFilm[1].daftarStudio = new Studio[1];
        daftarFilm[1].daftarStudio[0].namaStudio = "Studio 3";
        daftarFilm[1].daftarStudio[0].jadwal = "12:30 WIB";

        daftarFilm[2].judul = "Frozen";
        daftarFilm[2].jumlahStudio = 1;
        daftarFilm[2].daftarStudio = new Studio[1];
        daftarFilm[2].daftarStudio[0].namaStudio = "Studio 2";
        daftarFilm[2].daftarStudio[0].jadwal = "17:30 WIB";

        for (int i = 0; i < jumlahFilm; i++) {
            for (int s = 0; s < daftarFilm[i].jumlahStudio; s++) {
                daftarFilm[i].daftarStudio[s].kursiRoot = NULL;
                for (int k = 1; k <= jumlahKursi; k++)
                    daftarFilm[i].daftarStudio[s].kursiRoot =
                        tambahKursi(daftarFilm[i].daftarStudio[s].kursiRoot, k);
                for (int z = 0; z < jumlahKursi; z++)
                    daftarFilm[i].daftarStudio[s].idPemesanKursi[z] = -1;
            }
        }
    }

    void tampilFilm() {
        cout << "\n=== DAFTAR FILM ===\n";
        for (int i = 0; i < jumlahFilm; i++)
            cout << i + 1 << ". " << daftarFilm[i].judul << endl;
    }

    void tampilStudio(int idxFilm) {
        cout << "\n=== STUDIO UNTUK " << daftarFilm[idxFilm].judul << " ===\n";
        for (int s = 0; s < daftarFilm[idxFilm].jumlahStudio; s++)
            cout << s + 1 << ". " << daftarFilm[idxFilm].daftarStudio[s].namaStudio
                 << " - " << daftarFilm[idxFilm].daftarStudio[s].jadwal << endl;
    }
} bioskop;

// ============================================================
// IMPLEMENTASI FUNGSI UNDO (setelah struct Bioskop)
// ============================================================
void Stack::undo(Bioskop &b) {
    int i, s, k; // Deklarasi variabel loop agar C++98 kompatibel

    if (top == -1) {
        cout << "Tidak ada aksi untuk di-undo.\n";
        return;
    }

    cout << "Mengembalikan aksi: " << data[top] << endl;
    int id = idUser[top];
    string jenis = tipe[top];
    User* u = cariUserByID(id);

    if (u == NULL) {
        cout << "User tidak ditemukan.\n";
        top--;
        return;
    }

    // === Undo Pemesanan ===
    if (jenis == "pesan") {
        for (i = 0; i < b.jumlahFilm; i++) {
            if (b.daftarFilm[i].judul == u->film) {
                for (s = 0; s < b.daftarFilm[i].jumlahStudio; s++) {
                    if (b.daftarFilm[i].daftarStudio[s].namaStudio == u->studio) {
                        for (k = 0; k < u->jumlahTiket; k++) {
                            int noKursi = u->kursi[k];
                            Kursi* kursi = cariKursi(b.daftarFilm[i].daftarStudio[s].kursiRoot, noKursi);
                            if (kursi != NULL) kursi->terisi = false;
                            b.daftarFilm[i].daftarStudio[s].idPemesanKursi[noKursi - 1] = -1;
                        }
                    }
                }
            }
        }

       u->status = "Menunggu";
		u->film = "";
		u->studio = "";
		u->jadwal = "";
		u->jumlahTiket = 0;
		
	
		antrean.enqueueFront(u->id);

		
		cout << "? Undo pemesanan berhasil. Kursi dikosongkan kembali & user dimasukkan lagi ke antrean.\n";

    }

    // === Undo Pembatalan ===
    else if (jenis == "batal") {
        for (i = 0; i < b.jumlahFilm; i++) {
            if (b.daftarFilm[i].judul == u->film) {
                for (s = 0; s < b.daftarFilm[i].jumlahStudio; s++) {
                    if (b.daftarFilm[i].daftarStudio[s].namaStudio == u->studio) {
                        for (k = 0; k < u->jumlahTiket; k++) {
                            int noKursi = u->kursi[k];
                            Kursi* kursi = cariKursi(b.daftarFilm[i].daftarStudio[s].kursiRoot, noKursi);
                            if (kursi != NULL) kursi->terisi = true;
                            b.daftarFilm[i].daftarStudio[s].idPemesanKursi[noKursi - 1] = u->id;
                        }
                    }
                }
            }
        }

        u->status = "Selesai";
        cout << "? Undo pembatalan berhasil. Kursi dikembalikan seperti semula.\n";
    }

    top--;
}


/* ============================================================
   PROGRAM UTAMA
   ============================================================ */
int main() {
    bioskop.init();
    int pilih, idCounter = 1001;

    do {
        system("cls");
        cout << "===== SISTEM BOOKING BIOSKOP =====\n";
        cout << "1. Tambah User ke Antrean\n";
        cout << "2. Proses Pemesanan Tiket\n";
        cout << "3. Batalkan Pemesanan (ID)\n";
        cout << "4. Lihat Semua Data User\n";
        cout << "5. Lihat Data Kursi (per Film/Studio)\n";
        cout << "6. Lihat Riwayat Transaksi\n";
        cout << "7. Undo Aksi Terakhir\n";
        cout << "8. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilih;
        cin.ignore();

        system("cls");

        if (pilih == 1) {
            string nama;
            cout << "Masukkan nama user: ";
            getline(cin, nama);
            tambahUser(nama, idCounter);
            antrean.enqueue(idCounter);
            cout << "? User " << nama << " ditambahkan (ID: " << idCounter << ")\n";
            idCounter++;
            antrean.tampil();
        }

        else if (pilih == 2) {
            if (antrean.kosong()) { cout << "Antrean kosong.\n"; system("pause"); continue; }
            int id = antrean.dequeue();
            User* u = cariUserByID(id);
            cout << "\nGiliran ID " << id << " (" << u->nama << ")\n";

            bioskop.tampilFilm();
            int pf; cout << "Pilih film: "; cin >> pf; pf--;
            bioskop.tampilStudio(pf);
            int ps; cout << "Pilih studio: "; cin >> ps; ps--;

            Studio& studio = bioskop.daftarFilm[pf].daftarStudio[ps];
            tampilKursi(studio.kursiRoot, studio.idPemesanKursi);

            int jumlahTiket; cout << "Masukkan jumlah tiket: "; cin >> jumlahTiket;
            for (int i = 0; i < jumlahTiket; i++) {
                int no; cout << "Pilih kursi ke-" << i + 1 << ": "; cin >> no;
                Kursi* k = cariKursi(studio.kursiRoot, no);
                if (k != NULL && !k->terisi) {
                    k->terisi = true;
                    studio.idPemesanKursi[no - 1] = u->id;
                    u->kursi[i] = no;
                } else { cout << "Kursi tidak tersedia!\n"; i--; }
            }

            u->jumlahTiket = jumlahTiket;
            u->film = bioskop.daftarFilm[pf].judul;
            u->studio = studio.namaStudio;
            u->jadwal = studio.jadwal;
            u->status = "Selesai";

            histori.push("[" + intToString(u->id) + "] " + u->nama + " memesan " + u->film, u->id, "pesan");
            cout << "? Pemesanan berhasil!\n";
        }

      else if (pilih == 3) {
		    int id; 
		    cout << "Masukkan ID user: "; 
		    cin >> id;
		
		    User* u = cariUserByID(id);
		    if (u == NULL) { 
		        cout << "? User tidak ditemukan.\n"; 
		    }
		    else if (u->status == "Menunggu") {
		        cout << "? Pemesanan belum diproses, tidak bisa dibatalkan.\n";
		    }
		    else if (u->status == "Batal") {
		        cout << "? Pemesanan sudah dibatalkan sebelumnya.\n";
		    }
		    else if (u->status == "Selesai") {
		        // ?? 1. Cari film dan studio user
		        for (int i = 0; i < bioskop.jumlahFilm; i++) {
		            if (bioskop.daftarFilm[i].judul == u->film) {
		                for (int s = 0; s < bioskop.daftarFilm[i].jumlahStudio; s++) {
		                    if (bioskop.daftarFilm[i].daftarStudio[s].namaStudio == u->studio) {
		
		                        
		                        for (int k = 0; k < u->jumlahTiket; k++) {
		                            int noKursi = u->kursi[k];
		                            Kursi* kursi = cariKursi(bioskop.daftarFilm[i].daftarStudio[s].kursiRoot, noKursi);
		                            if (kursi != NULL) kursi->terisi = false;
		                            bioskop.daftarFilm[i].daftarStudio[s].idPemesanKursi[noKursi - 1] = -1;
		                        }
		                    }
		                }
		            }
		        }
		
		        // ?? 3. Ubah status user
		        u->status = "Batal";
		        histori.push("[" + intToString(u->id) + "] " + u->nama + " membatalkan " + u->film, u->id, "batal");
		        cout << "? Pemesanan dibatalkan & kursi dikosongkan.\n";
		    }
		}


        else if (pilih == 4) {
            cout << "\n=== SEMUA DATA USER ===\n";
            User* u = headUser;
            while (u != NULL) {
                cout << "[" << u->id << "] " << u->nama
                     << " | Film: " << u->film
                     << " | Studio: " << u->studio
                     << " | Tiket: " << u->jumlahTiket
                     << " | Kursi: ";
                for (int i = 0; i < u->jumlahTiket; i++)
                    cout << u->kursi[i] << " ";
                cout << "| Status: " << u->status << endl;
                u = u->next;
            }
        }

        else if (pilih == 5) {
            bioskop.tampilFilm();
            int pf; cout << "Pilih film: "; cin >> pf; pf--;
            bioskop.tampilStudio(pf);
            int ps; cout << "Pilih studio: "; cin >> ps; ps--;
            cout << "\n=== STATUS KURSI ===\n";
            tampilKursi(bioskop.daftarFilm[pf].daftarStudio[ps].kursiRoot,
                        bioskop.daftarFilm[pf].daftarStudio[ps].idPemesanKursi);
        }

        else if (pilih == 6) {
            cout << "\n=== RIWAYAT TRANSAKSI ===\n";
            histori.tampil();
        }

       else if (pilih == 7) {
		    histori.undo(bioskop);
		}


        if (pilih != 8) {
            cout << endl; system("pause");
        }

    } while (pilih != 8);

    cout << "Terima kasih!\n";
    return 0;
}
