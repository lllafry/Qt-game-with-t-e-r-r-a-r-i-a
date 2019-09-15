#include "loader.h"

loader::loader()
{
    int i, j;
    // sprites
    const int frms_amnt[] = {frms_furnace, frms_smoke, frms_torch};
    const QString sprites[] = {"furnace", "smoke", "torch"};
    for (j = 0; j < 3; j++)
        for (i = 0; i < frms_amnt[j]; i++)
            data[sprites[j]].append(
                        QPixmap(":res/" + sprites[j] + "_" + QString::number(i) + ".png"));
    // npc
    const int delta_npc_frames[] = {56, 56, 56, 58, 56, 56, 56, 56};
    const int delta_worker_frames = 58;
    QPixmap temp_npc;
    for (j = 0; j < anmt_citizen; j++)
    {
        temp_npc = QPixmap(":res/npc_" + QString::number(j) + ".png");
        for (i = 0; i < 14; i++)
        {
            if (i == 1) continue; // пропуск кадра падения
            data["npc_" + QString::number(j)].append(
                        temp_npc.copy(0, delta_npc_frames[j]*i, 40, 54));
        }
    }
    temp_npc = QPixmap(":res/npc_w.png");
    for (i = 0; i < 14; i++)
    {
        if (i == 1) continue; // пропуск кадра падения
        data["npc_w"].append(
                    temp_npc.copy(0, delta_worker_frames*i, 40, 54));
    }
    // mirror
    QTransform transf;
    transf.scale(-1, 1);
    for (auto key: data.keys())
    {
        for (i = 0; i < data[key].length(); i++)
            data[key + "_m"].append(data[key].at(i).transformed(transf));

    }
    // lift
    data["lift_on"].append(QPixmap(":res/lift_on.png"));
    data["lift_off"].append(QPixmap(":res/lift_off.png"));
    data["lift_clk"].append(QPixmap(":res/lift_clk.png"));
}
