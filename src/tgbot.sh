#!/bin/bash

STATUS=$1  # Берём статус из аргумента, если он есть, иначе из $?

TELEGRAM_BOT_TOKEN="7801350153:AAHl3OKhu-3ZqnjJHlERn-cXmxl-V2mLgFU"
TELEGRAM_CHAT_ID="520817272"

CI_JOB_NAME=${CI_JOB_NAME:-"Неизвестная задача"}
CI_COMMIT_BRANCH=${CI_COMMIT_BRANCH:-"main"}
CI_PROJECT_NAME=${CI_PROJECT_NAME:-$(basename $(pwd))}
CI_JOB_URL=${CI_JOB_URL:-""}

if [ $STATUS -eq 0 ]; then
  EMOJI="✅ Успех"
else
  EMOJI="❌ Ошибка"
fi

MESSAGE="📌 Джоба '$CI_JOB_NAME' завершена: $EMOJI%0AПроект: $CI_PROJECT_NAME%0AВетка: $CI_COMMIT_BRANCH%0AURL: $CI_JOB_URL"

curl -s -X POST "https://api.telegram.org/bot$TELEGRAM_BOT_TOKEN/sendMessage" \
  -d chat_id="$TELEGRAM_CHAT_ID" \
  -d text="$MESSAGE" \
