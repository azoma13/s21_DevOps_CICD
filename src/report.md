## Part 1. Настройка gitlab-runner

- Подними виртуальную машину Ubuntu Server 22.04 LTS.

![alt text](png/image.png)

- Скачай и установи на виртуальную машину gitlab-runner.

![alt text](png/image-1.png)

![alt text](png/image-2.png)

- Запусти gitlab-runner и зарегистрируй его для использования в текущем проекте (DO6_CICD).

![alt text](png/image-3.png)

## Part 2. Сборка

- В файле gitlab-ci.yml добавь этап запуска сборки через мейк файл из проекта C2. Файлы, полученные после сборки (артефакты), сохрани в произвольную директорию со сроком хранения 30 дней.

![alt text](png/image-4.png)

![alt text](png/image-5.png)

## Part 3. Тест кодстайла

- Напиши этап для CI, который запускает скрипт кодстайла (clang-format). Если кодстайл не прошел, то «зафейли» пайплайн. 

![alt text](png/image-6.png)

В пайплайне отобрази вывод утилиты clang-format.

![alt text](png/image-7.png)

## Part 4. Интеграционные тесты

- Напиши этап для CI, который запускает твои интеграционные тесты из того же проекта. Запусти этот этап автоматически только при условии, если сборка и тест кодстайла прошли успешно.

![alt text](png/image-9.png)

- Если тесты не прошли, то «зафейли» пайплайн.

![alt text](png/image-8.png)

- В пайплайне отобрази вывод, что интеграционные тесты успешно прошли / провалились.

![alt text](png/image-10.png)

## Part 5. Этап деплоя

- Подними вторую виртуальную машину Ubuntu Server 22.04 LTS.

![alt text](png/image-12.png)

- Напиши этап для CD, который «разворачивает» проект на другой виртуальной машине. Запусти этот этап вручную при условии, что все предыдущие этапы прошли успешно. В файле gitlab-ci.yml добавь этап запуска написанного скрипта.

![alt text](png/image-13.png)

- Напиши bash-скрипт, который при помощи ssh и scp копирует файлы, полученные после сборки (артефакты), в директорию /usr/local/bin второй виртуальной машины.

![alt text](png/image-14.png)

- Настраиваем мост с между нашим сервером и сервером для деплоя, проверяем подключение по ssh:

![alt text](png/image-11.png)

- В результате ты должен получить готовые к работе приложения из проекта C2_SimpleBashUtils (s21_cat и s21_grep) на второй виртуальной машине.

![alt text](png/image-15.png)

![alt text](png/image-16.png)

- Сохрани дампы образов виртуальных машин.

![alt text](png/image-17.png)

## Part 6. Дополнительно. Уведомления

Настрой уведомления о успешном/неуспешном выполнении пайплайна через бота с именем «[твой nickname] DO6 CI/CD» в Telegram. Создаем бота через BotFather 

![alt text](png/image-18.png)

- Дополняем скрипт из файла notifications.md своим токеном бота и id аккаунта.

![alt text](png/image-19.png)

- В .gitlab-ci.yml файле, после каждого job добавляем проверку на статус выполнения 

![alt text](png/image-20.png)

Текст уведомления должен содержать информацию об успешности прохождения как этапа CI, так и этапа CD.
В остальном текст уведомления может быть произвольным.

![alt text](png/image-21.png)