#include "Qcreator.h"

#define _CRT_SECURE_NO_WARNINGS

int main()
{
    setlocale(LC_ALL, "ru");
    SchemeReader blsc;
    blsc.DeserializeFromJson("input.json");
    QShaper qcreator(&blsc);
    qcreator.Init();
}

