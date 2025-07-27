#/bin/bash

eval "$(ssh-agent)"
ssh-add ~/.ssh/id_rsa

REMOTE_USER="user"
REMOTE_HOST="10.10.0.1"
REMOTE_DIR="/usr/local/bin"

LOCAL_CAT_BIN="cat/s21_cat"
LOCAL_GREP_BIN="grep/s21_grep"

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
