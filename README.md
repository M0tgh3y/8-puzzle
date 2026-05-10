# 8 Puzzle Heuristic Database Generator

## معرفی پروژه

این پروژه یک پیاده‌سازی کامل از **پایگاه داده هیوریستیک (Heuristic Database)** برای مسئله معروف **8-Puzzle** با استفاده از زبان ++C است.

برنامه با استفاده از الگوریتم **Breadth-First Search (BFS)** تمام حالت‌های قابل دستیابی پازل را از روی حالت هدف تولید کرده و فاصله دقیق هر حالت تا Goal State را محاسبه می‌کند.

نتایج در فایل `heuristic.txt` ذخیره می‌شوند تا بتوان از آن‌ها به‌عنوان یک هیوریستیک دقیق در الگوریتم‌های جستجو مانند:

- A*
- IDA*
- Best First Search
- Pattern Database Search

استفاده کرد.

---

# ویژگی‌های پروژه

- تولید کامل فضای حالت برای پازل 3×3
- محاسبه فاصله واقعی تا Goal State
- ذخیره اطلاعات در پایگاه داده متنی
- استفاده از BFS برای تضمین کوتاه‌ترین مسیر
- پشتیبانی از Star Tiles (`*`)
- قابلیت تست وضعیت‌های مختلف پازل
- تشخیص حالت‌های غیرقابل دستیابی
- استفاده از ساختمان داده‌های بهینه مانند:
  - `unordered_map`
  - `unordered_set`
  - `queue`

---

# ایده اصلی پروژه

در مسئله 8-Puzzle تعداد زیادی حالت مختلف وجود دارد و محاسبه فاصله تا هدف در زمان اجرا می‌تواند هزینه‌بر باشد.

در این پروژه، به‌جای محاسبه مداوم فاصله:

1. تمام حالت‌های ممکن یک‌بار تولید می‌شوند.
2. فاصله دقیق هر حالت تا هدف ذخیره می‌شود.
3. در آینده فقط با یک Lookup ساده می‌توان فاصله را دریافت کرد.

این روش پایه‌ی اصلی **Pattern Database Heuristics** در هوش مصنوعی است.

---

# نحوه عملکرد برنامه

## 1. دریافت Goal State

ابتدا کاربر وضعیت هدف را وارد می‌کند.

نمونه:

```txt
1 2 3
4 5 6
7 8 0
```

عدد `0` نشان‌دهنده خانه خالی (Blank Tile) است.

---

## 2. تعریف Star Tiles

برنامه اجازه می‌دهد بعضی Tileها به‌صورت `*` تعریف شوند.

مثال:

```txt
How many stars (*) ? 2

Enter star values:
1 2
```

در این حالت، هنگام ساخت State String اعداد 1 و 2 به شکل `*` ذخیره می‌شوند.

مثال:

```txt
*,*,3,4,5,6,7,8,0,
```

این تکنیک برای کاهش فضای حالت و ساخت Pattern Database استفاده می‌شود.

---

## 3. تولید فضای حالت با BFS

برنامه از Goal State شروع می‌کند و تمام حرکت‌های ممکن را بررسی می‌کند.

در هر مرحله:

- مکان خانه خالی پیدا می‌شود
- حرکت‌های معتبر اعمال می‌شوند
- حالت جدید ساخته می‌شود
- اگر قبلاً دیده نشده باشد:
  - ذخیره می‌شود
  - فاصله آن ثبت می‌شود
  - وارد صف BFS می‌شود

---

# الگوریتم مورد استفاده

## Breadth-First Search (BFS)

ویژگی مهم BFS:

- تضمین پیدا شدن کوتاه‌ترین مسیر
- مناسب برای تولید Exact Heuristic
- پیمایش سطح‌به‌سطح فضای حالت

پیچیدگی زمانی پروژه وابسته به تعداد حالت‌های قابل دستیابی است.

برای 8-Puzzle تعداد حالت‌های Reachable برابر است با:

```txt
181440
```

---

# ساختار State

هر State شامل موارد زیر است:

```cpp
struct State {

    vector<vector<int>> board;

    vector<int> stars;

    string stateStr;

    int dist;
};
```

توضیحات:

| فیلد | توضیح |
|---|---|
| `board` | وضعیت فعلی پازل |
| `stars` | عناصر ستاره‌ای |
| `stateStr` | نمایش متنی یکتا |
| `dist` | فاصله تا Goal |

---

# فایل Heuristic Database

تمام Stateها در فایل زیر ذخیره می‌شوند:

```txt
heuristic.txt
```

فرمت هر خط:

```txt
state_string distance
```

مثال:

```txt
1,2,3,4,5,6,7,8,0, 0
1,2,3,4,5,0,7,8,6, 1
```

---

# نحوه کامپایل و اجرا

## کامپایل

```bash
g++ main.cpp -o puzzle
```

## اجرا

```bash
./puzzle
```

---

# نمونه اجرای برنامه

```txt
====================
GOAL STATE
====================

Enter 3x3 board:
(Use 0 for empty)

1 2 3
4 5 6
7 8 0

How many stars (*) ? 1

Enter star values:
1

Generating ALL states...

1000 states generated...
2000 states generated...
3000 states generated...

DONE!

Total states = 181440

Saved in heuristic.txt
```

---

# تست یک State

کاربر می‌تواند هر وضعیت دلخواهی را وارد کند:

```txt
2 3 6
1 5 0
4 7 8
```

اگر State قابل دستیابی باشد:

```txt
Exact Distance To Goal = 12
```

اگر غیرقابل دستیابی باشد:

```txt
This state is NOT reachable from the goal state.
```

---

# ساختمان داده‌های استفاده‌شده

| ساختمان داده | کاربرد |
|---|---|
| `queue` | پیاده‌سازی BFS |
| `unordered_set` | جلوگیری از تکرار State |
| `unordered_map` | ذخیره Heuristic |
| `vector<vector<int>>` | نمایش صفحه پازل |

---

# مفاهیم هوش مصنوعی استفاده‌شده

این پروژه شامل مفاهیم مهم زیر است:

- State Space Search
- Breadth-First Search
- Exact Heuristic
- Pattern Database
- State Encoding
- Graph Traversal
- Heuristic Search

---

# توسعه‌های پیشنهادی

- پشتیبانی از 15-Puzzle
- ذخیره‌سازی باینری برای کاهش حجم فایل
- استفاده از Hash Encoding سریع‌تر
- اضافه کردن الگوریتم A*
- اضافه کردن GUI
- Parallel BFS
- Memory Optimization

---

# ساختار پروژه

```txt
.
├── main.cpp
├── heuristic.txt
└── README.md
```

---

# نتیجه‌گیری

این پروژه یک نمونه کامل از استفاده BFS برای تولید Exact Heuristic Database در مسئله 8-Puzzle است.

پروژه علاوه بر جنبه آموزشی، پایه مناسبی برای توسعه الگوریتم‌های پیشرفته جستجو و هوش مصنوعی محسوب می‌شود.

---

# نویسنده

Motahare Gheysari + my best friend chatGPT
