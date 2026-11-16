#include "raylib.h"
#include <iostream>
#include <math.h>

using namespace std;

const int TOI_DA_KHUNG = 10;
const int TOI_DA_TRANG = 25;

// Ham chuyen chuoi ky tu sang so nguyen
int chuyenChuoiThanhSo(char chuoi[]) {
    int ketQua = 0;
    int i = 0;
    while (chuoi[i] != '\0') {
        if (chuoi[i] >= '0' && chuoi[i] <= '9') {
            ketQua = ketQua * 10 + (chuoi[i] - '0');
        }
        i++;
    }
    return ketQua;
}

// Ham dem so luong trang hien co trong chuoi nhap
int demSoTrangTrongChuoi(char chuoiNhap[]) {
    int dem = 0;
    bool dangDocSo = false;
    for (int i = 0; chuoiNhap[i] != '\0'; i++) {
        if (chuoiNhap[i] >= '0' && chuoiNhap[i] <= '9') {
            if (!dangDocSo) {
                dem++;
                dangDocSo = true;
            }
        }
        else {
            dangDocSo = false;
        }
    }
    return dem;
}

// Ham cat bot chuoi trang neu nguoi dung giam "So luong trang" o tren
void catBotChuoiTrang(char chuoiNhap[], int& doDaiChuoi, int soTrangGioiHan) {
    if (soTrangGioiHan <= 0) {
        chuoiNhap[0] = '\0';
        doDaiChuoi = 0;
        return;
    }
    int dem = 0;
    bool dangDocSo = false;
    int viTriCat = 0;

    for (int i = 0; chuoiNhap[i] != '\0'; i++) {
        if (chuoiNhap[i] >= '0' && chuoiNhap[i] <= '9') {
            if (!dangDocSo) {
                dem++;
                dangDocSo = true;
            }
            if (dem <= soTrangGioiHan) {
                viTriCat = i + 1;
            }
            else {
                break;
            }
        }
        else {
            dangDocSo = false;
            if (dem < soTrangGioiHan) {
                viTriCat = i + 1;
            }
            else {
                break; // Da du so trang gioi han, cat bo phan du thua phia sau
            }
        }
    }
    chuoiNhap[viTriCat] = '\0';
    doDaiChuoi = viTriCat;
}

// Ham tach chuoi trang cach nhau boi dau cach thanh mang so nguyen
int tachChuoiTrang(char chuoiNhap[], int chuoiTrang[], int soTrangToiDa) {
    int demTrang = 0;
    int soHienTai = 0;
    bool dangDocSo = false;

    for (int i = 0; chuoiNhap[i] != '\0'; i++) {
        if (chuoiNhap[i] >= '0' && chuoiNhap[i] <= '9') {
            soHienTai = soHienTai * 10 + (chuoiNhap[i] - '0');
            dangDocSo = true;
        }
        else {
            if (dangDocSo && demTrang < soTrangToiDa) {
                chuoiTrang[demTrang] = soHienTai;
                demTrang++;
                soHienTai = 0;
                dangDocSo = false;
            }
        }
    }
    if (dangDocSo && demTrang < soTrangToiDa) {
        chuoiTrang[demTrang] = soHienTai;
        demTrang++;
    }
    return demTrang;
}

// Ham chuyen so nguyen duong thanh chuoi de ve bang Raylib DrawText
void soThanhChuoi(int so, char chuoiDich[]) {
    if (so < 0) {
        chuoiDich[0] = '-';
        chuoiDich[1] = '\0';
        return;
    }
    if (so == 0) {
        chuoiDich[0] = '0';
        chuoiDich[1] = '\0';
        return;
    }
    char tam[20];
    int dem = 0;
    while (so > 0) {
        tam[dem] = (char)('0' + (so % 10));
        so /= 10;
        dem++;
    }
    for (int i = 0; i < dem; i++) {
        chuoiDich[i] = tam[dem - 1 - i];
    }
    chuoiDich[dem] = '\0';
}

// Ham kiem tra trang co nam trong khung hien tai khong
bool kiemTraTonTai(int khungHienTai[], int soKhungTrang, int trangCanTim) {
    for (int i = 0; i < soKhungTrang; i++) {
        if (khungHienTai[i] == trangCanTim) return true;
    }
    return false;
}

// Ham tinh toan mo phong theo loai thuat toan: 1 = FIFO, 2 = OPT, 3 = LRU
void chayMoPhong(int loaiThuatToan, int chuoiTrang[], int soTrang, int soKhungTrang,
    int bangKetQua[TOI_DA_KHUNG][TOI_DA_TRANG], bool mangLoiTrang[],
    int viTriBiThay[TOI_DA_TRANG], int& tongLoiTrang) {
    int khungHienTai[TOI_DA_KHUNG];
    for (int i = 0; i < soKhungTrang; i++) khungHienTai[i] = -1;

    tongLoiTrang = 0;
    int conTroFIFO = 0;

    for (int j = 0; j < soTrang; j++) {
        int trangMoi = chuoiTrang[j];

        if (!kiemTraTonTai(khungHienTai, soKhungTrang, trangMoi)) {
            int viTriThay = -1;

            if (loaiThuatToan == 1) { // FIFO
                viTriThay = conTroFIFO;
                conTroFIFO = (conTroFIFO + 1) % soKhungTrang;
            }
            else if (loaiThuatToan == 2) { // OPT
                for (int i = 0; i < soKhungTrang; i++) {
                    if (khungHienTai[i] == -1) { viTriThay = i; break; }
                }
                if (viTriThay == -1) {
                    int xaNhat = -1;
                    for (int i = 0; i < soKhungTrang; i++) {
                        int lanToi = soTrang + 1;
                        for (int k = j + 1; k < soTrang; k++) {
                            if (khungHienTai[i] == chuoiTrang[k]) { lanToi = k; break; }
                        }
                        if (lanToi > xaNhat) { xaNhat = lanToi; viTriThay = i; }
                    }
                }
            }
            else if (loaiThuatToan == 3) { // LRU
                for (int i = 0; i < soKhungTrang; i++) {
                    if (khungHienTai[i] == -1) { viTriThay = i; break; }
                }
                if (viTriThay == -1) {
                    int lauNhat = soTrang + 1;
                    for (int i = 0; i < soKhungTrang; i++) {
                        int ganNhat = -1;
                        for (int k = j - 1; k >= 0; k--) {
                            if (khungHienTai[i] == chuoiTrang[k]) { ganNhat = k; break; }
                        }
                        if (ganNhat < lauNhat) { lauNhat = ganNhat; viTriThay = i; }
                    }
                }
            }

            khungHienTai[viTriThay] = trangMoi;
            mangLoiTrang[j] = true;
            viTriBiThay[j] = viTriThay;
            tongLoiTrang++;
        }
        else {
            mangLoiTrang[j] = false;
            viTriBiThay[j] = -1;
        }

        for (int i = 0; i < soKhungTrang; i++) {
            bangKetQua[i][j] = khungHienTai[i];
        }
    }
}

int main() {
    const int chieuRongCuaSo = 1150;
    const int chieuCaoCuaSo = 720;
    InitWindow(chieuRongCuaSo, chieuCaoCuaSo, "De Tai 10 - Mo Phong Thay The Trang (HDH) - C++ & Raylib");
    SetTargetFPS(60);

    // Du lieu mac dinh trong 3 o nhap ban phim
    char oNhapKhung[10] = "0";
    int doDaiKhung = 1;

    char oNhapSoTrang[10] = "0";
    int doDaiSoTrang = 2;

    char oNhapChuoiTrang[150] = " ";
    int doDaiChuoi = 23;

    // 0: dang chon o Khung Trang, 1: o So Trang, 2: o Chuoi Trang
    int oDangChon = 0;
    int thuatToanDangChon = 1; // 1: FIFO, 2: OPT, 3: LRU

    int soKhungTrang = 3;
    int soTrang = 12;
    int chuoiTrang[TOI_DA_TRANG];
    int bangKetQua[TOI_DA_KHUNG][TOI_DA_TRANG];
    bool mangLoiTrang[TOI_DA_TRANG];
    int viTriBiThay[TOI_DA_TRANG];
    int tongLoiTrang = 0;

    // Toa do cac o nhap va nut bam
    Rectangle hinhOKhung = { 220, 70, 100, 36 };
    Rectangle hinhOSoTrang = { 530, 70, 100, 36 };
    Rectangle hinhOChuoi = { 220, 120, 650, 36 };

    Rectangle nutFIFO = { 220, 175, 160, 40 };
    Rectangle nutOPT = { 400, 175, 185, 40 };
    Rectangle nutLRU = { 605, 175, 160, 40 };

    while (!WindowShouldClose()) {
        Vector2 viTriChuot = GetMousePosition();

        // Lay gioi han so trang hien tai tu o "So luong trang"
        int soTrangGioiHan = chuyenChuoiThanhSo(oNhapSoTrang);
        if (soTrangGioiHan < 1) soTrangGioiHan = 1;
        if (soTrangGioiHan > TOI_DA_TRANG) soTrangGioiHan = TOI_DA_TRANG;

        // Xu ly click chuot chon o nhap hoac nut thuat toan
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(viTriChuot, hinhOKhung)) oDangChon = 0;
            else if (CheckCollisionPointRec(viTriChuot, hinhOSoTrang)) oDangChon = 1;
            else if (CheckCollisionPointRec(viTriChuot, hinhOChuoi)) oDangChon = 2;

            if (CheckCollisionPointRec(viTriChuot, nutFIFO)) thuatToanDangChon = 1;
            if (CheckCollisionPointRec(viTriChuot, nutOPT))  thuatToanDangChon = 2;
            if (CheckCollisionPointRec(viTriChuot, nutLRU))  thuatToanDangChon = 3;
        }

        // Nhan TAB hoac ENTER de chuyen nhanh giua cac o nhap
        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ENTER)) {
            oDangChon = (oDangChon + 1) % 3;
        }

        // Neu nguoi dung da roi khoi o "So luong trang", tu dong cat gon chuoi trang theo gioi han moi
        if (oDangChon != 1 && doDaiSoTrang > 0) {
            catBotChuoiTrang(oNhapChuoiTrang, doDaiChuoi, soTrangGioiHan);
        }

        // Doc ky tu nguoi dung go tu ban phim
        int kyTu = GetCharPressed();
        while (kyTu > 0) {
            if (oDangChon == 0 && (kyTu >= '0' && kyTu <= '9') && doDaiKhung < 2) {
                oNhapKhung[doDaiKhung] = (char)kyTu;
                doDaiKhung++;
                oNhapKhung[doDaiKhung] = '\0';
            }
            else if (oDangChon == 1 && (kyTu >= '0' && kyTu <= '9') && doDaiSoTrang < 2) {
                oNhapSoTrang[doDaiSoTrang] = (char)kyTu;
                doDaiSoTrang++;
                oNhapSoTrang[doDaiSoTrang] = '\0';
            }
            else if (oDangChon == 2) {
                int soTrangHienCo = demSoTrangTrongChuoi(oNhapChuoiTrang);

                // Truong hop 1: Nguoi dung go phim Cach (Space)
                if (kyTu == ' ') {
                    // Chi cho phep go dau cach khi chua du so trang gioi han va ky tu truoc khong phai dau cach
                    if (doDaiChuoi > 0 && oNhapChuoiTrang[doDaiChuoi - 1] != ' ' && soTrangHienCo < soTrangGioiHan) {
                        oNhapChuoiTrang[doDaiChuoi] = ' ';
                        doDaiChuoi++;
                        oNhapChuoiTrang[doDaiChuoi] = '\0';
                    }
                }
                // Truong hop 2: Nguoi dung go chu so (0 - 9)
                else if (kyTu >= '0' && kyTu <= '9') {
                    bool batDauTrangMoi = (doDaiChuoi == 0 || oNhapChuoiTrang[doDaiChuoi - 1] == ' ');

                    if (batDauTrangMoi) {
                        // Chi cho phep bat dau trang moi neu so trang hien co NHO HON so trang gioi han
                        if (soTrangHienCo < soTrangGioiHan) {
                            oNhapChuoiTrang[doDaiChuoi] = (char)kyTu;
                            doDaiChuoi++;
                            oNhapChuoiTrang[doDaiChuoi] = '\0';
                        }
                    }
                    else {
                        // Dang go tiep chu so thu 2 cua trang hien tai (gioi han moi trang toi da 2 chu so: 0-99)
                        bool chuaQuaHaiChuSo = (doDaiChuoi < 2 || oNhapChuoiTrang[doDaiChuoi - 2] == ' ');
                        if (soTrangHienCo <= soTrangGioiHan && chuaQuaHaiChuSo) {
                            oNhapChuoiTrang[doDaiChuoi] = (char)kyTu;
                            doDaiChuoi++;
                            oNhapChuoiTrang[doDaiChuoi] = '\0';
                        }
                    }
                }
            }
            kyTu = GetCharPressed();
        }

        // Xu ly phim Backspace xoa ky tu
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (oDangChon == 0 && doDaiKhung > 0) {
                doDaiKhung--;
                oNhapKhung[doDaiKhung] = '\0';
            }
            else if (oDangChon == 1 && doDaiSoTrang > 0) {
                doDaiSoTrang--;
                oNhapSoTrang[doDaiSoTrang] = '\0';
            }
            else if (oDangChon == 2 && doDaiChuoi > 0) {
                doDaiChuoi--;
                oNhapChuoiTrang[doDaiChuoi] = '\0';
            }
        }

        // Cap nhat gia tri soKhungTrang, soTrang va chay mo phong
        soKhungTrang = chuyenChuoiThanhSo(oNhapKhung);
        if (soKhungTrang < 1) soKhungTrang = 1;
        if (soKhungTrang > TOI_DA_KHUNG) soKhungTrang = TOI_DA_KHUNG;

        for (int i = 0; i < TOI_DA_TRANG; i++) chuoiTrang[i] = 0;
        int soTrangThucTe = tachChuoiTrang(oNhapChuoiTrang, chuoiTrang, TOI_DA_TRANG);
        soTrang = (soTrangGioiHan < soTrangThucTe) ? soTrangGioiHan : soTrangThucTe;

        if (soTrang > 0) {
            chayMoPhong(thuatToanDangChon, chuoiTrang, soTrang, soKhungTrang, bangKetQua, mangLoiTrang, viTriBiThay, tongLoiTrang);
        }
        else {
            tongLoiTrang = 0;
        }

        // ================= VE GIAO DIEN =================
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Tieu de
        DrawRectangle(0, 0, chieuRongCuaSo, 50, DARKBLUE);
        DrawText("DE TAI 10: MO PHONG THUAT TOAN THAY THE TRANG (FIFO - OPT - LRU)", 20, 15, 22, WHITE);

        // Ve 3 o nhap du lieu
        DrawText("So khung trang (1-10):", 20, 78, 18, DARKGRAY);
        DrawRectangleRec(hinhOKhung, (oDangChon == 0) ? SKYBLUE : LIGHTGRAY);
        DrawRectangleLinesEx(hinhOKhung, 2, (oDangChon == 0) ? BLUE : GRAY);
        DrawText(oNhapKhung, (int)hinhOKhung.x + 10, (int)hinhOKhung.y + 8, 20, BLACK);

        DrawText("So luong trang (1-25):", 340, 78, 18, DARKGRAY);
        DrawRectangleRec(hinhOSoTrang, (oDangChon == 1) ? SKYBLUE : LIGHTGRAY);
        DrawRectangleLinesEx(hinhOSoTrang, 2, (oDangChon == 1) ? BLUE : GRAY);
        DrawText(oNhapSoTrang, (int)hinhOSoTrang.x + 10, (int)hinhOSoTrang.y + 8, 20, BLACK);

        // Hien thi trang thai dem so trang da nhap so voi gioi han
        int soTrangDaGo = demSoTrangTrongChuoi(oNhapChuoiTrang);
        Color mauDemTrang = (soTrangDaGo == soTrangGioiHan) ? DARKGREEN : MAROON;
        DrawText(TextFormat("Da nhap: %d / %d trang", soTrangDaGo, soTrangGioiHan), 650, 78, 18, mauDemTrang);

        DrawText("Chuoi trang (cach dau cach):", 20, 128, 16, DARKGRAY);
        DrawRectangleRec(hinhOChuoi, (oDangChon == 2) ? SKYBLUE : LIGHTGRAY);
        DrawRectangleLinesEx(hinhOChuoi, 2, (oDangChon == 2) ? BLUE : GRAY);

        // Dung Scissor Mode de dam bao chu khong bao gio tran ra ngoai khung hinhOChuoi
        BeginScissorMode((int)hinhOChuoi.x, (int)hinhOChuoi.y, (int)hinhOChuoi.width, (int)hinhOChuoi.height);
        DrawText(oNhapChuoiTrang, (int)hinhOChuoi.x + 10, (int)hinhOChuoi.y + 8, 20, BLACK);
        EndScissorMode();

        if (soTrangDaGo >= soTrangGioiHan) {
            DrawText("Da du so trang gioi han!", 885, 130, 15, DARKGREEN);
        }
        else {
            DrawText("(Click hoac nhan TAB de doi o)", 885, 130, 14, GRAY);
        }

        // Ve cac nut chon thuat toan
        DrawText("Chon thuat toan:", 20, 185, 18, DARKGRAY);

        DrawRectangleRec(nutFIFO, (thuatToanDangChon == 1) ? MAROON : DARKGRAY);
        DrawText("1. FIFO", (int)nutFIFO.x + 45, (int)nutFIFO.y + 10, 20, WHITE);

        DrawRectangleRec(nutOPT, (thuatToanDangChon == 2) ? MAROON : DARKGRAY);
        DrawText("2. OPT (Toi uu)", (int)nutOPT.x + 20, (int)nutOPT.y + 10, 20, WHITE);

        DrawRectangleRec(nutLRU, (thuatToanDangChon == 3) ? MAROON : DARKGRAY);
        DrawText("3. LRU", (int)nutLRU.x + 45, (int)nutLRU.y + 10, 20, WHITE);

        // ================= VE BANG MO PHONG =================
        int gocX = 130;
        int gocY = 245;
        int rongO = 40;
        int caoO = 34;

        // Hang tieu de: Chuoi trang tham chieu
        DrawText("Trang:", 25, gocY + 8, 18, DARKBLUE);
        for (int j = 0; j < soTrang; j++) {
            int x = gocX + j * rongO;
            DrawRectangle(x, gocY, rongO - 2, caoO - 2, DARKBLUE);
            char chuoiSo[10];
            soThanhChuoi(chuoiTrang[j], chuoiSo);
            DrawText(chuoiSo, x + 12, gocY + 7, 18, YELLOW);
        }

        // Cac hang Khung trang
        for (int i = 0; i < soKhungTrang; i++) {
            int y = gocY + (i + 1) * caoO;
            char nhanKhung[20] = "Khung ";
            char soK[5];
            soThanhChuoi(i + 1, soK);
            nhanKhung[6] = soK[0];
            nhanKhung[7] = soK[1];
            nhanKhung[8] = '\0';
            DrawText(nhanKhung, 25, y + 8, 17, BLACK);

            for (int j = 0; j < soTrang; j++) {
                int x = gocX + j * rongO;
                Color mauNen = LIGHTGRAY;

                if (mangLoiTrang[j] && viTriBiThay[j] == i) {
                    mauNen = ORANGE; // To mau cam cho khung vua bi thay the (Page Fault)
                }
                else if (!mangLoiTrang[j] && bangKetQua[i][j] == chuoiTrang[j]) {
                    mauNen = GREEN;  // To mau xanh la cho khung trung trang (Page Hit)
                }

                DrawRectangle(x, y, rongO - 2, caoO - 2, mauNen);
                DrawRectangleLines(x, y, rongO - 2, caoO - 2, GRAY);

                if (bangKetQua[i][j] != -1) {
                    char chuoiGiaTri[10];
                    soThanhChuoi(bangKetQua[i][j], chuoiGiaTri);
                    DrawText(chuoiGiaTri, x + 12, y + 7, 18, BLACK);
                }
                else {
                    DrawText("-", x + 14, y + 7, 18, DARKGRAY);
                }
            }
        }

        // Hang danh dau Loi trang (L) / Trung trang (H)
        int yLoi = gocY + (soKhungTrang + 1) * caoO + 5;
        DrawText("Loi trang:", 25, yLoi + 6, 17, RED);
        for (int j = 0; j < soTrang; j++) {
            int x = gocX + j * rongO;
            if (mangLoiTrang[j]) {
                DrawRectangle(x, yLoi, rongO - 2, caoO - 2, RED);
                DrawText("L", x + 13, yLoi + 7, 18, WHITE);
            }
            else {
                DrawRectangle(x, yLoi, rongO - 2, caoO - 2, LIME);
                DrawText("H", x + 13, yLoi + 7, 18, BLACK);
            }
        }

        // Thong ke ket qua ben duoi
        int yThongKe = chieuCaoCuaSo - 75;
        DrawRectangle(20, yThongKe, chieuRongCuaSo - 40, 60, LIGHTGRAY);
        DrawRectangleLines(20, yThongKe, chieuRongCuaSo - 40, 60, DARKBLUE);

        int tyLePhanTram = (soTrang > 0) ? (int)round(((double)tongLoiTrang / soTrang) * 100.0) : 0;
        int soLanTrung = soTrang - tongLoiTrang;

        DrawText(TextFormat("Tong so Loi trang (L - Cam/Do): %d", tongLoiTrang), 40, yThongKe + 18, 20, MAROON);
        DrawText(TextFormat("So lan Trung trang (H - Xanh): %d", soLanTrung), 420, yThongKe + 18, 20, DARKGREEN);
        DrawText(TextFormat("Ty le Loi trang: %d%%", tyLePhanTram), 790, yThongKe + 18, 20, DARKBLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}