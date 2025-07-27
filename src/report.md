## Part.1 Настройка gitlab-runner

### Ubuntu 22.04

- [Скачиваем](https://releases.ubuntu.com/jammy/) **Ubuntu Jammy Jellfish**
	- **Ubuntu 22.04 LTS**
	- Устанавливаем без быстрой установки 

![](files/Pasted%20image%2020250708133650.png)
- Устанавливаем `openSSH` во время установки
![](files/Pasted%20image%2020250708184915.png)
- Изменяем файл` sshd_config`
![](files/Pasted%20image%2020250708185201.png)
- *Убираем комментарии и устанавливаем* `Port 2023`
![](files/Pasted%20image%2020250708185138.png)
- *Убираем комментарии и устанавливаем*`PubkeyAuthentication no`  и `PasswordAuthentication yes`

- Это делается для того чтобы `ssh` не пытался подключиться через ssh-ключ, а подключалось через пароль
	- Подключение по ссш: `ssh user@localhost -p 2023`

## Gitlab-runner

### Установка

```
curl -L "https://packages.gitlab.com/install/repositories/runner/gitlab-runner/script.deb.sh" | sudo bash

sudo apt install gitlab-runner
```

![](files/Pasted%20image%2020250708190734.png)
- *Через скрипт настраиваем пакетный менеджер и через пакетный менеджер устанавливаем `gitlab-runner*

### Регистрация

- `gitlab-runner register`
	- Мы должны ввести:
		- URL проекта
		- Токен
		- Описание
		- Тег
			- Фильтрация раннера для выполнение определенных задач
				- Когда описываем тег в `gitlab-ci.yml` мы говорим чтобы сборка выполнялась только раннером с тегом `do06`
		- Примечание
		- Механизм выполнение задач
			- `shell`

![](files/Pasted%20image%2020250709163128.png)
- *Выполнение команды `gitlab-runner register`*
	- Если выполнить `gitlab-runner` без `sudo` то она будет работать локально и не будет добавлен в системные сервисы и она не запуститься со системой

![](files/Pasted%20image%2020250709163142.png)
- *Теги gitlab раннера*

## Part 2. Сборка

### CI for SimpleBashUtils

- **GitLab CI** - ищет файл `.gitlab-ci.yml` в корне репозитория
	- Стандартное место для **CI** файлов
	- Упрощает управление несколькими проектами

```
├── .gitlab-ci.yml
└── src/
    ├── cat/
    │   ├── Makefile
    │   └── *.c
    └── grep/
        ├── Makefile
        └── *.c
```
- *Пример структуры*

### Копирование файлов в репозиторий

- `scp -P 2023 -4 -r /home/yukitoame/dev/s21/C/C3_SimpleBashUtils.ID_353513-1 user@localhost:/home/user/`
	- `-P 2023` - порт для подключение по ssh
	- `-4` - подключение по 4 версии протокола айпи
	- `-r` - рекурсивно копирует всю директорию
![](files/Pasted%20image%2020250709144602.png)
- *Вывод команды `scp`*

- Копируем директории в репозиторий
![](files/Pasted%20image%2020250709153542.png)
- *Вывод команды `cp -r`*

### .gitlab-ci.yml

![](files/Pasted%20image%2020250709173537.png)
- *Пример `gitlab-ci.yml`*

![](files/Pasted%20image%2020250709173940.png)
![](files/Pasted%20image%2020250709174307.png)
- Запушил все файлы и получили успешный пайплайн

## Part-3.Тест кодстайла

- Для того чтобы добавить тесты, надо добавить стадию тестов в файле настройки 
- И добавляем отдельные джобы для тестов код стайла

![](files/Pasted%20image%2020250709184917.png)
- *Добавленные джобы*

![](files/Pasted%20image%2020250709185034.png)
- *Отдально добавил `.clang-format` файл из проекта `SimpleBashUtils` для проверка по `clang-format`*

![](files/Pasted%20image%2020250709185217.png)
- Так все работает, но джоба не падае  из-за варнингов

![](files/Pasted%20image%2020250709191438.png)
![](files/Pasted%20image%2020250709191207.png)
- *Добавил `clang-format -n -Werror *.c *.h` чтобы clang-format возвращал код выхода 1 если есть предупреждение*

![](files/Pasted%20image%2020250709191627.png)
- *Теперь джоба падает из-за предупреждений*

![](files/Pasted%20image%2020250709192129.png)
- *После форматирования файла, все джобы проходят*

```
stages:
  - build
  - test

build_cat:
  stage: build
  tags:
    - build
  script:
    - cd src/cat && make s21_cat
  artifacts:
    paths:
      - src/cat
    expire_in: 30 days

build_grep:
  stage: build
  tags:
    - build
  script:
    - cd src/grep && make s21_grep
  artifacts:
    paths:
      - src/grep
    expire_in: 30 days

codestyle_test_cat:
  stage: test
  tags:
    - codestyle
  script:
    - cd src/cat
    - clang-format -n -Werror *.c *.h
  artifacts:
    paths:
      - src/cat

codestyle_test_grep:
  stage: test
  tags:
    - codestyle
  script:
    - cd src/grep
    - clang-format -n -Werror *.c *.h
  artifacts:
    paths:
      - src/grep
```
- Финальный `docker-ci.yml` для 3 таска

## Part 4. Интеграционные тесты

- Тесты проекта выполняются через `make tests`
	- Тест сравнивает выводы с оригинальными утилитами
	- Еще тест включает в себя тесты на утечки памяти 

![](files/Pasted%20image%2020250711103534.png)
- `Makefile`

- Добавляем джобы для `.gitlab-ci.yml`
![](files/Pasted%20image%2020250711111115.png)
- *Добавленные джобы для `.gitlab-ci.yml`*
	- *Добавление вывода "тесты прошлии или не прошли"*
	- *"|" После пайпа можно написать целый баш скрипт в самом yml файле*

![](files/Pasted%20image%2020250711111523.png)
![](files/Pasted%20image%2020250711111600.png)
- *Все работает*

### Фиксы

- Я заметил что в пайплайне, а именно в стадии тестов джобы проходят не в том очередности которые я добавлял
	- Возможно стоит добавить стадию `codestyle`

![](files/Pasted%20image%2020250711112419.png)
- *Добавил стадию кодстайла*

![](files/Pasted%20image%2020250711112444.png)
- *Изменил стадию в джобах*

![](files/Pasted%20image%2020250711112841.png)
- *Джобы после добавление стадии кодстайла*

- Еще я узнал что есть ключ `needs`, где можно указывать зависимость джобы, можно добавить джобы кодстайла как зависимость для стадии тестов

![](files/Pasted%20image%2020250711113305.png)
- *Добавление `needs` в `.gitlab-ci.yml`*

![](files/Pasted%20image%2020250711113628.png)
![](files/Pasted%20image%2020250711113751.png)
- *Пайплайн прошел после добавление `needs` в джобу интеграционных тестов*

## Part 5. Этап деплоя

### Ubuntu clone

- Делаю клон виртуальной машины, чтобы получить вторую машину для деплоя 
	- Вот тут стоить быть осторожным, потому что `gitlab-runner` может начать работать в заклоненной машине, так что есть вариант установить чистую машину

![](files/Pasted%20image%2020250712123303.png)
- *Клон Ubuntu 22.04 LTS*

![](files/Pasted%20image%2020250712123627.png)
- *Маппинг портов для второй машины*

![](files/Pasted%20image%2020250712124006.png)
![](files/Pasted%20image%2020250712124125.png)
- *Открываем порт `2024` и перезапускаем ssh демона*

![](files/Pasted%20image%2020250712124218.png)
- *Подключаемся к машине деплоя*

- Для удобство поменяю название машины на `ubuntujammy-deploy`
- `sudo hostnamectl set-hostname ubuntujammy-deploy`
- `sudo vim /etc/hosts`
	- Поменять старый хостнэйм на новый 
	- Перезапустить систему

#### Настройка сети для двух виртуальных машин

![](files/Pasted%20image%2020250712125636.png)
![](files/Pasted%20image%2020250712125712.png)
- *Создание второго сетевого интерфейса типа "внутрення сеть" на виртуальных машинах*

![](files/Pasted%20image%2020250712130107.png)
![](files/Pasted%20image%2020250712130119.png)
- *Вывод команды `ip a` смотрим что сетевой интерфейс был включен, следовательно будем настраивать внутренную сеть используя интерфейс `enp0s8`*

![](files/Pasted%20image%2020250712130925.png)
![](files/Pasted%20image%2020250712131602.png)
- *Первая машина успешно пингует вторую*
![](files/Pasted%20image%2020250712130948.png)
![](files/Pasted%20image%2020250712131646.png)
- *Вторая машина успешно пингует первую машину*

![](files/Pasted%20image%2020250712131746.png)
- *Первая машина успешно заходить через `ssh`*

### Ручной деплой CD (Continuous Deployment)

- Так получилось что у меня гитлаб раннер начал работать в клонированной машине, а именно в `ubuntujammy-deploy`

![](files/Pasted%20image%2020250713155126.png)
- *Переходим на юзера `gitlab-runner` и создаю ссш-ключ*

![](files/Pasted%20image%2020250713155931.png)
- *Копирую ключ в другую машину для того чтобы заходить по ссш `ssh-copy-id -p 2023 user@10.10.0.1`*

![](files/Pasted%20image%2020250713160221.png)
- *Настраиваем `ssh` для входа по ключу*

![](files/Pasted%20image%2020250713161329.png)
- *Перезапускаем `sudo systemctl restart ssh`*

![](files/Pasted%20image%2020250713160335.png)
- *Даем права юзеру `sudo chmod -R 777 /usr/local/bin`, для того чтобы во время копирования через `scp` юзер мог копировать в эту директорию* 


![](files/Pasted%20image%2020250712191521.png)
-  *Добавляем стадию деплоя и джобу для `.gitlab-ci.yml`*
	- В `needs` надо указывать ту джобу которая собирает утилиту, и оттуда брать артефакты
	- `when: manual` - стадию надо запускать вручную если все стадии прошли

- Создаем скрипт для копироваания исполняемых файлов, используя `scp`
```bash
#/bin/bash

eval "$(ssh-agent)"
ssh-add ~/.ssh/id_rsa

# Параметры подключения к удалённой машине
REMOTE_USER="user"
REMOTE_HOST="10.10.0.1"
REMOTE_DIR="/usr/local/bin"

# Локальные пути к бинарникам
LOCAL_CAT_BIN="cat/s21_cat"
LOCAL_GREP_BIN="grep/s21_grep"

# Проверяем, что бинарники существуют
if [ ! -f "$LOCAL_CAT_BIN" ]; then
  echo "Файл $LOCAL_CAT_BIN не найден."
  exit 1
fi

if [ ! -f "$LOCAL_GREP_BIN" ]; then
  echo "Файл $LOCAL_GREP_BIN не найден."
  exit 1
fi

echo "SSH Identity:"
ssh-add -l

# Копируем бинарники на удалённую машину
echo "Копируем s21_cat..."
scp -P 2023 "$LOCAL_CAT_BIN" "$REMOTE_USER@$REMOTE_HOST:$REMOTE_DIR/"
if [ $? -ne 0 ]; then
  echo "Ошибка при копировании s21_cat."
  exit 1
fi

echo "Копируем s21_grep..."
scp -P 2023 "$LOCAL_GREP_BIN" "$REMOTE_USER@$REMOTE_HOST:$REMOTE_DIR/"
if [ $? -ne 0 ]; then
  echo "Ошибка при копировании s21_grep."
  exit 1
fi

# Делаем их исполняемыми
echo "Выполняем chmod +x на удалённой машине..."
ssh -p 2023 "$REMOTE_USER@$REMOTE_HOST" "chmod +x $REMOTE_DIR/s21_cat $REMOTE_DIR/s21_grep"
if [ $? -ne 0 ]; then
  echo "Ошибка при изменении прав доступа."
  exit 1
fi

# Проверяем, всё ли успешно
if [ $? -eq 0 ]; then
  echo "✅ Файлы успешно скопированы и подготовлены."
else
  echo "❌ Ошибка при выполнении SSH-команд."
  exit 1
fi
```

![](files/Pasted%20image%2020250713161429.png)
![](files/Pasted%20image%2020250713161452.png)
- *Деплой успешно прошел*

![](files/Pasted%20image%2020250713161848.png)
- *Утилита доступна в машине*

## Part 6. Уведомление на ТГ

![](files/Pasted%20image%2020250713164114.png)
- *Сделанный в `BotFather` бот*

- Пишем боту и через `curl -s "https://api.telegram.org/bot <TOKEN>/getUpdates"` получаем `id` чата
	- Вставляем его в наш скрипт 

![](files/Pasted%20image%2020250714124239.png)
- *Сделанный скрипт*

![](files/Pasted%20image%2020250714124335.png)
- *Меняем файл `.gitlab-ci.yml` для того чтобы уведомление работали*
	- Мой вариант работает с флагами


![](files/Pasted%20image%2020250713181642.png)
- *Намеренная ошибка для теста*

![](files/Pasted%20image%2020250714124450.png)
![](files/Pasted%20image%2020250714124515.png)
- *Полученные уведомления на телеграм*
	- Этап CI работает

![](files/Pasted%20image%2020250714125032.png)
- *Этап деплоя*
![](files/Pasted%20image%2020250714125100.png)
- Так же этап CD прошел успешно 

#### Дампы

- Дампы сохранены локально через `export` с расширением `.ova`
