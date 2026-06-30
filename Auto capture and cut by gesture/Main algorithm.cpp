#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")


#include <bits/stdc++.h>
#include <filesystem>
#include <chrono>   // Thư viện để đo thời gian (dùng cho sleep)
#include <thread>   // Thư viện để xử lý luồng (dùng cho sleep)

using namespace std;

/*
cấm sửa endl thành "\n", hay đi thi nên cần kết quả 1 lần, đây cân phải push kết quả luôn
*/

bool file_da_san_sang(const string& ten_file) 
{
    ifstream f(ten_file, ios::binary | ios::app);
        return f.is_open(); // Nếu mở được trả về true, ngược lại false
    //cái này để tránh việc video bị crash
}



int main()
{

    //thư mục đang rình
    string s;
    string path = "";

    cout<<"Hay nhap duong dan vao CAMERA FOLDER: "<<endl;
    getline(cin, s);
    cout<<"Duong dan vao CAMERA FOLDER la: "<<s<<endl;

    //Hoàn tất việc lấy đường dẫn vào camera
    path.reserve(10000);
    for (long long i=0;i<s.size();i++)
    {
        path+=s[i];
        if (s[i] == '\\')
        {
            path+='\\';
        }
    }
    cout<<endl;

    //Hoàn tất việc lấy đường dẫn vào folder video được chỉ định
    cout<<"Hay nhap duong dan vao FOLDER CHO VIDEO VAO TRONG: "<<endl;
    getline(cin, s);
    cout<<"Duong dan vao FOLDER CHO VIDEO VAO TRONG la: "<<s<<endl;

    string folder_dich = "";
    folder_dich.reserve(10000);

    for (long long i=0;i<s.size();i++)
    {
        folder_dich+=s[i];
        if (s[i] == '\\')
        {
            folder_dich+='\\';
        }
    }
    cout<<endl;

    long long so_ngay_tu_dong_xoa;
    cout<<"Hay nhap so ngay TU DONG XOA VIDEO: "<<endl;

    cin>>so_ngay_tu_dong_xoa;
    cin.ignore();//cái này bỏ đi cũng được, tùy nhưng nếu nay mai update code thì coi như sự chuẩn bị cho nhập thêm

    cout<<"=== DANG THUC HIEN VIEC KIEM TRA FILE CU VA XOA FILE CU TRONG TAM: "<<so_ngay_tu_dong_xoa<<"ngay ==="<<endl;


    auto t_hien_tai = chrono::system_clock::now();
    long long giay_hien_tai = chrono::duration_cast<chrono::seconds>(t_hien_tai.time_since_epoch()).count();

    vector<filesystem::path> nhung_file_can_xoa;
    nhung_file_can_xoa.reserve(10000);

    for (auto const&file : filesystem::directory_iterator(folder_dich)) //đi từng file trong vector, cụ thể là kiểm tra xem xóa file nào cần xóa
    {
        auto t_file = filesystem::last_write_time(file);
        long long giay_file = chrono::duration_cast<chrono::seconds>(t_file.time_since_epoch()).count();

        long long hieu = giay_hien_tai - giay_file;
        long long hang_so_quy_doi = 24*60*60;
        if (hieu >= so_ngay_tu_dong_xoa*hang_so_quy_doi)//quy đổi ra giây
        {
            //lệnh cho file cần remove vào
            nhung_file_can_xoa.push_back(file.path());
        }
    }

    //xóa những file được chỉ định, đã có path rồi
    for (auto const&file : nhung_file_can_xoa)
    {
        filesystem::remove(file);//không sử dụng file.path() nữa vì bản chất truyền vào vector đã là path rồi
    }

    /*
    LƯU Ý: Phần xóa trên kia bắt buộc phải dùng độ phức tạp 2n thay vì n vì 1 lí do đơn giản:
    SỰ AN TOÀN CỦA VIỆC XÓA FILE
    -> CẤM SỬA
    nếu như mà về n thì cái file nó sẽ bị dịch chuyển ngay khi bị xóa trong vòng lặp
    -> hên xui trong việc xóa file, có thể bị sót file
    */






    // 1. Lấy mốc thời gian cũ làm chuẩn
    auto thoi_gian_cu = filesystem::last_write_time(path);

    cout << "=== HE THONG DANG RINH... ===" << endl;

    string ma_don_hang="nothing";

    string old_state = " ";

    string old_image = " ";

    // 2. Vòng lặp rình rập vĩnh cửu

    bool found=false;

    bool in_khong_tim_duoc = false;

    while (true)
    {
        // Cho máy nghỉ 350ms để không ngốn CPU + đợi file load
        this_thread::sleep_for(chrono::milliseconds(350));

        // 3. Lấy thời gian mới nhất của thư mục
        auto thoi_gian_moi = filesystem::last_write_time(path);

        // 4. So sánh, cái này để tối ưu trước không vào vòng lặp là lại tốn công
        if (thoi_gian_moi > thoi_gian_cu)
        {
            cout << "[!] BIEN DONG! Co thay doi trong folder" << endl;

            //từ từ để win update lại thời gian cho nó chính xác
            this_thread::sleep_for(chrono::milliseconds(150));

            auto thoi_gian_max = filesystem::file_time_type::min();

            // Xử lý logic bốc file ra vector ở đây...
            //lấy thời gian mới nhất
            for (auto const&file : filesystem::directory_iterator(path)) //đi từng file trong vector
            {
                thoi_gian_max=max(thoi_gian_max, filesystem::last_write_time(file.path()));
            }

            string tenfile,loaifile;

            for (auto const&file : filesystem::directory_iterator(path)) //đi từng file trong vector
            {
                auto extension_of_file = file.path().extension();
                auto thoi_gian_cua_file = filesystem::last_write_time(file.path());
                if ((extension_of_file == ".jpg" || extension_of_file == ".png" || extension_of_file == ".mp4")) //check if this is an image or video and also check for the newest one
                {
                    //ngăn lọc 2: sự mới nhất, cái này chỉ lấy thằng mới nhất trong folder (ko có nghĩa nó là file thêm vào)
                    if (thoi_gian_cua_file == thoi_gian_max)
                    {
                        found=true;
                        tenfile=file.path().string();

                        if (extension_of_file == ".mp4")
                        {
                            loaifile = "video";
                        }
                        else
                        {
                            loaifile = "image";
                        }
                        cout<<tenfile<<" ("<<loaifile<<")"<<endl<<endl;
                        break;

                    }
                }
            }

            // 5. Cập nhật lại mốc cũ bằng mốc mới để chờ lượt tiếp theo
            thoi_gian_cu = thoi_gian_moi;
            if (found == false)
            {
                cout<<"Khong co file nao, bo me em chup lai anh ma don di lol"<<endl<<endl;
                continue; //ngắt vòng lặp
            }

            /*
            liên tục kiểm tra xem là file có mở được không
            nếu không mở được thì phải từ từ

            LÍ DO CHÍNH: Kiểm tra xem video đã được quay xong chưa hay là ảnh đã mở được để lấy mã đơn chưa
            */
            cout<<"Dang thu xem file co the ton tai khong........"<<endl;
            while (true)
            {
                this_thread::sleep_for(chrono::milliseconds(350));
                if (file_da_san_sang(tenfile) == true)
                {
                    break;
                }
            }


            cout<<"Hoan tat viec lay file !!!!!!!!!... chuan bi sang buoc"<<endl;



            //xử lí hình ảnh (lấy mã đơn)
            if (loaifile == "image" && old_image != tenfile) //vừa đảm bảo là ảnh và vừa đảm bảo là ko bị trùng ảnh (khi dịch chuyển video)
            {
                if (old_state == " " || old_state == "video")
                {
                    cout<<" lay ma don hang...";

                    // 1. Tạo chuỗi lệnh gọi zbarimg.exe truyền biến tuyệt đối 'tenfile' vào
                    // Dùng --raw để lấy chuỗi sạch, bọc 'tenfile' trong dấu \" để cân hết mọi khoảng trắng đường dẫn
                    string lenh_zbar = "zbarimg.exe --raw \"" + tenfile + "\" > qr_res.txt 2>nul";

                    // 2. Kích hoạt thằng sai vặt ZBar quét ngầm
                    system(lenh_zbar.c_str());

                    // 3. Mở file tạm đọc kết quả vào biến
                    ifstream file_doc("qr_res.txt");
                    if (file_doc >> ma_don_hang)
                    {
                        cout << endl << "=============================================" << endl;
                        cout << "[THANH CONG] Ma don hang la: " << ma_don_hang << endl;
                        cout << "=============================================" << endl;
                    } else
                    {
                        cout << "[CANH BAO] Anh moi thoi nhung ZBar khong tim thay ma QR. Yeu cau bo me em chup lai anh!!!" << endl;
                        ma_don_hang = "nothing"; // Reset lại trạng thái
                        old_state = " ";
                        found = false;
                        continue;
                    }

                    old_image = tenfile;

                    old_state = "image";
                    file_doc.close();

                    // 4. Dọn dẹp file tạm cho sạch folder tool
                    system("del qr_res.txt >nul 2>&1");
                }
                else
                {
                    old_state = " ";//reset lại quá trình
                    cout<<"Bo me em chua QUAY VIDEO, vui long [    QUAY LAI VIDEO    ] hoac la [    DONG CHUONG TRINH va CHUP ANH -> QUAY VID LAI 1 LAN NUA    ]"<<endl;
                }

            }

            //xử lí video (đổi tên file video thành mã đơn hàng và chuyển file sang thư mục được chỉ định
            if (loaifile == "video")
            {
                if (old_state == "image")
                {
                    cout << " chuyen ten file va dich chuyen video sang thu muc..." << endl;

                    // 1. Định nghĩa thư mục đích bạn muốn chuyển video về (Nhớ tạo sẵn folder này trên máy nhé)

                    // Tự động tạo thư mục đích nếu lỡ may chưa có, đỡ sập app
                    filesystem::create_directories(folder_dich);

                    // 2. Lấy tên gốc của file video (ví dụ: "WIN_2026_xxx.mp4")
                    string ten_file_goc = filesystem::path(tenfile).filename().string();

                    // 3. Tạo đường dẫn mới hoàn chỉnh ở thư mục đích
                    // Bạn có thể đổi tên tùy ý ở đây, ví dụ gán thêm chữ "Da_Xu_Ly_" vào trước tên file
                    // 3. Tạo đường dẫn mới hoàn chỉnh: chỉ lấy mã đơn hàng + đuôi file .mp4
                    string duong_dan_moi = folder_dich + "\\" + ma_don_hang + ".mp4";


                    try
                    {
                        // 4. Thực hiện Di chuyển + Đổi tên trong 1 nốt nhạc
                        filesystem::rename(tenfile, duong_dan_moi);

                        cout << "=============================================" << endl;
                        cout << "[THANH CONG] DA DI CHUYEN VIDEO THANH CONG!" << endl;
                        cout << " -> Tu: " << tenfile << endl;
                        cout << " -> Sang: " << duong_dan_moi << endl;
                        cout << "=============================================" << endl;
                    }
                    catch (const filesystem::filesystem_error& e)
                    {
                        cout << "[LOI VIDEO] Khong the di chuyen file: " << e.what() << endl;
                        cout << "Bo me em co the DONG chuong trinh va chay lai tu dau theo huong dan";
                    }

                    old_state = "video";

                    cout<<endl;
                }
                else
                {
                    old_state = " ";//reset lại quá trình
                    cout<<"Bo me em chua CHUP ANH, bay gio hay CHUP ANH MA DON HANG"<<endl;
                    cout<<"Bo me em co the DONG chuong trinh va chay lai tu dau theo huong dan"<<endl;
                }

            }

            found = false;//reset lại quá trình lấy file

            in_khong_tim_duoc = false;

        }
        else
        {
            if (in_khong_tim_duoc == false)
            {
                cout<<"Dang doi CHUP ANH / QUAY VIDEO luot tiep theo"<<endl;
                in_khong_tim_duoc = true;
            }
        }
    }

    return 0;
}

/*
cách tạo file exe
vào thư mục chỉ định này
trên đường dẫn gõ "cmd"
có command promt rồi thì paste thằng này vào:

"C:\Program Files\CodeBlocks\MinGW\bin\g++" -std=c++20 -O3 "Get QR code convert to string.cpp" -o "Phan mem (bo me em click vao day de chay).exe" -static-libgcc -static-libstdc++ -static

yêu cầu máy có codeblocks bản mingw
*/


/*
SƠ ĐỒ CỦA VIỆC CHẠY:
[BƯỚC ĐẦU: KHỞI ĐỘNG]
      │
      ▼
Quét sạch file cũ quá X ngày ở Folder Đích (Tránh đầy ổ cứng, an toàn).
      │
      ▼
[BƯỚC RÌNH RẬP - VÒNG LẶP VĨNH CỬU] ◄────────────────────────────────────────┐
      │                                                                      │
      ├──► Không có biến động? ──► Sleep 350ms              ─────────────────┤
      │                                                                      │
      └──► CÓ BIẾN ĐỘNG! (Thư mục camera thay đổi thời gian)                 │
            │                                                                │
            ▼                                                                │
      Đợi 150ms cho Windows cập nhật, bốc ngay thằng MỚI NHẤT                │
            │                                                                │
            ▼                                                                │
      Vòng lặp True (Check File Lock): Đợi file ghi xong 100%                │
            │                                                                │
            ▼                                                                │
      [PHÂN NHÁNH TRẠNG THÁI (STATE MACHINE)]                                │
            │                                                                │
            ├──► Nếu là ẢNH (.jpg/.png):                                     │
            │      ├─ Check trùng ảnh cũ.                                    │
            │      ├─ Gọi ZBar quét ngầm QR ──► Phọt mã đơn vào file txt.    │
            │      ├─ Đọc file txt lấy mã đơn.                               │
            │      └─ Chuyển trạng thái: old_state = "image". ───────────────┤
            │                                                                │
            └──► Nếu là VIDEO (.mp4):                                        │
                   ├─ Check xem trước đó đã chụp ảnh chưa (old_state=="image")│
                   ├─ ĐÚNG: Rename + Bốc thẳng sang ổ đích bằng mã đơn.mp4   │
                   ├─ SAI: Chửi nhẹ "Bố mẹ chưa chụp ảnh" rồi reset.         │
                   └─ Chuyển trạng thái: old_state = "video". ───────────────┘

LƯU Ý: Code tự làm và có phân nhánh riêng cho từng việc trong nháp nên soạn thảo lại ra đây mất nhiều thời gian
-> Bản thiết kế dòng chạy của code ở trên kia được đưa code cho AI phác thảo nên có thể có một chút sai sót ngoài ý muốn nhưng cũng khá đúng rồi
*/