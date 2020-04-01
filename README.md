# Một số lưu ý

## Tổ chức ô nhớ EEPr

### Sự kiện mỗi ngày: 7 cells  X  3 buổi mỗi ngày  X  7 ngày trong tuần  =  147 cells. 0 -> 146
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | ... |
| - | - | - | - | - | - | - | --- |
| WeekDay | Hour | Minute | Relay1 | Relay2 | Relay3 | Duration | *lặp lại* |

### Sự kiện tuỳ chỉnh: ô 147 *<số sự kiện tuỳ chỉnh hiện tại>*  +  10 cells  X  đến hết ô nhớ EEPr (1024 bytes). 147 | 148 -> 1023
| 148 | 149 | 150 | 151 | 152 | 153 | 154 | 155 | 156 | 157 | ... |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| MonthDate | Month | Year1 *<hàng nghìn & hàng trăm>* | Year2 *<hàng chục & hàng đơn vị>* | Hour | Minute | Relay1 | Relay2 | Relay3 | Duration | *lặp lại* |
