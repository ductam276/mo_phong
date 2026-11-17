#include "raylib.h"
#include <math.h>

const int TOI_DA_KHUNG = 10;
const int TOI_DA_TRANG = 25;
const int DO_DAI_CHUOI_TOI_DA = 150;

enum ThuatToan {
    FIFO = 1,
    MRU = 2,
    LRU = 3
};

int chuyenChuoiThanhSo(const char chuoi[]) {
    int ketQua = 0;

    for (int i = 0; chuoi[i] != '\0'; i++) {
        if (chuoi[i] >= '0' && chuoi[i] <= '9') {
            ketQua = ketQua * 10 + (chuoi[i] - '0');
        }
    }

    return ketQua;
}

int demSoTrangTrongChuoi(const char chuoiNhap[]) {
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

int tachChuoiTrang(const char chuoiNhap[], int chuoiTrang[], int soTrangToiDa) {
    int demTrang = 0;
    int soHienTai = 0;
    bool dangDocSo = false;

    for (int i = 0; chuoiNhap[i] != '\0'; i++) {
        if (chuoiNhap[i] >= '0' && chuoiNhap[i] <= '9') {
            soHienTai = soHienTai * 10 + (chuoiNhap[i] - '0');
            dangDocSo = true;
        }
        else if (dangDocSo) {
            if (demTrang < soTrangToiDa) {
                chuoiTrang[demTrang++] = soHienTai;
            }
            soHienTai = 0;
            dangDocSo = false;
        }
    }

    if (dangDocSo && demTrang < soTrangToiDa) {
        chuoiTrang[demTrang++] = soHienTai;
    }

    return demTrang;
}

void catBotChuoiTrang(char chuoiNhap[], int& doDaiChuoi, int soTrangGioiHan) {
    int demTrang = 0;
    bool dangDocSo = false;
    int viTriCat = 0;

    for (int i = 0; chuoiNhap[i] != '\0'; i++) {
        bool laChuSo = chuoiNhap[i] >= '0' && chuoiNhap[i] <= '9';

        if (laChuSo) {
            if (!dangDocSo) {
                if (demTrang == soTrangGioiHan) break;
                demTrang++;
                dangDocSo = true;
            }
            viTriCat = i + 1;
        }
        else {
            if (demTrang == soTrangGioiHan) break;
            dangDocSo = false;
            viTriCat = i + 1;
        }
    }
    chuoiNhap[viTriCat] = '\0';
    doDaiChuoi = viTriCat;
}

int timViTriTrang(const int khungHienTai[], int soKhungTrang, int trangCanTim) {
    for (int i = 0; i < soKhungTrang; i++) {
        if (khungHienTai[i] == trangCanTim) return i;
    }
    return -1;
}

int timKhungTrong(const int khungHienTai[], int soKhungTrang) {
    for (int i = 0; i < soKhungTrang; i++) {
        if (khungHienTai[i] == -1) return i;
    }

    return -1;
}

int timKhungXaNhat(const int lanTruyCapGanNhat[], int soKhungTrang) {
    int viTri = 0;

    for (int i = 1; i < soKhungTrang; i++) {
        if (lanTruyCapGanNhat[i] < lanTruyCapGanNhat[viTri]) {
            viTri = i;
        }
    }

    return viTri;
}

int timKhungGanNhatNhat(const int lanTruyCapGanNhat[], int soKhungTrang) {
    int viTri = 0;
    for (int i = 1; i < soKhungTrang; i++) {
        if (lanTruyCapGanNhat[i] > lanTruyCapGanNhat[viTri]) {
            viTri = i;
        }
    }
    return viTri;
}


void chayMoPhong(int loaiThuatToan, const int chuoiTrang[], int soTrang, int soKhungTrang,
    int bangKetQua[TOI_DA_KHUNG][TOI_DA_TRANG], bool mangLoiTrang[],
    int viTriBiThay[TOI_DA_TRANG], int& tongLoiTrang) {
    int khungHienTai[TOI_DA_KHUNG];
    int lanTruyCapGanNhat[TOI_DA_KHUNG];
    int conTroFIFO = 0;

    for (int i = 0; i < soKhungTrang; i++) {
        khungHienTai[i] = -1;
        lanTruyCapGanNhat[i] = -1;
    }

    tongLoiTrang = 0;

    for (int j = 0; j < soTrang; j++) {
        int trangMoi = chuoiTrang[j];
        int viTriTrang = timViTriTrang(khungHienTai, soKhungTrang, trangMoi);

        if (viTriTrang != -1) {
            mangLoiTrang[j] = false;
            viTriBiThay[j] = -1;
            lanTruyCapGanNhat[viTriTrang] = j;
        }
        else {
            int viTriThay = -1;

            if (loaiThuatToan == FIFO) {
                viTriThay = conTroFIFO;
                conTroFIFO = (conTroFIFO + 1) % soKhungTrang;
            }
            else {
                viTriThay = timKhungTrong(khungHienTai, soKhungTrang);

                if (viTriThay == -1 && loaiThuatToan == LRU) {
                    viTriThay = timKhungXaNhat(lanTruyCapGanNhat, soKhungTrang);
                }
                else if (viTriThay == -1 && loaiThuatToan == MRU) {
                    viTriThay = timKhungGanNhatNhat(lanTruyCapGanNhat, soKhungTrang);
                }
            }

            khungHienTai[viTriThay] = trangMoi;
            lanTruyCapGanNhat[viTriThay] = j;
            mangLoiTrang[j] = true;
            viTriBiThay[j] = viTriThay;
            tongLoiTrang++;
        }

        for (int i = 0; i < soKhungTrang; i++) {
            bangKetQua[i][j] = khungHienTai[i];
        }
    }
}

void themKyTu(char chuoi[], int& doDai, int sucChua, char kyTu) {
    if (doDai >= sucChua - 1) return;

    chuoi[doDai++] = kyTu;
    chuoi[doDai] = '\0';
}

void xoaKyTuCuoi(char chuoi[], int& doDai) {
    if (doDai <= 0) return;

    chuoi[--doDai] = '\0';
}

int soChuSoTrangDangNhap(const char chuoi[], int doDai) {
    int dem = 0;

    for (int i = doDai - 1; i >= 0 && chuoi[i] != ' '; i--) {
        dem++;
    }

    return dem;
}

void veSoGiuaO(int so, int x, int y, int rongO, int kichThuocChu, Color mauChu) {
    const char* chuoiSo = TextFormat("%d", so);
    int rongChu = MeasureText(chuoiSo, kichThuocChu);
    DrawText(chuoiSo, x + (rongO - rongChu) / 2, y, kichThuocChu, mauChu);
}

int main() {
    const int CHIEU_RONG_CUA_SO = 1150;
    const int CHIEU_CAO_CUA_SO = 720;

    InitWindow(CHIEU_RONG_CUA_SO, CHIEU_CAO_CUA_SO, "Mo phong thuat toan thay the trang");
    SetTargetFPS(60);

    char oNhapKhung[3] = "";
    char oNhapSoTrang[3] = "";
    char oNhapChuoiTrang[DO_DAI_CHUOI_TOI_DA] = "";
    int doDaiKhung = 0;
    int doDaiSoTrang = 0;
    int doDaiChuoi = 0;
    int oDangChon = 0;
    ThuatToan thuatToanDangChon = FIFO;

    int soKhungTrang = 1;
    int soTrang = 0;
    int chuoiTrang[TOI_DA_TRANG] = {};
    int bangKetQua[TOI_DA_KHUNG][TOI_DA_TRANG] = {};
    bool mangLoiTrang[TOI_DA_TRANG] = {};
    int viTriBiThay[TOI_DA_TRANG] = {};
    int tongLoiTrang = 0;

    Rectangle hinhOKhung = { 220, 70, 100, 36 };
    Rectangle hinhOSoTrang = { 530, 70, 100, 36 };
    Rectangle hinhOChuoi = { 220, 120, 650, 36 };
    Rectangle nutFIFO = { 220, 175, 160, 40 };
    Rectangle nutMRU = { 400, 175, 185, 40 };
    Rectangle nutLRU = { 605, 175, 160, 40 };

    while (!WindowShouldClose()) {
        Vector2 viTriChuot = GetMousePosition();
        int soTrangNhap = chuyenChuoiThanhSo(oNhapSoTrang);
        int gioiHanNhapTrang = soTrangNhap;

        if (gioiHanNhapTrang < 1 || gioiHanNhapTrang > TOI_DA_TRANG) {
            gioiHanNhapTrang = TOI_DA_TRANG;
        }

        if (oDangChon != 1 && soTrangNhap >= 1 && soTrangNhap <= TOI_DA_TRANG) {
            catBotChuoiTrang(oNhapChuoiTrang, doDaiChuoi, soTrangNhap);
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(viTriChuot, hinhOKhung)) oDangChon = 0;
            else if (CheckCollisionPointRec(viTriChuot, hinhOSoTrang)) oDangChon = 1;
            else if (CheckCollisionPointRec(viTriChuot, hinhOChuoi)) oDangChon = 2;

            if (CheckCollisionPointRec(viTriChuot, nutFIFO)) thuatToanDangChon = FIFO;
            if (CheckCollisionPointRec(viTriChuot, nutMRU)) thuatToanDangChon = MRU;
            if (CheckCollisionPointRec(viTriChuot, nutLRU)) thuatToanDangChon = LRU;
        }

        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ENTER)) {
            oDangChon = (oDangChon + 1) % 3;
        }

        int kyTu = GetCharPressed();
        while (kyTu > 0) {
            if (kyTu >= '0' && kyTu <= '9') {
                if (oDangChon == 0 && doDaiKhung < 2) {
                    themKyTu(oNhapKhung, doDaiKhung, (int)sizeof(oNhapKhung), (char)kyTu);
                }
                else if (oDangChon == 1 && doDaiSoTrang < 2) {
                    themKyTu(oNhapSoTrang, doDaiSoTrang, (int)sizeof(oNhapSoTrang), (char)kyTu);
                }
                else if (oDangChon == 2) {
                    bool batDauTrangMoi = doDaiChuoi == 0 || oNhapChuoiTrang[doDaiChuoi - 1] == ' ';
                    int soTrangDaNhap = demSoTrangTrongChuoi(oNhapChuoiTrang);

                    if ((batDauTrangMoi && soTrangDaNhap < gioiHanNhapTrang) ||
                        (!batDauTrangMoi && soChuSoTrangDangNhap(oNhapChuoiTrang, doDaiChuoi) < 2)) {
                        themKyTu(oNhapChuoiTrang, doDaiChuoi, DO_DAI_CHUOI_TOI_DA, (char)kyTu);
                    }
                }
            }
            else if (oDangChon == 2 && kyTu == ' ' && doDaiChuoi > 0 &&
                oNhapChuoiTrang[doDaiChuoi - 1] != ' ' &&
                demSoTrangTrongChuoi(oNhapChuoiTrang) < gioiHanNhapTrang) {
                themKyTu(oNhapChuoiTrang, doDaiChuoi, DO_DAI_CHUOI_TOI_DA, ' ');
            }

            kyTu = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (oDangChon == 0) xoaKyTuCuoi(oNhapKhung, doDaiKhung);
            else if (oDangChon == 1) xoaKyTuCuoi(oNhapSoTrang, doDaiSoTrang);
            else xoaKyTuCuoi(oNhapChuoiTrang, doDaiChuoi);
        }

        soKhungTrang = chuyenChuoiThanhSo(oNhapKhung);
        if (soKhungTrang < 1) soKhungTrang = 1;
        if (soKhungTrang > TOI_DA_KHUNG) soKhungTrang = TOI_DA_KHUNG;

        int soTrangGioiHan = chuyenChuoiThanhSo(oNhapSoTrang);
        if (soTrangGioiHan < 1) soTrangGioiHan = 1;
        if (soTrangGioiHan > TOI_DA_TRANG) soTrangGioiHan = TOI_DA_TRANG;

        int soTrangThucTe = tachChuoiTrang(oNhapChuoiTrang, chuoiTrang, TOI_DA_TRANG);
        soTrang = soTrangThucTe < soTrangGioiHan ? soTrangThucTe : soTrangGioiHan;

        if (soTrang > 0) {
            chayMoPhong(thuatToanDangChon, chuoiTrang, soTrang, soKhungTrang,
                bangKetQua, mangLoiTrang, viTriBiThay, tongLoiTrang);
        }
        else {
            tongLoiTrang = 0;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(0, 0, CHIEU_RONG_CUA_SO, 50, DARKBLUE);
        DrawText("Mo phong thuat toan thay the trang",
            20, 15, 22, WHITE);

        DrawText("So khung trang (1-10):", 20, 78, 18, DARKGRAY);
        DrawRectangleRec(hinhOKhung, oDangChon == 0 ? SKYBLUE : LIGHTGRAY);
        DrawRectangleLinesEx(hinhOKhung, 2, oDangChon == 0 ? BLUE : GRAY);
        DrawText(oNhapKhung, (int)hinhOKhung.x + 10, (int)hinhOKhung.y + 8, 20, BLACK);

        DrawText("So luong trang (1-25):", 340, 78, 18, DARKGRAY);
        DrawRectangleRec(hinhOSoTrang, oDangChon == 1 ? SKYBLUE : LIGHTGRAY);
        DrawRectangleLinesEx(hinhOSoTrang, 2, oDangChon == 1 ? BLUE : GRAY);
        DrawText(oNhapSoTrang, (int)hinhOSoTrang.x + 10, (int)hinhOSoTrang.y + 8, 20, BLACK);

        int soTrangDaGo = demSoTrangTrongChuoi(oNhapChuoiTrang);
        Color mauDemTrang = soTrangDaGo == soTrangGioiHan ? DARKGREEN : MAROON;
        DrawText(TextFormat("Da nhap: %d / %d trang", soTrangDaGo, soTrangGioiHan),
            650, 78, 18, mauDemTrang);

        DrawText("Chuoi trang:", 20, 128, 16, DARKGRAY);
        DrawRectangleRec(hinhOChuoi, oDangChon == 2 ? SKYBLUE : LIGHTGRAY);
        DrawRectangleLinesEx(hinhOChuoi, 2, oDangChon == 2 ? BLUE : GRAY);
        BeginScissorMode((int)hinhOChuoi.x, (int)hinhOChuoi.y,
            (int)hinhOChuoi.width, (int)hinhOChuoi.height);
        DrawText(oNhapChuoiTrang, (int)hinhOChuoi.x + 10, (int)hinhOChuoi.y + 8, 20, BLACK);
        EndScissorMode();

        if (soTrangDaGo >= soTrangGioiHan) {
            DrawText("Da du so trang gioi han!", 885, 130, 15, DARKGREEN);
        }
        else {
            DrawText("(Click hoac nhan TAB de doi o)", 885, 130, 14, GRAY);
        }

        DrawText("Chon thuat toan:", 20, 185, 18, DARKGRAY);
        DrawRectangleRec(nutFIFO, thuatToanDangChon == FIFO ? MAROON : DARKGRAY);
        DrawText("1. FIFO", (int)nutFIFO.x + 45, (int)nutFIFO.y + 10, 20, WHITE);

        DrawRectangleRec(nutMRU, thuatToanDangChon == MRU ? MAROON : DARKGRAY);
        DrawText("2. MRU", (int)nutMRU.x + 12, (int)nutMRU.y + 11, 17, WHITE);

        DrawRectangleRec(nutLRU, thuatToanDangChon == LRU ? MAROON : DARKGRAY);
        DrawText("3. LRU", (int)nutLRU.x + 45, (int)nutLRU.y + 10, 20, WHITE);

        int gocX = 130;
        int gocY = 245;
        int rongO = 40;
        int caoO = 30;

        DrawText("Trang:", 25, gocY + 6, 18, DARKBLUE);
        for (int j = 0; j < soTrang; j++) {
            int x = gocX + j * rongO;
            DrawRectangle(x, gocY, rongO - 2, caoO - 2, DARKBLUE);
            veSoGiuaO(chuoiTrang[j], x, gocY + 5, rongO - 2, 18, YELLOW);
        }

        for (int i = 0; i < soKhungTrang; i++) {
            int y = gocY + (i + 1) * caoO;
            DrawText(TextFormat("Khung %d", i + 1), 25, y + 6, 17, BLACK);

            for (int j = 0; j < soTrang; j++) {
                int x = gocX + j * rongO;
                Color mauNen = LIGHTGRAY;

                if (mangLoiTrang[j] && viTriBiThay[j] == i) {
                    mauNen = ORANGE;
                }
                else if (!mangLoiTrang[j] && bangKetQua[i][j] == chuoiTrang[j]) {
                    mauNen = GREEN;
                }

                DrawRectangle(x, y, rongO - 2, caoO - 2, mauNen);
                DrawRectangleLines(x, y, rongO - 2, caoO - 2, GRAY);

                if (bangKetQua[i][j] == -1) {
                    DrawText("-", x + 14, y + 5, 18, DARKGRAY);
                }
                else {
                    veSoGiuaO(bangKetQua[i][j], x, y + 5, rongO - 2, 18, BLACK);
                }
            }
        }

        int yLoi = gocY + (soKhungTrang + 1) * caoO + 5;
        DrawText("Loi trang:", 25, yLoi + 5, 17, RED);
        for (int j = 0; j < soTrang; j++) {
            int x = gocX + j * rongO;
            Color mauNen = mangLoiTrang[j] ? RED : LIME;
            const char* kyHieu = mangLoiTrang[j] ? "L" : "H";

            DrawRectangle(x, yLoi, rongO - 2, caoO - 2, mauNen);
            DrawText(kyHieu, x + 13, yLoi + 5, 18, mangLoiTrang[j] ? WHITE : BLACK);
        }

        int yThongKe = CHIEU_CAO_CUA_SO - 75;
        int soLanTrung = soTrang - tongLoiTrang;

        DrawRectangle(20, yThongKe, CHIEU_RONG_CUA_SO - 40, 60, LIGHTGRAY);
        DrawRectangleLines(20, yThongKe, CHIEU_RONG_CUA_SO - 40, 60, DARKBLUE);
        DrawText(TextFormat("Tong so Loi trang L (Vang): %d", tongLoiTrang),
            40, yThongKe + 18, 20, MAROON);
        DrawText(TextFormat("So lan Trung trang H (Xanh la): %d", soLanTrung),
            420, yThongKe + 18, 20, DARKGREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
