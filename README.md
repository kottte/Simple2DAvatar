# Simple2DAvatar
![Platforms](https://img.shields.io/badge/Platforms-Windows%20%7C%20MacOS%20%7C%20Linux-informational)
![License](https://img.shields.io/badge/License-MIT-blue.svg)

**EN [English](#english)&nbsp;&nbsp;|&nbsp;&nbsp;RU [Русский](#русский)**
## English
Simple lightweight desktop app for creating PNG streaming avatars

---
### Installation

Download the latest version from [Releases](https://github.com/kottte/Simple2DAvatar/releases)

---

### Folder structure

```
Simple2Davatar
├── Simple2DAvatar(.exe/.app)
├── app.conf
└── sprites/
    ├── image.png
    └── image2.png
```
---
### Launch

Launch `Simple2DAvatar(.exe/.app)`

When you launch it for the first time on macOS, it will request access to the microphone.

---
### Usage
The avatar is created using 2 frames. The program captures audio from the microphone and calculates the volume level. A volume threshold can be set in the configuration. When this threshold is exceeded, the first frame is replaced by the second.
The frames must be in PNG format.
Place the images in the same folder as `Simple2DAvatar(.exe/.app)` and specify the path to them in `config.txt`. (see [Configuration](#configuration))

---
### Adding to OBS Studio
You can add an avatar in OBS Studio using a window capture source. You can remove the window background and title using filters: **Chroma Key** and **Crop/Pad**.

---
### Configuration
Configuration is handled in the `config.txt` file, which uses the INI format and is located next to the executable file.
If the program is already running, you must restart it for any changes to take effect. 

#### Example `config.txt`
```txt
[Window]
Width=800
Height=600

[Image]
Image1=sprites/image.png
Image2=sprites/image2.png
OffsetX=0
OffsetY=0
Scale=1.0

[Audio]
Threshold=-30
```

#### Parameters

| Key | Type | Description |
|---|---|---|
| `Width` | int | Width of the window's client area (px) |
| `Height` | int | Height of the window's client area (px) |
| `Image1` | string | Path to the first image (for silence) |
| `Image2` | string | Path to the second image (for loud sound) |
| `OffsetX` | int | X-axis offset from the center (px); can move the image beyond the window edge |
| `OffsetY` | int | Y-axis offset from the center (px) |
| `Scale` | float | Size multiplier (`1.0` = fit to window) |
| `Threshold` | float | Volume threshold in dBFS (a negative number) |

Image paths are specified relative to the folder containing `Simple2DAvatar(.exe/.app)`. In the example `config.txt`, the images are placed in a `sprites` folder for convenience.

#### `Threshold` Reference Points

| Level | Approximate dBFS |
|---|---|
| Silence / room noise | −60 … −50 |
| Quiet speech | −40 … −30 |
| Normal speech | −30 … −20 |
| Loud speech / shouting | −20 … −10 |

The hysteresis is fixed at **3 dB**. The visual element turns on when `level > Threshold` and turns off when `level < Threshold − 3`. This prevents flickering if the volume fluctuates around the threshold.

---
### Dependencies
| Library | License | purpose |
|---|---|---|
| wxWidgets | wxWindows Library Licence (LGPL + exception) | GUI, window, timers |
| miniaudio | Public Domain / MIT-0 | Audio capture |

Both libraries are pulled in automatically: `wxWidgets` via `FetchContent`, and `miniaudio` as a vendored dependency in `external/`.

---
### License
[LICENSE](LICENSE)


## Русский

Простое легковесное приложение для создания 2Д аватаров для стримов

---

### Установка
Скачайте последнюю версию со страницы [Releases](https://github.com/kottte/Simple2DAvatar/releases)

---
### Структура папок
```
Simple2Davatar
├── Simple2DAvatar(.exe/.app)
├── app.conf
└── sprites/
    ├── image.png
    └── image2.png
```

---
### Запуск

Запустить `Simple2DAvatar(.exe/.app)`

При первом запуске на macOS, приложение запросит доступ к микрофону.


---
### Использование
Аватар создаётся на основе 2 кадров. Программа захватывает звук с микровона и рассчитывает громкость. В настройках задается порог громкости, при превышении которого 1 кадр заменяется на другой.
В качестве кадров используются изображения в формате PNG.
Поместите изображения в папку с `Simple2DAvatar(.exe/.app)` и укажите путь к ним в `config.txt`. (см. [Настройка](#настройка))

---
### Добавление в OBS Studio
Добавить аватар в OBS Studio можно при помощи источника захват окна.
Убрать фон и заголовок окна можно при помощи фильтров: **Хромакей** и **Кадрирование**.

---

### Настройка
Настройка происходит в файле `config.txt` в формате INI. Расположен рядом с исполняемым файлом.
Для применения изменений необходимо перезапустить программу если она была запущена.

#### Пример `config.txt`
```txt
[Window]
Width=800
Height=600

[Image]
Image1=sprites/image.png
Image2=sprites/image2.png
OffsetX=0
OffsetY=0
Scale=1.0

[Audio]
Threshold=-30
```

#### Параметры

| Ключ | Тип | Описание |
|---|---|---|
| `Width` | int | Ширина клиентской области окна (px) |
| `Height` | int | Высота клиентской области окна (px) |
| `Image1` | string | Путь к первому изображению (тишина) |
| `Image2` | string | Путь ко второму изображению (громко) |
| `OffsetX` | int | Смещение по X от центра (px), может уводить за край |
| `OffsetY` | int | Смещение по Y от центра (px) |
| `Scale` | float | Множитель размера (`1.0` = вписать в окно) |
| `Threshold` | float | Порог громкости в dBFS (отрицательное число) | 

Пути к изображениям указываются относительно папки в, которой лежит `Simple2DAvatar(.exe/.app)` В примере `config.txt` изображения вынесены в папку `sprites` для удобства.

#### Ориентиры по `Threshold`

| Уровень | Примерный dBFS |
|---|---|
| Тишина / шум комнаты | −60 … −50 |
| Тихая речь | −40 … −30 |
| Обычная речь | −30 … −20 |
| Громкая речь / крик | −20 … −10 |

Гистерезис фиксированный — **3 dB**. Изображение включается при `level > Threshold` и выключается при `level < Threshold − 3`. Это устраняет мерцание, если громкость колеблется вокруг порога.

---

### Зависимости

| Библиотека | Лицензия | Назначение |
|---|---|---|
| wxWidgets | wxWindows Library Licence (LGPL + exception) | GUI, окно, таймеры |
| miniaudio | Public Domain / MIT-0 | Захват микрофона |

Обе библиотеки подтягиваются автоматически: `wxWidgets` — через `FetchContent`, `miniaudio` — vendored в `external/`.

---

### Лицензия

См. [LICENSE](LICENSE).