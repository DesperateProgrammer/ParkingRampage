// ParkinRampage_LevelConverter.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

string Convert(FILE* input)
{
    string tmp = "";
    char lineb[1024];
    vector<string> map;
    int mapWidth = 0;
    int mapHeight = 0;
    string difficulty = "";
    while (!feof(input))
    {
        memset(lineb, 0, sizeof(lineb));
        fgets(lineb, sizeof(lineb), input);
        string line = string(lineb);
        if (!line.size())
            continue;
        if (line[0] == '#')
            continue;
        char ch = 'A';
        switch (line[0])
        {
            case '~':
            {
                tmp += "{\n";
                tmp += "  " + to_string(mapWidth) + ", " + to_string(mapHeight) + ", \n";
                tmp += "  " + difficulty + ", \n";
                vector<string> cars;
                uint32_t carFlags = 0;
                for (int x = 0; x < mapWidth; x++)
                {
                    for (int y = 0; y < mapHeight; y++)
                    {
                        char carId = map[y][x];
                        if (carId == '-')
                            continue;
                        if (carFlags & (1 << (tolower(carId) - 'a')))
                        {
                            // car was already added
                        }
                        else
                        {
                            carFlags |= (1 << (tolower(carId) - 'a'));
                            if (carId == tolower(carId))
                            {
                                // vertically
                                // find length:
                                int length = 0;
                                for (int _y = 0; _y < mapHeight; _y++)
                                    if (map[_y][x] == carId)
                                        length++;
                                string carTmp = "    {\n";
                                carTmp += "      " + ((rand() % 2) ? (string)"ORIENTATION_UP, " : (string)"ORIENTATION_DOWN, ");
                                switch (length)
                                {
                                    case 0:
                                    case 1:
                                        continue;
                                    case 2:
                                        if (carId == 'A')
                                            carTmp += (string)"CAR_TARGET, LENGTH_2, ";
                                        else
                                        {
                                            carTmp += (string)"CAR_";
                                            ch = 'A' + (rand() % 3);
                                            carTmp.push_back(ch);
                                            carTmp += ", LENGTH_2, ";
                                        }
                                        break;
                                    case 3:
                                        carTmp += (string)"TRUCK_";
                                        ch = 'A' + (rand() % 3);
                                        carTmp.push_back(ch);
                                        carTmp += ", LENGTH_3, ";
                                        break;
                                    case 4:
                                        // ToDo
                                        continue;
                                }
                                carTmp += to_string(x) + ", " + to_string(y) + ", \n";
                                carTmp += "    },\n";
                                cars.push_back(carTmp);
                            }
                            else
                            {
                                // horizontally
                                // vertically
                                // find length:
                                int length = 0;
                                for (int _x = 0; _x < mapWidth; _x++)
                                    if (map[y][_x] == carId)
                                        length++;
                                string carTmp = "    {\n";
                                carTmp += "      " + ((rand() % 2) ? (string)"ORIENTATION_LEFT, " : (string)"ORIENTATION_RIGHT, ");
                                switch (length)
                                {
                                case 0:
                                case 1:
                                    continue;
                                case 2:
                                    if (carId == 'A')
                                        carTmp += (string)"CAR_TARGET, LENGTH_2, ";
                                    else
                                    {
                                        carTmp += (string)"CAR_";
                                        ch = 'A' + (rand() % 2);
                                        carTmp.push_back(ch);
                                        carTmp += ", LENGTH_2, ";
                                    }
                                    break;
                                case 3:
                                    carTmp += (string)"TRUCK_";
                                    ch = 'A' + (rand() % 2);
                                    carTmp.push_back(ch);
                                    carTmp += ", LENGTH_3, ";
                                    break;
                                case 4:
                                    // ToDo
                                    continue;
                                }
                                carTmp += to_string(x) + ", " + to_string(y) + ", \n";
                                carTmp += "    },\n";
                                cars.push_back(carTmp);
                            }
                        }
                    }
                }
                tmp += "  " + to_string(cars.size()) + ", \n";
                tmp += "  {\n";
                for (int i = 0; i < cars.size(); i++)
                {
                    tmp += cars[i];
                }
                tmp += "  }\n";
                tmp += "},\n";
            }
            break;
            case '|':
            {
                map.push_back(line.substr(1, mapWidth));
                mapHeight++;
            }
            break;
            case 'B':
            {
                difficulty = "eBEGINNER";
                int i = 0;
                for (; (i + 1 < line.size()) && (line[i + 1] == '='); i++)
                    ;
                mapWidth = i;
                mapHeight = 0;
                map.clear();
                break;
            }
            case 'A':
            {
                difficulty = "eADVANCED";
                int i = 0;
                for (; (i + 1 < line.size()) && (line[i + 1] == '='); i++)
                    ;
                mapWidth = i;
                mapHeight = 0;
                map.clear();
                break;
            }
            case 'I':
            {
                difficulty = "eINTERMEDIATE";
                int i = 0;
                for (; (i + 1 < line.size()) && (line[i + 1] == '='); i++)
                    ;
                mapWidth = i;
                mapHeight = 0;
                map.clear();
                break;
            }
            case 'E':
            {
                difficulty = "eEXPERT";
                int i = 0;
                for (; (i + 1 < line.size()) && (line[i + 1] == '='); i++)
                    ;
                mapWidth = i;
                mapHeight = 0;
                map.clear();
                break;
            }
        }
    }
    return tmp;
}



int main(int argc, char **argv)
{
    string levels = "// This file was generated\n";
    levels += "// Please edit the .lvl files to add or modify entries\n";
    levels += "// ===================================================\n";

    if (argc < 3)
    {
        printf("Usage:\n");
        printf("  LevelConvert.exe <lvlFile> <outputFile>\n");
        printf("=========================================\n");
        for (int i = 0; i < argc; i++)
            printf("  %i: %s\n", i, argv[i]);
        return 0;
    }

    FILE* f = 0;
    fopen_s(&f, argv[1], "rb");
    if (!f)
        return -1;
    levels += Convert(f);
    fclose(f);
    fopen_s(&f, argv[2], "wb+");
    if (!f)
        return -1;
    fwrite(levels.c_str(), levels.size(), 1, f);
    fclose(f);
    printf("Converted %s\n", argv[1]);
    return 0;
}