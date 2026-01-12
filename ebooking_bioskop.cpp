#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;

/* ======================================================
   TREE ? Mengatur kursi per studio
   ====================================================== */
struct Kursi {
    int nomor;
    bool terisi;
    Kursi* left;
    Kursi* right;
};

struct Kursi* tambahKursi(Kursi* root, int nomor) {
    if (!root) {
        root = new Kursi{nomor, false, NULL, NULL};
    } else if (nomor < root->nomor)
        root->left = tambahKursi(root->left, nomor);
    else if (nomor > root->nomor)
        root->right = tambahKursi(root->right, nomor);
    return root;
}

struct Kursi* cariKursi(Kursi* root, int nomor) {
    if (!root || root->nomor == nomor) return root;
    if (nomor < root->nomor) return cariKursi(root->left, nomor);
    return cariKursi(root->right, nomor);
}

void tampilKursi(Kursi* root) {
    if (!root) return;
    tampilKursi(root->left);
    cout << "Kursi " << root->nomor << (root->terisi ? " (Terisi)" : " (Kosong)") << endl;
    tampilKursi(root->right);
}

void kosongkanKursi(Kursi* root) {
    if (!root) return;
    kosongkanKursi(root->left);
    root->terisi = false;
    kosongkanKursi(root->right);
}

/* ======================================================
   ARRAY ? Menyimpan data film & studio
   ====================================================== */
struct Studio {
    string namaStudio;
    string jadwal;
    Kursi* kursiRoot;
};

struct Film {
    string judul;
    int jumlahStudio;
    Studio* daftarStudio;
};

/* ======================================================
   LINKED LIST ? Data User
   ====================================================== */
struct User {
    string nama;
    string film;
    string studio;
    string jadwal;
    int kursi;
    User* next;
};
User* headUser = NULL;

void tambahUser(string nama) {
    User* baru = new User{nama, "", "", "", -1, NULL};
    if (!headUser)
        headUser = baru;
    else {
        User* temp = headUser;
        while (temp->next) temp = temp->next;
        temp->next = baru;
    }
}

User* cariUser(string nama) {
    for (User* u = headUser; u; u = u->next)
        if (u->nama == nama) return u;
    return NULL;
}

/* ======================================================
   QUEUE ? Antrean pengguna
   ====================================================== */
struct Queue {
    string data[100];
    int front = 0, rear = -1, count = 0;

    void enqueue(string nama) {
        data[++rear] = nama;
        count++;
    }

    string dequeue() {
        if (count == 0) return "Kosong";
        string n = data[front++];
        count--;
        return n;
    }

    bool kosong() { return count == 0; }

    void tampil() {
        cout << "Antrean: ";
        if (count == 0) cout << "(Kosong)";
        for (int i = front; i <= rear; i++) cout << "[" << data[i] << "] ";
        cout << endl;
    }
} antrean;

/* ======================================================
   STACK ? Menyimpan riwayat pemesanan
   ====================================================== */
struct Stack {
    string data[200];
    int top = -1;
    void push(string s) { data[++top] = s; }
    bool kosong() { return top == -1; }
    void tampil() {
        for (int i = top; i >= 0; i--)
            cout << "- " << data[i] << endl;
    }
} histori;

/* ======================================================
   BIOSKOP ? Mengelola keseluruhan sistem
   ====================================================== */
struct Bioskop {
    Film daftarFilm[3];
    int jumlahFilm = 3;
    int jumlahKursi = 10;

    void init() {
        // Film 1
        daftarFilm[0].judul = "Avatar";
        daftarFilm[0].jumlahStudio = 2;
        daftarFilm[0].daftarStudio = new Studio[2]{
            {"Studio 1", "10:00 WIB", NULL},
            {"Studio 2", "15:00 WIB", NULL}
        };

        // Film 2
        daftarFilm[1].judul = "Joker";
        daftarFilm[1].jumlahStudio = 1;
        daftarFilm[1].daftarStudio = new Studio[1]{
            {"Studio 3", "12:30 WIB", NULL}
        };

        // Film 3
        daftarFilm[2].judul = "Frozen";
        daftarFilm[2].jumlahStudio = 1;
        daftarFilm[2].daftarStudio = new Studio[1]{
            {"Studio 2", "17:30 WIB", NULL}
        };

        // Tambah kursi di tiap studio
        for (int i = 0; i < jumlahFilm; i++) {
            for (int s = 0; s < daftarFilm[i].jumlahStudio; s++) {
                for (int k = 1; k <= jumlahKursi; k++)
                    daftarFilm[i].daftarStudio[s].kursiRoot =
                        tambahKursi(daftarFilm[i].daftarStudio[s].kursiRoot, k);
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

/* ======================================================
   PROGRAM UTAMA
   ====================================================== */
int main() {
    bioskop.init();
    int pilih;

    do {
        cout << "\n===== SISTEM BOOKING BIOSKOP =====\n";
        cout << "1. Tambah User ke Antrean\n";
        cout << "2. Proses Pemesanan Tiket\n";
        cout << "3. Batalkan Pemesanan User\n";
        cout << "4. Lihat Status Kursi\n";
        cout << "5. Lihat Riwayat Pemesanan\n";
        cout << "6. Kosongkan Kursi Studio (Film Selesai)\n";
        cout << "7. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilih;
        cin.ignore();

        /* ======== 1. Tambah User (Linked List + Queue) ======== */
        if (pilih == 1) {
            string nama;
            cout << "\nMasukkan nama user: ";
            getline(cin, nama);
            tambahUser(nama);
            antrean.enqueue(nama);
            cout << "? " << nama << " ditambahkan ke antrean.\n";
            antrean.tampil();
            system("pause");
            system("cls");
        }

        /* ======== 2. Pemesanan Tiket (Queue ? Film/Tree) ======== */
        else if (pilih == 2) {
            if (antrean.kosong()) {
                cout << "?? Antrean kosong.\n";
                system("pause");
                system("cls");
                continue;
            }

            string nama = antrean.dequeue();
            cout << "\nGiliran: " << nama << endl;
            bioskop.tampilFilm();

            int pf;
            cout << "Pilih film: ";
            cin >> pf;
            if (pf < 1 || pf > bioskop.jumlahFilm) continue;
            pf--;

            bioskop.tampilStudio(pf);
            int ps;
            cout << "Pilih studio: ";
            cin >> ps;
            if (ps < 1 || ps > bioskop.daftarFilm[pf].jumlahStudio) continue;
            ps--;

            Studio& studio = bioskop.daftarFilm[pf].daftarStudio[ps];
            cout << "\nKursi tersedia di " << studio.namaStudio << " (" << studio.jadwal << "):\n";
            tampilKursi(studio.kursiRoot);

            int no;
            cout << "\nPilih nomor kursi: ";
            cin >> no;

            Kursi* k = cariKursi(studio.kursiRoot, no);
            if (k && !k->terisi) {
                k->terisi = true;
                User* u = cariUser(nama);
                u->film = bioskop.daftarFilm[pf].judul;
                u->studio = studio.namaStudio;
                u->jadwal = studio.jadwal;
                u->kursi = no;

                stringstream ss;
                ss << nama << " memesan '" << u->film << "' di " << u->studio
                   << " (" << u->jadwal << ") kursi " << no;
                histori.push(ss.str());
                cout << "? Pemesanan berhasil!\n";
            } else {
                cout << "? Kursi tidak tersedia!\n";
            }

            system("pause");
            system("cls");
        }

        /* ======== 3. Batalkan Pemesanan (Linked List + Tree + Stack) ======== */
        else if (pilih == 3) {
            string nama;
            cout << "\nMasukkan nama user: ";
            getline(cin, nama);

            User* u = cariUser(nama);
            if (!u || u->film == "") {
                cout << "? Tidak ada pesanan aktif.\n";
            } else {
                // Kosongkan kursi yang dipesan
                for (int i = 0; i < bioskop.jumlahFilm; i++) {
                    if (bioskop.daftarFilm[i].judul == u->film) {
                        for (int s = 0; s < bioskop.daftarFilm[i].jumlahStudio; s++) {
                            if (bioskop.daftarFilm[i].daftarStudio[s].namaStudio == u->studio) {
                                Kursi* k = cariKursi(bioskop.daftarFilm[i].daftarStudio[s].kursiRoot, u->kursi);
                                if (k) k->terisi = false;
                            }
                        }
                    }
                }
                stringstream ss;
                ss << "? " << nama << " membatalkan '" << u->film
                   << "' di " << u->studio << " (" << u->jadwal << ")";
                histori.push(ss.str());

                cout << "? Pesanan dibatalkan dan kursi dikosongkan.\n";
                u->film = "";
                u->studio = "";
                u->jadwal = "";
                u->kursi = -1;
            }
            system("pause");
            system("cls");
        }

        /* ======== 4. Lihat Status Kursi (Tree) ======== */
        else if (pilih == 4) {
            bioskop.tampilFilm();
            int pf;
            cout << "Pilih film: ";
            cin >> pf;
            if (pf < 1 || pf > bioskop.jumlahFilm) continue;
            pf--;

            bioskop.tampilStudio(pf);
            int ps;
            cout << "Pilih studio: ";
            cin >> ps;
            if (ps < 1 || ps > bioskop.daftarFilm[pf].jumlahStudio) continue;
            ps--;

            tampilKursi(bioskop.daftarFilm[pf].daftarStudio[ps].kursiRoot);
            system("pause");
            system("cls");
        }

        /* ======== 5. Lihat Riwayat (Stack) ======== */
        else if (pilih == 5) {
            cout << "\n=== RIWAYAT ===\n";
            if (histori.kosong()) cout << "Belum ada transaksi.\n";
            else histori.tampil();
            system("pause");
            system("cls");
        }

        /* ======== 6. Kosongkan Studio (Tree) ======== */
        else if (pilih == 6) {
            bioskop.tampilFilm();
            int pf;
            cout << "Pilih film: ";
            cin >> pf;
            if (pf < 1 || pf > bioskop.jumlahFilm) continue;
            pf--;

            bioskop.tampilStudio(pf);
            int ps;
            cout << "Pilih studio: ";
            cin >> ps;
            if (ps < 1 || ps > bioskop.daftarFilm[pf].jumlahStudio) continue;
            ps--;

            kosongkanKursi(bioskop.daftarFilm[pf].daftarStudio[ps].kursiRoot);
            stringstream ss;
            ss << "?? Studio '" << bioskop.daftarFilm[pf].daftarStudio[ps].namaStudio
               << "' untuk '" << bioskop.daftarFilm[pf].judul << "' dikosongkan.";
            histori.push(ss.str());
            cout << "? Studio dikosongkan!\n";
            system("pause");
            system("cls");
        }

        /* ======== 7. Keluar ======== */
        else if (pilih == 7) {
            cout << "Terima kasih telah menggunakan sistem ini! ??\n";
        }
        
        else {
        	cout<<"tidak valid ";
        	break;
    
		}

    } while (pilih != 7);

    return 0;
}

