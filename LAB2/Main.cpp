#include <iostream>
#include "Pipe.h"
#include "CompressorStation.h"
#include "Utils.h"
#include "Logger.h"

using namespace std;

vector<Pipe> pipes;
vector<CompressorStation> compressorStations;

void showMenu() {
    cout << "����:" << endl;
    cout << "1. �������� �����" << endl;
    cout << "2. �������� ��" << endl;
    cout << "3. ����������� ��� �������" << endl;
    cout << "4. ������������� �����" << endl;
    cout << "5. ������������� ��" << endl;
    cout << "6. ���������" << endl;
    cout << "7. ���������" << endl;
    cout << "8. ����� ����" << endl;
    cout << "9. ����� ��" << endl;
    cout << "10. �������� �������������� ����" << endl;
    cout << "0. �����" << endl;
    cout << "������� ��� �����: ";
}

void handleChoice() {
    int choice = safeIntInput("");
    switch (choice) {
    case 1: {
        Pipe pipe;
        pipe.read();
        pipes.push_back(pipe);
        Logger::log("��������� �����: " + pipe.getName());
        cout << "����� ���������!" << endl;
        break;
    }
    case 2: {
        CompressorStation cs;
        cs.read();
        compressorStations.push_back(cs);
        Logger::log("��������� ��: " + cs.getName());
        cout << "�� ���������!" << endl;
        break;
    }
    case 3:
        cout << "---- ����� ----" << endl;
        for (const auto& pipe : pipes) {
            pipe.print();
        }
        cout << "---- �� ----" << endl;
        for (const auto& cs : compressorStations) {
            cs.print();
        }
        break;
    case 4: {
        int id = safeIntInput("������� ID ����� ��� ��������������: ");
        if (id >= 0 && id < pipes.size()) {
            pipes[id].setRepair(safeIntInput("� �������? (1 - ��, 0 - ���): ") == 1);
            Logger::log("����� � ID " + to_string(id) + " ���������������");
            cout << "����� ���������!" << endl;
        }
        else {
            cout << "������������ ID!" << endl;
        }
        break;
    }
    case 5: {
        int id = safeIntInput("������� ID �� ��� ��������������: ");
        if (id >= 0 && id < compressorStations.size()) {
            int action = safeIntInput("1. ��������� ���\n2. ���������� ���\n������� ��������: ");
            if (action == 1) {
                compressorStations[id].startShop();
                Logger::log("��� ������� �� �� � ID " + to_string(id));
            }
            else if (action == 2) {
                compressorStations[id].stopShop();
                Logger::log("��� ���������� �� �� � ID " + to_string(id));
            }
            else {
                cout << "������������ �����!" << endl;
            }
        }
        else {
            cout << "������������ ID!" << endl;
        }
        break;
    }
    case 6: {
        string filename = safeStringInput("������� ��� ����� ��� ����������: ");
        saveData(filename, pipes, compressorStations);
        Logger::log("������ ��������� � ���� " + filename);
        break;
    }
    case 7: {
        string filename = safeStringInput("������� ��� ����� ��� ��������: ");
        loadData(filename, pipes, compressorStations);
        Logger::log("������ ��������� �� ����� " + filename);
        break;
    }
    case 8: {
        int filter = safeIntInput("1. �� ��������\n2. �� �������� '� �������'\n������� ������: ");
        if (filter == 1) {
            string name = safeStringInput("������� �������� �����: ");
            for (const auto& pipe : pipes) {
                if (pipe.getName() == name) {
                    pipe.print();
                }
            }
        }
        else if (filter == 2) {
            bool inRepair = safeIntInput("� �������? (1 - ��, 0 - ���): ") == 1;
            for (const auto& pipe : pipes) {
                if (pipe.isInRepair() == inRepair) {
                    pipe.print();
                }
            }
        }
        else {
            cout << "������������ �����!" << endl;
        }
        break;
    }
    case 9: {
        int filter = safeIntInput("1. �� ��������\n2. �� �������� ����������������� �����\n������� ������: ");
        if (filter == 1) {
            string name = safeStringInput("������� �������� ��: ");
            for (const auto& cs : compressorStations) {
                if (cs.getName() == name) {
                    cs.print();
                }
            }
        }
        else if (filter == 2) {
            double percent = safeDoubleInput("������� ������� ����������������� �����: ");
            for (const auto& cs : compressorStations) {
                if (cs.getUnusedShopPercent() >= percent) {
                    cs.print();
                }
            }
        }
        else {
            cout << "������������ �����!" << endl;
        }
        break;
    }
    case 10: {
        int filter = safeIntInput("1. �� ��������\n2. �� �������� '� �������'\n������� ������: ");
        vector<int> selectedPipes;
        if (filter == 1) {
            string name = safeStringInput("������� �������� �����: ");
            for (int i = 0; i < pipes.size(); ++i) {
                if (pipes[i].getName() == name) {
                    selectedPipes.push_back(i);
                }
            }
        }
        else if (filter == 2) {
            bool inRepair = safeIntInput("� �������? (1 - ��, 0 - ���): ") == 1;
            for (int i = 0; i < pipes.size(); ++i) {
                if (pipes[i].isInRepair() == inRepair) {
                    selectedPipes.push_back(i);
                }
            }
        }
        else {
            cout << "������������ �����!" << endl;
            break;
        }

        if (selectedPipes.empty()) {
            cout << "����� �� �������!" << endl;
            break;
        }

        int action = safeIntInput("1. �������������\n2. �������\n������� ��������: ");
        if (action == 1) {
            for (int id : selectedPipes) {
                pipes[id].setRepair(safeIntInput("� �������? (1 - ��, 0 - ���): ") == 1);
                Logger::log("����� � ID " + to_string(id) + " ���������������");
            }
        }
        else if (action == 2) {
            for (int id : selectedPipes) {
                Logger::log("����� � ID " + to_string(id) + " �������");
                pipes.erase(pipes.begin() + id);
            }
        }
        else {
            cout << "������������ �����!" << endl;
        }
        break;
    }
    case 0:
        cout << "����� �� ���������..." << endl;
        exit(0);
    default:
        cout << "������������ �����!" << endl;
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