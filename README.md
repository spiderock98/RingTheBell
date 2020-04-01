# Hệ Thống Chuông Tự Động Giáo Xứ ...

## Tổ chức ô nhớ EEPr

### Sự kiện mỗi ngày: 7 cells  X  3 buổi mỗi ngày  X  7 ngày trong tuần  =  147 cells. 0 -> 146
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | ... |
| - | - | - | - | - | - | - | --- |
| WeekDay | Hour | Minute | Relay1 | Relay2 | Relay3 | Duration | *lặp lại* |

### Sự kiện tuỳ chỉnh: ô 147 *<số sự kiện tuỳ chỉnh hiện tại>*  +  10 cells  X  đến hết ô nhớ EEPr (1024 bytes). 147 | 148 -> 1023
| 148 | 149 | 150 | 151 | 152 | 153 | 154 | 155 | 156 | 157 | ... |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| MonthDate | Month | Year1 *<hàng nghìn & hàng trăm>* | Year2 *<hàng chục & hàng đơn vị>* | Hour | Minute | Relay1 | Relay2 | Relay3 | Duration | *lặp lại* |

### Linh tinh
- biến *arrTick[256]* map tạm thời toàn bộ EEPr cells để làm biến tạm set sự kiện trước khi trả về EEPr

## Người sử dụng
### Chức năng các phím nhấn
#### sự kiện hàng tuần
- ở màn hình chính >> ***phím A*** để chuyển sang màn hình duyệt sự kiện lặp lại mỗi tuần
- tiếp tục nhấn ***phím A*** để duyệt qua hàng loạt các sự kiện
- dừng ở sự kiện bất kỳ >> ***phím B*** để đặt lại thời gian, relay, ... tương ứng
#### sự kiện tuỳ chỉnh
- ở màn hình chính >> ***phím D*** để chuyển sang màn hình duyệt sự kiện tuỳ chỉnh
- tiếp tục nhấn ***phím D*** để duyệt qua hàng loạt các sự kiện
- ở màn hình chính hoặc trong khi đang duyệt sự kiện tuỳ chỉnh >> ***nhấn giữ phím D*** để thêm sự kiện tuỳ chỉnh mới
- dừng ở sự kiện bất kỳ >> ***nhấn giữ phím #*** để xoá sự kiện đó
#### Bổ sung
- nguyên tắc chọn relay: ***phím 1 hoặc phím bất kỳ khác 0*** >> bật relay, ***phím 0*** >> tắt relay

### Lưu ý khi thao tác
- đặt thời gian theo hệ 24 giờ. VD: 12h00 >> 12:00 giờ trưa, 00h01 >> 12:01 khuya, 16h30 >> 4:30 chiều
- đọc kỹ phần *Chức năng phím nhấn* phía trên. Tuyệt đối không nhấn phím ngẫu nhiên khi thao tác trên màn hình không liên quan để tránh gây lỗi chương trình. ~~VD: nhấn D ở màn hình duyệt sự kiện là vô nghĩa~~
- không nhấn phím liên tục, nhấn quá nhanh, nhấn nhiều phím cùng lúc
- không được đặt sự kiện tuỳ chỉnh trùng khớp thời gian với các sự kiện hàng ngày >> tránh xung đột
- khi treo màn hình >> nhấn nút reset hệ thống
- khi đặt sự kiện hàng ngày lưu ý chọn mốc thời gian trong ngày tương ứng với mốc thời gian trên màn hình. ~~VD: Đặt chuông buổi SÁNG lúc 16h30 CHIỀU là vô lý~~
- nên kiểm tra qua tất cả các sự kiện để chắc chắn cài đặt sự kiện thành công
- sau khi hoàn tất mọi cài đặt thời gian thì phải quay lại được màn hình chính >> nếu không quay về được có thể nhấn reset. Sau đó mới đóng nắp tủ điện