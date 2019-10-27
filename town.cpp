#include "town.h"

town::town() : QGraphicsView()
{
    scene.setSceneRect(0, 0, 1696, 912);

    this->setRenderHint(QPainter::Antialiasing);
    this->setCacheMode(QGraphicsView::CacheBackground);
    this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    this->setBackgroundBrush(QPixmap(":res/scene.png"));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setDragMode(QGraphicsView::ScrollHandDrag);
    this->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "city"));

    this->setScene(&scene);

    // sprites
    // .house 0
    sprites.append(new sprite("smoke", 144, 544, 4));
    sprites.append(new sprite("furnace", 144, 688, 7));

    // .house 1
    for (int i= 0; i < 5; i++)
    {
        sprites.append(new sprite("torch_m", 464, 336 + i * 80, 7));
        sprites.append(new sprite("torch", 704, 336 + i * 80, 7));
    }
    // .house 2
    for (int i= 0; i < 6; i++)
    {
        sprites.append(new sprite("torch_m", 1072, 256 + i * 80, 7));
        sprites.append(new sprite("torch", 1392, 256 + i * 80, 7));
    }
    for (sprite *psprite : sprites)
        scene.addItem(psprite);
    // elevators
    elevators.append(new lift(560, 640));
    elevators.append(new lift(1328, 640));
    QObject::connect(&tm_tick, SIGNAL(timeout()), elevators.at(0), SLOT(move()));
    QObject::connect(&tm_tick, SIGNAL(timeout()), elevators.at(1), SLOT(move()));
    for (lift *pelevator : elevators)
        scene.addItem(pelevator);
    // workers
    workers.append(new worker(96, 672));
    for (lift *pelevator : elevators)
    {
        QObject::connect(&tm_tick, SIGNAL(timeout()), workers.at(0), SLOT(next_tick()));
        QObject::connect(workers.at(0), SIGNAL(fixed(int)), pelevator, SLOT(fixed(int)));
        QObject::connect(pelevator, SIGNAL(need_worker(int)),
                         workers.at(0), SLOT(new_task(int)));
    }
    QObject::connect(&settings, SIGNAL(set_val(int,int,int,int,int,int)),
                     this, SLOT(update_settings(int,int,int,int,int,int)));

    QObject::connect(&tm_update, SIGNAL(timeout()), &scene, SLOT(update()));
    tm_update.start(1000 / 50);
    QGraphicsProxyWidget *t = scene.addWidget(&settings);
    t->setPos(770, 280);
//    QTime tm_work;
//    tm_work.start();
//    this->update_settings(1000, 10, 10, 10, 100, 70);
//    qDebug() << tm_work.restart() << " ms";
}

town::~town()
{
    for (sprite *psprite : sprites)
        delete psprite;
    for (worker *pworker : workers)
        delete pworker;
    for (citizen *pcitizen : citizens)
        delete pcitizen;
    for (lift *pelevator : elevators)
        delete pelevator;
}

void town::update_settings(int num_citizen,
                           int num_game_speed,
                           int num_speed_citizen,
                           int num_speed_worker,
                           int num_elevator_maxcrew,
                           int num_elevator_loadcrew)
{
    if (num_elevator_maxcrew < num_elevator_loadcrew)
        std::logic_error("town::update_settings");
    //elevator
    for (lift *pelevator : elevators)
    {
        pelevator->set_max_crew(num_elevator_maxcrew);
        pelevator->set_max_load_crew(num_elevator_loadcrew);
    }
    //worker
    for (worker *pworker : workers)
        pworker->set_max_speed(num_speed_worker);
    // citizens
    tm_tick.start(1000 / num_game_speed);

    if (!citizens.length())
    {
        for (int i = 0; i < num_citizen; ++i)
            citizens.append(new citizen(qrand() % 8, qrand() % 21));
        for (citizen *pcitizen : citizens)
        {
            QObject::connect(&tm_tick, SIGNAL(timeout()), pcitizen, SLOT(next_tick()));
            for(lift *pelevator : elevators)
            {
                QObject::connect(pelevator,
                                 SIGNAL(current_pos(int,int,bool,QVector<int>)),
                                 pcitizen,
                                 SLOT(check_elevator(int,int,bool,QVector<int>)));
                QObject::connect(pcitizen, SIGNAL(call(int,int,int,int)),
                                 pelevator, SLOT(elevator_call(int,int,int,int)));
                QObject::connect(pcitizen, SIGNAL(enter_elevator(int,int,int)),
                                 pelevator, SLOT(entry(int,int,int)));
                QObject::connect(pcitizen, SIGNAL(leave_elevator(int)),
                                 pelevator, SLOT(leave(int)));
            }
        }

        for (citizen *pcitizen : citizens)
            scene.addItem(pcitizen);

        for (worker *pworker : workers)
            scene.addItem(pworker);
    }
    for (citizen *pcitizen : citizens)
        pcitizen->set_max_speed(num_speed_citizen);
}

void town::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        settings.change_visible();
        break;
    }
    QGraphicsView::keyPressEvent(event);
}
