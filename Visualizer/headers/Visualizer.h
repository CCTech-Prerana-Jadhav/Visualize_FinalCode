#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

#include "Triangulation.h"
#include "OpenGlWidget.h"
#include "GraphicsSynchronizer.h"

using namespace Geometry;

class Visualizer : public QMainWindow
{
    Q_OBJECT

public:
    Visualizer(QWidget *parent = nullptr);
    ~Visualizer();

private slots:
    void onLoadFileClick();
    void onTranslateClick();
    void onExportClick();

private:
    void setupUi();
    OpenGlWidget::Data convertTrianglulationToGraphicsObject(const Triangulation& inTriangulation);

private:
    Triangulation triangulation;
    Triangulation outputTriangulation;

    QPushButton* loadFile;
    QPushButton* translate;
    QPushButton* exportFile;

    OpenGlWidget* openglWidgetInput;
    OpenGlWidget* openglWidgetOutput;

    GraphicsSynchronizer* graphicsSynchronizer;

    QVector<GLfloat> vertices;

    QString tempFileName;


    // translation data
    QString inputFilePath;
    QString inputFileName;

};
