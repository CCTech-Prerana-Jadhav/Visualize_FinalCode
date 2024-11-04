#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

#include "Triangulation.h"
#include "OpenGlWidget.h"
#include "GraphicsSynchronizer.h"

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



    // translation data
    QString inputFilePath;

};

//#pragma once
//
//#include <QMainWindow>
//#include <QPushButton>
//#include "OpenGlWidget.h"
//#include "GraphicsSynchronizer.h"
//#include "Triangulation.h"

//class Visualizer : public QMainWindow
//{
//    Q_OBJECT
//
//public:
//    Visualizer(QWidget* parent = nullptr);
//    ~Visualizer();
//
//private slots:
//    void onLoadFileClick();
//    void onTranslateClick();
//    void onExportClick();
//
//private:
//    void setupUi();
//    void loadAndSetData(const QString& filePath, OpenGlWidget* widget);
//    QString getOppositeExtension(const QString& filePath);
//    void writeFile(const QString& fileName, const Triangulation& triangulationData);
//    OpenGlWidget::Data convertTrianglulationToGraphicsObject(const Triangulation& inTriangulation);
//
//    QPushButton* loadFile;
//    QPushButton* translate;
//    QPushButton* exportFile;
//    OpenGlWidget* openglWidgetInput;
//    OpenGlWidget* openglWidgetOutput;
//    GraphicsSynchronizer* graphicsSynchronizer;
//
//    QString inputFilePath;
//    Triangulation triangulation;
//    Triangulation outputTriangulation;
//};


