#include <iostream>
#include "Pipe.h"
#include "CompressorStation.h"
#include "Utils.h"
#include "Logger.h"

using namespace std;

vector<Pipe> pipes;
vector<CompressorStation> compressorStations;

void showMenu() {
    cout << "Меню:" << endl;
    cout << "1. Добавить трубу" << endl;
    cout << "2. Добавить КС" << endl;
    cout << "3. Просмотреть все объекты" << endl;
    cout << "4. Редактировать трубу" << endl;
    cout << "5. Редактировать КС" << endl;
    cout << "6. Сохранить" << endl;
    cout << "7. Загрузить" << endl;
    cout << "8. Поиск труб" << endl;
    cout << "9. Поиск КС" << endl;
    cout << "10. Пакетное редактирование труб" << endl;
    cout << "0. Выход" << endl;
    cout << "Введите ваш выбор: ";
}

void handleChoice() {
    int choice = safeIntInput("");
    switch (choice) {
    case 1: {
        Pipe pipe;
        pipe.read();
        pipes.push_back(pipe);
        Logger::log("Добавлена труба: " + pipe.getName());
        cout << "Труба добавлена!" << endl;
        break;
    }
    case 2: {
        CompressorStation cs;
        cs.read();
        compressorStations.push_back(cs);
        Logger::log("Добавлена КС: " + cs.getName());
        cout << "КС добавлена!" << endl;
        break;
    }
    case 3:
        cout << "---- Трубы ----" << endl;
        for (const auto& pipe : pipes) {
            pipe.print();
        }
        cout << "---- КС ----" << endl;
        for (const auto& cs : compressorStations) {
            cs.print();
        }
        break;
    case 4: {
        int id = safeIntInput("Введите ID трубы для редактирования: ");
        if (id >= 0 && id < pipes.size()) {
            pipes[id].setRepair(safeIntInput("В ремонте? (1 - да, 0 - нет): ") == 1);
            Logger::log("Труба с ID " + to_string(id) + " отредактирована");
            cout << "Труба обновлена!" << endl;
        }
        else {
            cout << "Неправильный ID!" << endl;
        }
        break;
    }
    case 5: {
        int id = safeIntInput("Введите ID КС для редактирования: ");
        if (id >= 0 && id < compressorStations.size()) {
            int action = safeIntInput("1. Запустить цех\n2. Остановить цех\nВведите действие: ");
            if (action == 1) {
                compressorStations[id].startShop();
                Logger::log("Цех запущен на КС с ID " + to_string(id));
            }
            else if (action == 2) {
                compressorStations[id].stopShop();
                Logger::log("Цех остановлен на КС с ID " + to_string(id));
            }
            else {
                cout << "Неправильный выбор!" << endl;
            }
        }
        else {
            cout << "Неправильный ID!" << endl;
        }
        break;
    }
    case 6: {
        string filename = safeStringInput("Введите имя файла для сохранения: ");
        saveData(filename, pipes, compressorStations);
        Logger::log("Данные сохранены в файл " + filename);
        break;
    }
    case 7: {
        string filename = safeStringInput("Введите имя файла для загрузки: ");
        loadData(filename, pipes, compressorStations);
        Logger::log("Данные загружены из файла " + filename);
        break;
    }
    case 8: {
        int filter = safeIntInput("1. По названию\n2. По признаку 'в ремонте'\nВведите фильтр: ");
        if (filter == 1) {
            string name = safeStringInput("Введите название трубы: ");
            for (const auto& pipe : pipes) {
                if (pipe.getName() == name) {
                    pipe.print();
                }
            }
        }
        else if (filter == 2) {
            bool inRepair = safeIntInput("В ремонте? (1 - да, 0 - нет): ") == 1;
            for (const auto& pipe : pipes) {
                if (pipe.isInRepair() == inRepair) {
                    pipe.print();
                }
            }
        }
        else {
            cout << "Неправильный выбор!" << endl;
        }
        break;
    }
    case 9: {
        int filter = safeIntInput("1. По названию\n2. По проценту незадействованных цехов\nВведите фильтр: ");
        if (filter == 1) {
            string name = safeStringInput("Введите название КС: ");
            for (const auto& cs : compressorStations) {
                if (cs.getName() == name) {
                    cs.print();
                }
            }
        }
        else if (filter == 2) {
            double percent = safeDoubleInput("Введите процент незадействованных цехов: ");
            for (const auto& cs : compressorStations) {
                if (cs.getUnusedShopPercent() >= percent) {
                    cs.print();
                }
            }
        }
        else {
            cout << "Неправильный выбор!" << endl;
        }
        break;
    }
    case 10: {
        int filter = safeIntInput("1. По названию\n2. По признаку 'в ремонте'\nВведите фильтр: ");
        vector<int> selectedPipes;
        if (filter == 1) {
            string name = safeStringInput("Введите название трубы: ");
            for (int i = 0; i < pipes.size(); ++i) {
                if (pipes[i].getName() == name) {
                    selectedPipes.push_back(i);
                }
            }
        }
        else if (filter == 2) {
            bool inRepair = safeIntInput("В ремонте? (1 - да, 0 - нет): ") == 1;
            for (int i = 0; i < pipes.size(); ++i) {
                if (pipes[i].isInRepair() == inRepair) {
                    selectedPipes.push_back(i);
                }
            }
        }
        else {
            cout << "Неправильный выбор!" << endl;
            break;
        }

        if (selectedPipes.empty()) {
            cout << "Трубы не найдены!" << endl;
            break;
        }

        int action = safeIntInput("1. Редактировать\n2. Удалить\nВведите действие: ");
        if (action == 1) {
            for (int id : selectedPipes) {
                pipes[id].setRepair(safeIntInput("В ремонте? (1 - да, 0 - нет): ") == 1);
                Logger::log("Труба с ID " + to_string(id) + " отредактирована");
            }
        }
        else if (action == 2) {
            for (int id : selectedPipes) {
                Logger::log("Труба с ID " + to_string(id) + " удалена");
                pipes.erase(pipes.begin() + id);
            }
        }
        else {
            cout << "Неправильный выбор!" << endl;
        }
        break;
    }
    case 0:
        cout << "Выход из программы..." << endl;
        exit(0);
    default:
        cout << "Неправильный выбор!" << endl;
        break;
    }
}

int main() {
    while (true) {
        showMenu();
        handleChoice();
    }
    return 0;
}