// ================================================================
// CAC THUAT TOAN THAY THE TRANG: FIFO - LRU - MRU
// File nay chi chua phan thuat toan, khong co Raylib hay giao dien.
// -1 co nghia la khung trang dang rong.
// ================================================================

const int TOI_DA_KHUNG = 10;
const int TOI_DA_TRANG = 25;

// Tim trangCanTim trong cac khung hien tai.
// Tra ve vi tri khung neu tim thay, nguoc lai tra ve -1.
int timViTriTrang(int khungHienTai[], int soKhungTrang, int trangCanTim) {
    for (int i = 0; i < soKhungTrang; i++) {
        if (khungHienTai[i] == trangCanTim) {
            return i;
        }
    }

    return -1;
}

// Tim khung dang rong de nap trang moi vao truoc.
int timKhungTrong(int khungHienTai[], int soKhungTrang) {
    for (int i = 0; i < soKhungTrang; i++) {
        if (khungHienTai[i] == -1) {
            return i;
        }
    }

    return -1;
}

// LRU: chon khung co lan truy cap gan nhat nho nhat.
// Nghia la trang trong khung do da lau nhat chua duoc dung lai.
int timKhungLRU(int lanTruyCapGanNhat[], int soKhungTrang) {
    int viTriLRU = 0;

    for (int i = 1; i < soKhungTrang; i++) {
        if (lanTruyCapGanNhat[i] < lanTruyCapGanNhat[viTriLRU]) {
            viTriLRU = i;
        }
    }

    return viTriLRU;
}

// MRU: chon khung co lan truy cap gan nhat lon nhat.
// Nghia la trang trong khung do vua duoc dung gan nhat.
int timKhungMRU(int lanTruyCapGanNhat[], int soKhungTrang) {
    int viTriMRU = 0;

    for (int i = 1; i < soKhungTrang; i++) {
        if (lanTruyCapGanNhat[i] > lanTruyCapGanNhat[viTriMRU]) {
            viTriMRU = i;
        }
    }

    return viTriMRU;
}

// Luu trang thai cua tat ca khung sau moi buoc.
// bangKetQua[i][j] la gia tri cua khung i tai buoc j.
void luuTrangThaiKhung(int khungHienTai[], int soKhungTrang, int buoc,
    int bangKetQua[TOI_DA_KHUNG][TOI_DA_TRANG]) {
    for (int i = 0; i < soKhungTrang; i++) {
        bangKetQua[i][buoc] = khungHienTai[i];
    }
}

// FIFO - First In First Out.
// Khi khung day, thay trang duoc nap vao khung som nhat.
void chayFIFO(int chuoiTrang[], int soTrang, int soKhungTrang,
    int bangKetQua[TOI_DA_KHUNG][TOI_DA_TRANG], bool mangLoiTrang[],
    int viTriBiThay[TOI_DA_TRANG], int& tongLoiTrang) {
    int khungHienTai[TOI_DA_KHUNG];
    int conTroFIFO = 0;
    tongLoiTrang = 0;

    for (int i = 0; i < soKhungTrang; i++) {
        khungHienTai[i] = -1;
    }

    for (int j = 0; j < soTrang; j++) {
        int trangMoi = chuoiTrang[j];
        int viTriTrang = timViTriTrang(khungHienTai, soKhungTrang, trangMoi);

        if (viTriTrang == -1) {
            // Loi trang: thay tai vi tri ma con tro FIFO dang chi den.
            khungHienTai[conTroFIFO] = trangMoi;
            mangLoiTrang[j] = true;
            viTriBiThay[j] = conTroFIFO;
            tongLoiTrang++;

            // Chuyen con tro sang khung tiep theo theo vong tron.
            conTroFIFO = (conTroFIFO + 1) % soKhungTrang;
        }
        else {
            // Trung trang: khong thay doi noi dung khung.
            mangLoiTrang[j] = false;
            viTriBiThay[j] = -1;
        }

        luuTrangThaiKhung(khungHienTai, soKhungTrang, j, bangKetQua);
    }
}

// LRU - Least Recently Used.
// Khi khung day, thay trang da lau nhat chua duoc truy cap lai.
void chayLRU(int chuoiTrang[], int soTrang, int soKhungTrang,
    int bangKetQua[TOI_DA_KHUNG][TOI_DA_TRANG], bool mangLoiTrang[],
    int viTriBiThay[TOI_DA_TRANG], int& tongLoiTrang) {
    int khungHienTai[TOI_DA_KHUNG];
    int lanTruyCapGanNhat[TOI_DA_KHUNG];
    tongLoiTrang = 0;

    for (int i = 0; i < soKhungTrang; i++) {
        khungHienTai[i] = -1;
        lanTruyCapGanNhat[i] = -1;
    }

    for (int j = 0; j < soTrang; j++) {
        int trangMoi = chuoiTrang[j];
        int viTriTrang = timViTriTrang(khungHienTai, soKhungTrang, trangMoi);

        if (viTriTrang != -1) {
            // Trung trang: cap nhat buoc su dung gan nhat cua trang do.
            lanTruyCapGanNhat[viTriTrang] = j;
            mangLoiTrang[j] = false;
            viTriBiThay[j] = -1;
        }
        else {
            int viTriThay = timKhungTrong(khungHienTai, soKhungTrang);

            // Neu khong con khung trong, chon trang dung lau nhat.
            if (viTriThay == -1) {
                viTriThay = timKhungLRU(lanTruyCapGanNhat, soKhungTrang);
            }

            khungHienTai[viTriThay] = trangMoi;
            lanTruyCapGanNhat[viTriThay] = j;
            mangLoiTrang[j] = true;
            viTriBiThay[j] = viTriThay;
            tongLoiTrang++;
        }

        luuTrangThaiKhung(khungHienTai, soKhungTrang, j, bangKetQua);
    }
}

// MRU - Most Recently Used.
// Khi khung day, thay trang vua duoc truy cap gan nhat.
void chayMRU(int chuoiTrang[], int soTrang, int soKhungTrang,
    int bangKetQua[TOI_DA_KHUNG][TOI_DA_TRANG], bool mangLoiTrang[],
    int viTriBiThay[TOI_DA_TRANG], int& tongLoiTrang) {
    int khungHienTai[TOI_DA_KHUNG];
    int lanTruyCapGanNhat[TOI_DA_KHUNG];
    tongLoiTrang = 0;

    for (int i = 0; i < soKhungTrang; i++) {
        khungHienTai[i] = -1;
        lanTruyCapGanNhat[i] = -1;
    }

    for (int j = 0; j < soTrang; j++) {
        int trangMoi = chuoiTrang[j];
        int viTriTrang = timViTriTrang(khungHienTai, soKhungTrang, trangMoi);

        if (viTriTrang != -1) {
            // Trung trang: cap nhat buoc su dung gan nhat cua trang do.
            lanTruyCapGanNhat[viTriTrang] = j;
            mangLoiTrang[j] = false;
            viTriBiThay[j] = -1;
        }
        else {
            int viTriThay = timKhungTrong(khungHienTai, soKhungTrang);

            // Neu khong con khung trong, chon trang vua dung gan nhat.
            if (viTriThay == -1) {
                viTriThay = timKhungMRU(lanTruyCapGanNhat, soKhungTrang);
            }

            khungHienTai[viTriThay] = trangMoi;
            lanTruyCapGanNhat[viTriThay] = j;
            mangLoiTrang[j] = true;
            viTriBiThay[j] = viTriThay;
            tongLoiTrang++;
        }

        luuTrangThaiKhung(khungHienTai, soKhungTrang, j, bangKetQua);
    }
}
