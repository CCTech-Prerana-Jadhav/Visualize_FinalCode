#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

#include "Triangulation.h"
#include "OpenGlWidget.h"
#include "GraphicsSynchronizer.h"
#include <QProgressBar>
using namespace Geometry;
class Visualizer : public QMainWindow
{
    Q_OBJECT

public:
    Visualizer(QWidget* parent = nullptr);
    ~Visualizer();

private slots:
    void onLoadFileClick();
    void onTranslateClick();
    void onExportClick();
    void onScaleClick();
    void onRotateClick();
    void onDistanceTranslateClick();

private:
    void setupUi();
    OpenGlWidget::Data convertTrianglulationToGraphicsObject(const Triangulation& inTriangulation);

private:
    Triangulation triangulation;
    Triangulation outputTriangulation;

    QPushButton* loadFile;
    QPushButton* translate;
    QPushButton* exportFile;
    QPushButton* scale;
    QPushButton* rotate;
    QPushButton* distanceTranslation;

    QProgressBar* progressbar;

    OpenGlWidget* openglWidgetInput;
    OpenGlWidget* openglWidgetOutput;

    GraphicsSynchronizer* graphicsSynchronizer;

    QVector<GLfloat> vertices;



    // translation data
    QString inputFilePath;
    QString tempFilePath;
    QString outputFilePath;

    void loadSTLFile(const QString&, Triangulation& triangulation, OpenGlWidget*);
    void loadOBJFile(const QString&, Triangulation& triangulation, OpenGlWidget*);
};