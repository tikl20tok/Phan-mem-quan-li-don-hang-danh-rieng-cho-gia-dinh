# Phan-mem-quan-li-don-hang-danh-rieng-cho-gia-dinh
Cơ bản là tự động hóa quá trình gán mã đơn vào trong file video và chuyển file video vào thư mục được chỉ định. Sử dụng ứng camera bên ngoài (liên kết qua đường dẫn mà những cái ảnh và video được gửi tới đó)


Thực hiện thao tác sau:
B1: NHẬP ĐƯỜNG DẪN CỦA:
 - Ảnh / Video được cho camera app cho vào
 - Địa chỉ muốn cho video vào

B2: Để treo ứng dụng ở đó, tất cả các thông tin cần thiết về đơn hàng ứng dụng đều hiển thị lên

B3: Thao tác (LẶP TUẦN TỰ):
    .1: Nhấn chụp ảnh trên ứng dụng camera
    .2: Nhấn quay video trên ứng dụng camera

LƯU Ý: NẾU NHƯ MÀ CÓ GIÁN ĐOẠN XẢY RA SẼ CÓ CẢNH BÁO, CHỈ CẦN THỰC HIỆN:
 1. ĐÓNG CHƯƠNG TRÌNH -> THỰC HIỆN LẠI TỪ BƯỚC 1 (KHUYÊN NÊN LÀM CÁI NÀY)
 HOẶC:
 2. THỰC HIỆN LẠI THAO TÁC ĐANG DANG DỞ


***Muốn tạo file exe thì tải full bộ này về
Trên đường dẫn vào folder "Get QR..." thay vào và nhấn cmd
-> trong terminal copy paste dòng này vào:
"C:\Program Files\CodeBlocks\MinGW\bin\g++" -std=c++20 -O3 "Get QR code convert to string.cpp" -o "Phan mem (bo me em click vao day de chay).exe" -static-libgcc -static-libstdc++ -static

Có thể thay đổi tên tùy ý

Yêu cầu phải có bộ biên dịch, khuyên dùng codeblocks set C++20 và tải bản >=25.03 kèm bộ biên dịch